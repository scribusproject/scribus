<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS><TS version="1.1" language="ar">
<defaultcodec></defaultcodec>
<context>
    <name></name>
    <message>
        <location filename="" line="0"/>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
    <message>
        <location filename="" line="0"/>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getVGuides()

See getHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setVGuides()

See setHGuides.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <location filename="" line="0"/>
        <source>About Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%3-%2-%1 %4 %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Using Ghostscript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Ghostscript version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Build ID:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;About</source>
        <translation>&amp;حول</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Development Team:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Contributions from:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Windows&amp;#174; Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Official Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Other Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tango Project Icons:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;uthors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Official Translations and Translators:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Previous Translation Contributors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Translations</source>
        <translation>&amp;ترجمة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Homepage</source>
        <translation>صفحة الويب</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Online Reference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bugs and Feature Requests</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mailing List</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Online</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <location filename="" line="0"/>
        <source>Filename:</source>
        <translation>اسم الملف:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Version:</source>
        <translation>الاصدار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No</source>
        <translation>لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enabled:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Release Date:</source>
        <translation>تاريخ التحرير:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Description:</source>
        <translation>تعليق:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Author(s):</source>
        <translation>المؤلف:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copyright:</source>
        <translation>حقوق النشر:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>License:</source>
        <translation>التصريح:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Scribus: About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Open...</source>
        <translation>&amp;فتح...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;As...</source>
        <translation>&amp;حفظ باسم...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;العودة للمحفوظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Get Text...</source>
        <translation>استيراد نص...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Append &amp;Text...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Get Image...</source>
        <translation>استيراد صورة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;Text...</source>
        <translation>حفظ &amp;نص...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as P&amp;DF...</source>
        <translation>&amp;حفظ كاكروبات...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Print...</source>
        <translation>&amp;طباعة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print Previe&amp;w</source>
        <translation>&amp;معاينة طباعة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Quit</source>
        <translation>&amp;انهاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Undo</source>
        <translation>&amp;اعادة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Redo</source>
        <translation>&amp;تكرار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cu&amp;t</source>
        <translation>&amp;قص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paste (&amp;Absolute)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select &amp;All</source>
        <translation>&amp;تحديد الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Deselect All</source>
        <translation>&amp;الغاء تحديد الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;بحث/استبدال...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Image...</source>
        <translation>تحرير صورة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;olors...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;tyles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;JavaScripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>P&amp;references...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Other...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left</source>
        <translation>&amp;يسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Center</source>
        <translation>&amp;توسيط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Right</source>
        <translation>&amp;يمين</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Block</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;%1 %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underline &amp;Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Strike Through</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Small &amp;Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Su&amp;perscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Su&amp;bscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;hadow</source>
        <translation>&amp;ظل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Image Effects</source>
        <translation>&amp;مؤثرات الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Tabulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;نسخ مطابق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;نُسخ مطابقة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Group</source>
        <translation>&amp;مجموعة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is &amp;Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Si&amp;ze is Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Printing Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Flip Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Flip Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lower to &amp;Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Raise to &amp;Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Lower</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Raise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>More Info...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Update Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Adjust Frame to Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Low Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Normal Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Full Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert...</source>
        <translation>&amp;اضافة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Im&amp;port...</source>
        <translation>&amp;استيراد...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete...</source>
        <translation>&amp;حذف...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy...</source>
        <translation>&amp;نسخ...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Move...</source>
        <translation>&amp;نقل...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Convert to Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Manage Page Properties...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Fit in window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;50%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;75%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;100%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;200%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show G&amp;uides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Text Frame Columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Text Chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Rulers</source>
        <translation type="unfinished">عرض المسطرة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Properties</source>
        <translation>&amp;الاعدادات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Layers</source>
        <translation>&amp;الشرائح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Measurements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Action &amp;History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Tools</source>
        <translation>&amp;الادوات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>T&amp;able</source>
        <translation>&amp;جدول</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shape</source>
        <translation>&amp;شكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Line</source>
        <translation>&amp;خط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Freehand Line</source>
        <translation>&amp;خط حر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rotate Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Zoom in or out</source>
        <translation>تكبير او تصغير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Zoom in</source>
        <translation>تكبير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Zoom out</source>
        <translation>تصغير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Contents of Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Text...</source>
        <translation>تحرير نص...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Link Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unlink Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Eye Dropper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy Item Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit the text with the Story Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Text Frame</source>
        <translation>اضافة اطار نص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Image Frame</source>
        <translation>اضافة اطار صورة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Table</source>
        <translation>اضافة جدول</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Shape</source>
        <translation>اضافة شكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Polygon</source>
        <translation>اضافة مضلع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Line</source>
        <translation>اضافة خط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Bezier Curve</source>
        <translation>اضافة منحنى بيزير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Freehand Line</source>
        <translation>اضافة خط حر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;ادارة الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dehyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Generate Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Cascade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Tile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;About Scribus</source>
        <translation>&amp;حول سكريبس</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>About &amp;Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Toggle Palettes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Toggle Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Smart &amp;Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Non Breaking &amp;Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page &amp;Number</source>
        <translation>&amp;رقم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Line</source>
        <translation>خط جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Frame Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Column Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copyright</source>
        <translation>حقوق النشر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Registered Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Solidus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bullet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Middle Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Em Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>En Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Figure Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Quotation Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apostrophe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Straight Double</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Reversed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Left Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Right Guillemet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Low Single Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Low Double Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CJK Single Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CJK Single Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CJK Double Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CJK Double Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>En Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Em Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thin Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thick Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mid Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hair Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Smart Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Non Breaking Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Page Number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>fi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>fl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>ffi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>ffl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>ft</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>st</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Relative to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>First Selected</source>
        <translation>الاختيار الاول</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Last Selected</source>
        <translation>الاختيار الاخير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align bottoms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align right sides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Center on vertical axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align left sides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Center on horizontal axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align tops</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute right sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute bottoms equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute centers equidistantly horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make vertical gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute left sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute centers equidistantly vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute tops equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distribute the items with the distance specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <location filename="" line="0"/>
        <source>Align Text Left</source>
        <translation>صف لليسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align Text Right</source>
        <translation>صف لليمين</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align Text Center</source>
        <translation>توسيط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align Text Justified</source>
        <translation>محاذاة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align Text Forced Justified</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <location filename="" line="0"/>
        <source>Field Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type:</source>
        <translation>النوع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name:</source>
        <translation>الاسم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tool-Tip:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font for use with PDF 1.3:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Border</source>
        <translation>الاطار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color:</source>
        <translation>اللون:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Width:</source>
        <translation>العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thin</source>
        <translation>السمك</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Wide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Solid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dashed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Beveled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Inset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Other</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Read Only</source>
        <translation>للقراءة فقط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Required</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Don&apos;t Export Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Visibility:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Visible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hidden</source>
        <translation>اخفاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Print</source>
        <translation>لا طباعة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No View</source>
        <translation>لا عرض</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text for Button Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text for Roll Over</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Icons</source>
        <translation>الرموز</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use Icons</source>
        <translation>استخدام الرموز</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove</source>
        <translation>ازالة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pressed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Roll Over</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Icon Placement...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Highlight</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>highlight</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Invert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Outlined</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Push</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Multi-Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Password</source>
        <translation>كلمة سر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Limit of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do Not Scroll</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do Not Spell Check</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cross</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Diamond</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Circle</source>
        <translation>دائرة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Star</source>
        <translation>نجمة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Square</source>
        <translation>مربع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default is Checked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Editable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options</source>
        <translation>خيارات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>action</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>JavaScript</source>
        <translation>جافاسكربت</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Go To</source>
        <translation>اذهب إلى</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Submit Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import Data</source>
        <translation>استيراد بيانات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Event:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mouse Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mouse Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mouse Enter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mouse Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>On Focus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>On Blur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit...</source>
        <translation>تحرير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Submit to URL:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Submit Data as HTML</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import Data from:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Destination</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change...</source>
        <translation>تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Field is formatted as:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Percentage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Date</source>
        <translation>التاريخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Time</source>
        <translation>الوقت</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Decimals:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use Currency Symbol</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Prepend Currency Symbol</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Formatting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Percent Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Date Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Time Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Format:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keystroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value is not validated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value must be greater than or equal to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>and less or equal to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom validate script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Validate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value is not calculated</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value is the</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>sum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>product</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>average</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>minimum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>maximum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>of the following fields:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pick...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom calculation script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Calculate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flag is ignored for PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enter a comma separated list of fields here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Example:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selection Change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <location filename="" line="0"/>
        <source>Annotation Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>External Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>External Web-Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Destination</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hange...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;X-Pos</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Current &amp;page</source>
        <translation>&amp;الصفحة الحالية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Even pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>O&amp;dd pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;All pages</source>
        <translation>&amp;كل الصفحات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Within range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>to</source>
        <translation>الى</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Barcode Generator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <location filename="" line="0"/>
        <source>12 or 13 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>8 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>11 or 12 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>7 or 8 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>5 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2 digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of digits and capital letters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Variable number of hexadecimal characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error opening file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Barcode incomplete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Barcode Creator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Co&amp;de:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select one of the available barcode type here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset the barcode samples</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Include text in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Guard whitespace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+G</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Draw arrows to be sure of space next the code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>I&amp;nclude checksum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Generate and include a checksum in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Incl&amp;ude checksum digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Include the checksum digit in the barcode text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colors</source>
        <translation>الالوان</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background color - under the code lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of the lines in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of the text and numbers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hints and help is shown here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview of the result. 72dpi sample.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <location filename="" line="0"/>
        <source>Main</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Scrapbook Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scrapbook (*.scs)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a scrapbook file to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rename</source>
        <translation>اعادة تسمية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete</source>
        <translation>حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Entry</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Load...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;As...</source>
        <translation>&amp;حفظ باسم...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import Scrapbook File...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <location filename="" line="0"/>
        <source>Move Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <location filename="" line="0"/>
        <source>Icon Placement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layout:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Icon only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption below Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption above Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption right to Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption left to Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Caption overlays Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scale:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Always</source>
        <translation>دائماً</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>When Icon is too small</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>When Icon is too big</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Never</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scale How:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Proportional</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Non Proportional</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Icon</source>
        <translation>رمز</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Activate Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>System Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;RGB Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;CMYK Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Monitor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>P&amp;rinter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering Intents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>M&amp;onitor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pr&amp;inter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default color profile for imported CMYK images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default color profile for imported RGB images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default color profile for solid colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Color</source>
        <translation>تحرير لون</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color &amp;Model</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Web Safe RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New</source>
        <translation>جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Old</source>
        <translation>قديم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>HSV-Colormap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>M:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>K:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dynamic Color Bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Static Color Bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>R:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>G:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>B:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name of the color is not unique</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <location filename="" line="0"/>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <location filename="" line="0"/>
        <source>Select Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font:</source>
        <translation>الخط:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close this dialog and return to text editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Full Character Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Basic Latin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Latin-1 Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Latin Extended-A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Latin Extended-B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>General Punctuation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Super- and Subscripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Currency Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Letterlike Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arrows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mathematical Operators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Box Drawing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Block Elements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Geometric Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Miscellaneous Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dingbats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ligatures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Specials</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Greek</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Greek Extended</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cyrillic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cyrillic Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arabic</source>
        <translation>عربي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arabic Extended A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arabic Extended B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hebrew</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <location filename="" line="0"/>
        <source>Glyphs missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text overflow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object is not on a Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Missing Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object has transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object is a PDF Annotation or Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object is a placed PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Problems</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Current Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Ignore Errors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <location filename="" line="0"/>
        <source>Choose Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available Styles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Warning</source>
        <translation>تحذير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Colors</source>
        <translation>الالوان</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;uplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Current Color Set:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a color set to load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save the current color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create a new color within the current set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete the currently selected color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy of %1</source>
        <translation>نسخ من %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Color</source>
        <translation>لون جديد</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <location filename="" line="0"/>
        <source>Monochromatic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Split Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tetradic (Double Complementary)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Base Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Monochromatic Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Monochromatic Dark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>1st. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2nd. Analogous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>1st. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2nd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>3rd. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>4th. Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>1st. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2nd. Triadic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>1st. Tetradic (base opposite)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2nd. Tetradic (angle)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Cr&amp;eate color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;olor Components...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import existing color...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Merge colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Replace colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>E&amp;xit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;olor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal Vision</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Protanopia (Red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Deuteranopia (Green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tritanopia (Blue)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Full Color Blindness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vision Defect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select Method:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Angle (0 - 90 degrees):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Merge Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Replace Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merge created colors into the document colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Leave colors untouched</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colors of your chosen color scheme</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merging colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color %1 exists already!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color %1 appended.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Now opening the color manager.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Merging</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Color</source>
        <translation>لون جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Color Wheel...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color setting helper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color selector with color theory included.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Apply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Cancel</source>
        <translation>&amp;الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>color name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;OK</source>
        <translation>&amp;موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Warning</source>
        <translation>تحذير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>4-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Middle Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Monday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tuesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Wednesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thursday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Friday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sunday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>January</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>March</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>April</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>May</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>June</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>July</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>August</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>September</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>October</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>November</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>December</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Opacity:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Vector</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Diagonal Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cross Diagonal Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Radial Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Free linear Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Free radial Gradient</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Line Color Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Fill Color Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal or gradient fill method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the transparency for the color selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>CSV Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Field delimiter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>(TAB)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value delimiter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>delimiter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>First row is a header</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <location filename="" line="0"/>
        <source>Printer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Even Pages only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Odd Pages only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No</source>
        <translation>لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orientation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>N-Up Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page per Sheet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pages per Sheet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Compress File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Include Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Encoding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <location filename="" line="0"/>
        <source>Set Color Components</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>HSV</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>H:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>V:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>R:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>G:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>B:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>M:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>K:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set &amp;RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set C&amp;MYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set &amp;HSV</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <location filename="" line="0"/>
        <source>Cancelled by user</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <location filename="" line="0"/>
        <source>Delete Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace With:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <location filename="" line="0"/>
        <source>Delete Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>to:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <location filename="" line="0"/>
        <source>Delete Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace With:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Style</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <location filename="" line="0"/>
        <source>Importing failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Importing Word document failed 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <location filename="" line="0"/>
        <source>Document Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Title:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Author:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Keywords:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Descri&amp;ption:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>P&amp;ublisher:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Contributors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dat&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>T&amp;ype:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>F&amp;ormat:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Identi&amp;fier:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Source:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Relation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Co&amp;verage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ri&amp;ghts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documen&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Further &amp;Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A person or organisation responsible for making the document available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <location filename="" line="0"/>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>1, 2, 3, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>i, ii, iii, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>I, II, III, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>a, b, c, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A, B, C, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Number Out Of Bounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>From</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>auto add</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>types</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Boolean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Integer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Real Number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>String</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type</source>
        <translation>النوع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Auto Add To</source>
        <translation>اضافه اليه إلى</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <location filename="" line="0"/>
        <source>Setup Printer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print Destination</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Options...</source>
        <translation>&amp;خيارات...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File:</source>
        <translation>&amp;ملف:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hange...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;lternative Printer Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Co&amp;mmand:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print &amp;All</source>
        <translation>&amp;طباعة الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print Current Pa&amp;ge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print &amp;Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>N&amp;umber of Copies:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print Separations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print in Color if Available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print in Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All</source>
        <translation>الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cyan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magenta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yellow</source>
        <translation>اصفر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Black</source>
        <translation>اسود</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript Level 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript Level 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript Level 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options</source>
        <translation>خيارات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page</source>
        <translation>الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color</source>
        <translation>اللون</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Advanced Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Print</source>
        <translation>&amp;طباعة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to retrieve printer settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <location filename="" line="0"/>
        <source>Importing PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Analyzing PostScript:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Importing File:
%1
failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fatal Error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Generating Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Converting of %1 images failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;الاسم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distances</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Lines:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance from Text:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview of the Paragraph Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background</source>
        <translation>الخلفية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name of your paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font of selected text or object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Size</source>
        <translation>حجم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of text fill</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spacing above the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spacing below the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Toggles sample text of this paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Auto</source>
        <translation>آلي</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <location filename="" line="0"/>
        <source>Editor</source>
        <translation>المحرر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Open...</source>
        <translation>&amp;فتح...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;As...</source>
        <translation>&amp;حفظ باسم...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save and Exit</source>
        <translation>&amp;حفظ واغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;اغلاف بدون حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Undo</source>
        <translation>&amp;اعاده</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Redo</source>
        <translation>&amp;تكرار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cu&amp;t</source>
        <translation>&amp;قص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Get Field Names</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Image Effects</source>
        <translation>مؤثرا الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options:</source>
        <translation>الخيارات:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color:</source>
        <translation>اللون:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Brightness:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Contrast:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Blur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Brightness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colorize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Contrast</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Invert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Posterize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sharpen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Effects in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Export Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export as Image(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Export to Directory:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hange...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image &amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;All pages</source>
        <translation>&amp;كل الصفحات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export a range of pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export only the current page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available export formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change the output directory</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <location filename="" line="0"/>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Darken</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lighten</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hue</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Luminosity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dissolve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hard Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Soft Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Difference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exclusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Dodge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Burn</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exlusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Blend Mode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Opacity:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background</source>
        <translation>الخلفية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paths</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <location filename="" line="0"/>
        <source>Size:</source>
        <translation>الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resolution:</source>
        <translation>الدقة:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unknown</source>
        <translation>غير معروف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colorspace:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Title:</source>
        <translation>العنوان:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Title</source>
        <translation>لا عنوان</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Author:</source>
        <translation>المؤلف:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <location filename="" line="0"/>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <location filename="" line="0"/>
        <source>Available Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Substitutions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Additional Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>اسم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Available Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Name</source>
        <translation>اسم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font &amp;Substitutions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hange...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;dd...</source>
        <translation>&amp;اضافة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Remove</source>
        <translation>&amp;ازالة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Additional &amp;Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <location filename="" line="0"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start searching</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size of the selected font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample will be shown after key release</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>User</source>
        <comment>font preview</comment>
        <translation>المستخدم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>System</source>
        <comment>font preview</comment>
        <translation>النظام</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Fonts Preview</source>
        <translation>معاينة خط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Quick Search:</source>
        <translation>&amp;بحث سريع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Search</source>
        <translation>&amp;بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Name</source>
        <translation>اسم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Doc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type</source>
        <translation>النوع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Subset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Font Size:</source>
        <translation>&amp;حجم الخط:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample text to display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Se&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Append</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Font Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Preview dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Font Substitution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Original Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Substitution Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make these substitutions permanent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <location filename="" line="0"/>
        <source>Add, change or remove color stops here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Manage Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;elete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;dd</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>De&amp;lete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;olumns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Row &amp;Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colum&amp;n Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Refer to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview</source>
        <translation>معاينة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Lock Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <location filename="" line="0"/>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Search</source>
        <translation>&amp;بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relevance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Se&amp;arch</source>
        <translation>&amp;بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>De&amp;lete All</source>
        <translation>&amp;حذف الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Book&amp;marks</source>
        <translation>&amp;المفضلة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Print...</source>
        <translation>&amp;طباعة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>E&amp;xit</source>
        <translation>&amp;خروج</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Find...</source>
        <translation>&amp;بحث...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Find &amp;Next</source>
        <translation>&amp;بحث التالي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Find &amp;Previous</source>
        <translation>&amp;بحث السابق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;elete All</source>
        <translation>&amp;حذف الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Searching is case unsensitive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Find</source>
        <translation>بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search Term:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Bookmark&apos;s Title:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <location filename="" line="0"/>
        <source>Possible Hyphenation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Accept</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Skip</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Hyphenation Suggestions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Language:</source>
        <translation>&amp;اللغة:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Smallest Word:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Image Info</source>
        <translation>معلومات الصورة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>General Info</source>
        <translation>معلومات عامة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Date / Time:</source>
        <translation type="unfinished">التاريخ / الوقت:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No</source>
        <translation>لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Profile Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Has Layers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>EXIF Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Artist:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>User Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Camera Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Description:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scanner Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports EPS Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <location filename="" line="0"/>
        <source>Insert Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>after Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>at End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <location filename="" line="0"/>
        <source>Insert Table</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of columns:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit JavaScripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to delete this script?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;No</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Current Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select a Key for this Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;No Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set &amp;Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Load the selected shortcut set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ctrl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shift</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Meta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Meta+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shift+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ctrl+</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Delete Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add a new layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Line Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;نسخ مطابق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy of %1</source>
        <translation>نسخ من %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Yes</source>
        <translation>&amp;نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;No</source>
        <translation>&amp;لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <location filename="" line="0"/>
        <source>%</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Width:</source>
        <translation>عرض الخط:</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flat Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bevel Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Solid Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dashed Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dotted Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dash Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Select Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Author:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Get More:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>XML File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Manage Page Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <location filename="" line="0"/>
        <source>Preset Layouts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;اسفل:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Top:</source>
        <translation>&amp;اعلى:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Right:</source>
        <translation>&amp;يمين:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left:</source>
        <translation>&amp;يسار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Printer Margins...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>O&amp;utside:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Duplicate the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;No</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy of %1</source>
        <translation>نسخ من %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name:</source>
        <translation>الاسم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy #%1 of </source>
        <translation>نسخ #%1 من </translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unable to Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rename Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Name:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <location filename="" line="0"/>
        <source>Multiple Duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <location filename="" line="0"/>
        <source>pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distances</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>DX:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>DY:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Angle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Length:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;From Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Chan&amp;ge...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import Page(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> from 0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> from %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <location filename="" line="0"/>
        <source>Missing Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>instead</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <location filename="" line="0"/>
        <source>Move Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of copies:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Page(s):</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;X1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X&amp;2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y&amp;1:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y2:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X, Y, &amp;Z</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Geometry</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shape:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>R&amp;ound
Corners:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance of Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To&amp;p:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;اسفل:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left:</source>
        <translation>&amp;يسار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Right:</source>
        <translation>&amp;يمين:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>T&amp;abulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Path Text Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance from Curve:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill Rule</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Even-Odd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Non Zero</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use &amp;Bounding Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Use Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>St&amp;yle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lan&amp;guage:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Actual X-DPI:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Actual Y-DPI:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X-Sc&amp;ale:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y-Scal&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scale &amp;To Frame Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>P&amp;roportional</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Input Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>End Points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>T&amp;ype of Line:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>End Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line &amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ed&amp;ges:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bevel Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flat Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Endings:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cell Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line at Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line at the Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line at the Right </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line at Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name of selected object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal position of current basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical position of current basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rotation of object at current basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select top left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select top right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select bottom left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select bottom right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select center for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flip Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flip Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move one level up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move one level down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move to front</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move to back</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lock or unlock the object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lock or unlock the size of the object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable or disable printing of the object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font of selected text or object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Size</source>
        <translation>حجم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scaling width of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of text fill</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right to Left Writing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style of current paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphenation language of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change settings for left or end points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pattern of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thickness of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type of line joins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line style of current object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose the shape of frame...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit shape of the frame...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set radius of corner rounding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of columns in text frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance of text from top of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance of text from bottom of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance of text from left of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance of text from right of frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit tab settings of text frame...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow the image to be a different size to the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal offset of image within frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical offset of image within frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resize the image horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resize the image vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keep the X and Y scaling the same</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keep the aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make the image fit within the size of the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use image proportions rather than those of the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Source profile of the image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering intent for the image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Auto</source>
        <translation>آلي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flat Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Square Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Cap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bevel Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Solid Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dashed Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dotted Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dash Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>pt</source>
        <translation>نقطة</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Overall Progress:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Cancel</source>
        <translation>&amp;الغاء</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>My &amp;Plugin</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <location filename="" line="0"/>
        <source>Scribus - My Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The plugin worked!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open &amp;Existing Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open Recent &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not show this dialog again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document page size, either a standard size or a custom size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orientation of the document&apos;s pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Initial number of pages of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create text frames automatically when new pages are added</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of columns to create in automatically created text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between automatically created columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>N&amp;umber of Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Default Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Automatic Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>New &amp;from Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Load documents with predefined layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Nodes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Absolute Coordinates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit &amp;Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Reset Contour Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;End Editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Nodes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Control Points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add Nodes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete Nodes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Control Points Independently</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Control Points Symmetrical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset Control Points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset this Control Point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close this Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror the Path Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror the Path Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rotate the Path Clockwise</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shrink the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Angle of Rotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>% to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <location filename="" line="0"/>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>OpenDocument Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not ask again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Linewidth</source>
        <translation>عرض الخط</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Save as PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cha&amp;nge...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <location filename="" line="0"/>
        <source>Saving PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <location filename="" line="0"/>
        <source>Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable &amp;Antialiasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display Trans&amp;parency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Display CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cyan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magenta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yellow</source>
        <translation>اصفر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Black</source>
        <translation>اسود</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close</source>
        <translation>اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resize the scale of the page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All</source>
        <translation>الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <location filename="" line="0"/>
        <source>Processing Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <location filename="" line="0"/>
        <source>Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polyline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PathText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy of</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type</source>
        <translation>النوع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;OK</source>
        <translation>&amp;موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Cancel</source>
        <translation>&amp;الغاء</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <location filename="" line="0"/>
        <source>First Page is:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <location filename="" line="0"/>
        <source>%1 of %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 of %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <location filename="" line="0"/>
        <source>Quarto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Foolscap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Government Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Executive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Crown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Large Post</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Royal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Elephant</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Quad Demy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>STMT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>E</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <location filename="" line="0"/>
        <source>Result</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search Results for: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview</source>
        <translation>معاينة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <location filename="" line="0"/>
        <source>Manage Pictures</source>
        <translation>ادارة الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print</source>
        <translation>طباعة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search</source>
        <translation>بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel Search</source>
        <translation>الغاء البحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Goto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select a base directory for search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus - Image Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The search failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export As Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exports selected pages as bitmap images.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>خطأ مجهول</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <location filename="" line="0"/>
        <source>Plugin Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>How to run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type</source>
        <translation>النوع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Load it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugin ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No</source>
        <translation>لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <location filename="" line="0"/>
        <source>Polygon Properties</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <location filename="" line="0"/>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Factor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of corners for polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>GUI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Theme:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Documents:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Change...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hange...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Scripts:</source>
        <translation>&amp;سكربت...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cha&amp;nge...</source>
        <translation>&amp;تغيير...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>General</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Units:</source>
        <translation>الوحدات:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Autosave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>min</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Interval:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Undo/Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Action history length</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Typography</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tools</source>
        <translation>ادوات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fonts</source>
        <translation>خطوط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color:</source>
        <translation>اللون:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Pictures</source>
        <translation>عرض الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Top:</source>
        <translation>&amp;اعلى:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left:</source>
        <translation>&amp;يسار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;اسفل:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Right:</source>
        <translation>&amp;يمين:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal:</source>
        <translation>افقي:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical:</source>
        <translation>رأسي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript Interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resolution:</source>
        <translation>الدقة:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image Processing Tool</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>External Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Startup Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Miscellaneous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default documents directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Default orientation of document pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Time period between saving automatically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color for paper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the default zoom level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Locate Ghostscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Locate your image editor</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Save...</source>
        <translation>حفظ...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <location filename="" line="0"/>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Migrate Old Scribus Settings?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Postscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF 1.4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF/X-3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error Writing Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error Loading Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <comment>layout type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Gutenberg</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magazine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fibonacci</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Golden Mean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Nine Parts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Open...</source>
        <translation>&amp;فتح...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;As...</source>
        <translation>&amp;حفظ باسم...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Exit</source>
        <translation>&amp;خروج</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Run</source>
        <translation>&amp;تشغيل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Run As &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save Output...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Script</source>
        <translation>&amp;سكربت</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Python Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Script Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Write your commands here. A selection is processed as script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output of your script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Python Scripts (*.py)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open Python Script File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Warning</source>
        <translation>تحذير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Hu&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Sat:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Val:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Red:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Green:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bl&amp;ue:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;lpha channel:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Basic colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Custom colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add to Custom Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Copy or Move a File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Read: %1</source>
        <translation>كتابة: %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Write: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File &amp;name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File &amp;type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>One directory up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size</source>
        <translation>الحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type</source>
        <translation>النوع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Date</source>
        <translation>التاريخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Look &amp;in:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Back</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create New Folder</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Detail View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview File Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview File Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Read-write</source>
        <translation>قراءة-كتابة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Read-only</source>
        <translation>قراءة فقط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Write-only</source>
        <translation>كتابة فقط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Inaccessible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Symlink to File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Symlink to Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Symlink to Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dir</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Special</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save As</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Open</source>
        <translation>&amp;فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Rename</source>
        <translation>&amp;اعادة تسمية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>R&amp;eload</source>
        <translation>&amp;اعادة تحميل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sort by &amp;Name</source>
        <translation>فرز &amp;بالاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sort by &amp;Size</source>
        <translation>فرز &amp;بالحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sort by &amp;Date</source>
        <translation>فرز &amp;بالتاريخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Unsorted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sort</source>
        <translation>فرز</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;hidden files</source>
        <translation>&amp;عرض الملفات المخفية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>the file</source>
        <translation>الملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>the directory</source>
        <translation>المسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>the symlink</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete %1</source>
        <translation>حذف %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Yes</source>
        <translation>&amp;نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;No</source>
        <translation>&amp;لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Folder 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Folder</source>
        <translation>مجلد جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Folder %1</source>
        <translation>مجلد جديد %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Find Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Directories</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save</source>
        <translation>حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error</source>
        <translation>خطأ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1
File not found.
Check path and filename.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select a Directory</source>
        <translation>تحديد مسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Directory:</source>
        <translation>المسار:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Font</source>
        <translation>&amp;خط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font st&amp;yle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size</source>
        <translation>&amp;الحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Effects</source>
        <translation>مؤثرات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Stri&amp;keout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Color</source>
        <translation>&amp;لون</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scr&amp;ipt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply</source>
        <translation>تطبيق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close</source>
        <translation>اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select Font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <location filename="" line="0"/>
        <source>Clear</source>
        <translation>مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select All</source>
        <translation>تحديد الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Undo</source>
        <translation>&amp;اعادة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Redo</source>
        <translation>&amp;تكرار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cu&amp;t</source>
        <translation>&amp;قص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <location filename="" line="0"/>
        <source>Line up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Customize...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <location filename="" line="0"/>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="" line="0"/>
        <source>Copy #%1 of </source>
        <translation>نسخ #%1 من </translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background</source>
        <translation>الخلفية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Importing text</source>
        <translation>استيراد نص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Supported Formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Afrikaans</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arabic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Albanian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Basque</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bulgarian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Breton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Catalan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Croatian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Czech</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Danish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dutch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>English</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>English (British)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Esperanto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Estonian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>German</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>German (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Finnish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>French</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Greek</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hungarian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Icelandic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Italian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Japanese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lithuanian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Luxembourgish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Norwegian (Bokmaal)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Norwegian (Nnyorsk)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Norwegian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portuguese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Russian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Swedish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spanish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Slovak</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Slovenian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Serbian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turkish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ukranian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Welsh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Crash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;OK</source>
        <translation>&amp;موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Master Page </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>4A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>2A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Comm10E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>DLE</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Could not open output file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output stream not writeable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Verification of settings failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Could not open input file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unable to read settings XML:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unable to read settings XML: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All</source>
        <translation>الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exporting PostScript File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Printing File</source>
        <translation>طباعة ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Black</source>
        <translation>اسود</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cyan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magenta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yellow</source>
        <translation>اصفر</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Freetype2 library not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 cannot be read, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Creating Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to load font %1 - font type unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Modified Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 loaded from %2(%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reading Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Writing updated Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Searching for Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>table &apos;%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Initializing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Development Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> mm</source>
        <translation>ملم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> in</source>
        <translation>بوصة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> cm</source>
        <translation>سم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>mm</source>
        <translation>ملم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>in</source>
        <translation>بوصة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>cm</source>
        <translation>سم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Points (pt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Inches (in)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Picas (p)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Centimeters (cm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cicero (c)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File exists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Replace</source>
        <translation>&amp;استبدال</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Barcode Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>My Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New From Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Template: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Newsletters</source>
        <translation>صحف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Brochures</source>
        <translation>منشورات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Catalogs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flyers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Signs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cards</source>
        <translation>كروت</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Letterheads</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Envelopes</source>
        <translation>مظاريف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Business Cards</source>
        <translation>كروت اعمال</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Calendars</source>
        <translation>تقاويم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Advertisements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Labels</source>
        <translation>ملصقات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Menus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Programs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Presentations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magazines</source>
        <translation>مجلات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Posters</source>
        <translation>جداريات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Announcements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Folds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Own Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export As Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as Image</source>
        <translation>حفظ كصورة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export successful.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File exists. Overwrite?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes</source>
        <translation>نعم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No</source>
        <translation>لا</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Yes all</source>
        <translation>نعم للكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PS/EPS Importer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save As Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Property not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Child not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Types matched, but setting property failed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;About Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Script</source>
        <translation>&amp;سكربت</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>About Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
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
        <location filename="" line="0"/>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>SVG Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Warning</source>
        <translation>تحذير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>SVG Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Old .sla format support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenOffice.org Draw Importer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Comma Separated Value Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CSV_data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CSV_header</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Word Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>HTML Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>html</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>
External Links
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenDocument Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OpenOffice.org Writer Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>English (American)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>English (Australian)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>German (Swiss)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <location filename="" line="0"/>
        <source>Clear</source>
        <translation>مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select All</source>
        <translation>تحديد الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Undo</source>
        <translation>&amp;اعادة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Redo</source>
        <translation>&amp;تكرار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cu&amp;t</source>
        <translation>&amp;قص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <location filename="" line="0"/>
        <source>System Menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unshade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normalize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Maximize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close</source>
        <translation>اغلاق</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Restore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Move</source>
        <translation>&amp;نقل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size</source>
        <translation>&amp;الحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mi&amp;nimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ma&amp;ximize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Stay on &amp;Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Restore Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Close</source>
        <translation>اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sh&amp;ade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>%1 - [%2]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Unshade</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Setup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Width:</source>
        <translation>&amp;العرض:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Height:</source>
        <translation>&amp;الارتفاع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Autosave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>min</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Interval:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color:</source>
        <translation>اللون:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Pictures</source>
        <translation>عرض الصور</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Top:</source>
        <translation>&amp;اعلى:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left:</source>
        <translation>&amp;يسار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;اسفل:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Right:</source>
        <translation>&amp;يمين:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal:</source>
        <translation>افقي:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical:</source>
        <translation>رأسي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Typography</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tools</source>
        <translation>ادوات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fonts</source>
        <translation>خطوط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color for paper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Adjusting Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Style Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Column 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name:</source>
        <translation>الاسم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>O&amp;K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>A&amp;pply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ca&amp;ncel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <location filename="" line="0"/>
        <source>Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lines</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <location filename="" line="0"/>
        <source>Style Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style of current paragraph</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <location filename="" line="0"/>
        <source>Fill Color Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of text fill</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of text fill</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <location filename="" line="0"/>
        <source>Stroke Color Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <location filename="" line="0"/>
        <source>Font Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font of selected text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Size</source>
        <translation>حجم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scaling width of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <location filename="" line="0"/>
        <source>Character Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exports SVG Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exports the current page into an SVG file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Import &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports SVG Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scalable Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <location filename="" line="0"/>
        <source>User settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>System wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save</source>
        <translation>&amp;حفظ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save user configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot write file %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>User settings saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot open file %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save a document as a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unknown</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <location filename="" line="0"/>
        <source>Top</source>
        <translation>اعلى</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right</source>
        <translation>يمين</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bottom</source>
        <translation>اسفل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left</source>
        <translation>يسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow Docking To...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal</source>
        <translation>افقي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical</source>
        <translation>رأسي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Floating Orientation...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <location filename="" line="0"/>
        <source>Printing...</source>
        <translation>طباعة...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Copy #%1 of </source>
        <translation>نسخ #%1 من </translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background</source>
        <translation>الخلفية</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <location filename="" line="0"/>
        <source>Document Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Background</source>
        <translation>الخلفية</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <location filename="" line="0"/>
        <source>Initializing Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reading Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Initializing Story Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reading Scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Setting up Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Searching for Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Exiting now.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fatal Error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font System Initialized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open &amp;Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Export</source>
        <translation>&amp;تصدير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>St&amp;yle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Color</source>
        <translation>&amp;لون</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size</source>
        <translation>&amp;الحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Font</source>
        <translation>&amp;خط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Effects</source>
        <translation>&amp;مؤثرات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;PDF Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;onvert To</source>
        <translation>&amp;تحويل إلى</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>I&amp;nsert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>E&amp;xtras</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Help</source>
        <translation>&amp;مساعدة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Alignment</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ready</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Importing Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Found nothing to import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Loading...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Supported Formats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save As</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saving...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Ignore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Printing...</source>
        <translation>طباعة...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Printing failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot Cut In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>About Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Manual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as</source>
        <translation>حفظ باسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name:</source>
        <translation>الاسم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Size:</source>
        <translation>&amp;الحجم:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size</source>
        <translation>الحجم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The following programs are missing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All</source>
        <translation>الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>-Page%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Lock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The program %1 is already running!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The program %1 is missing!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color Not Found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send to Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Previe&amp;w Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Liga&amp;ture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y-Pos:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to replace your existing image?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <location filename="" line="0"/>
        <source>Invalid argument: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File %1 does not exist, aborting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print help (this message) and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List the currently installed interface languages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not show the splashscreen on startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output version information and exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>filename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use filename as path for user given preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display a console window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>To override the default language choice:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Homepage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Issues</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy Here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move Here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Picture</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Original PPI: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Actual PPI: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>CMYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Linked Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text on a Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraphs: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Words: </source>
        <translation>الكلمات:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Chars: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Print: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Disabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>In&amp;fo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;PDF Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Le&amp;vel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page %1 to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cannot Convert In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <location filename="" line="0"/>
        <source>Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>has been changed since the last save.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Discard</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Embedded Python scripting support.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <location filename="" line="0"/>
        <source>Script error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Examine Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documentation for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <location filename="" line="0"/>
        <source>Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Startup Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Locate Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <location filename="" line="0"/>
        <source>Show Page Previews</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <location filename="" line="0"/>
        <source>Search/Replace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search for:</source>
        <translation>بحث عن:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraph Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font</source>
        <translation>الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Size</source>
        <translation>حجم الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font Effects</source>
        <translation>مؤثرات الخط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Stroke Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Stroke Shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left</source>
        <translation>يسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Center</source>
        <translation>توسيط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right</source>
        <translation>يمين</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Block</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace with:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Whole Word</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Ignore Case</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Search</source>
        <translation>&amp;بحث</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Replace</source>
        <translation>&amp;استبدال</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Close</source>
        <translation>&amp;اغلاق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search finished</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Search finished, found %1 matches</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <location filename="" line="0"/>
        <source>Select Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selected Fields</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <location filename="" line="0"/>
        <source>Other...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shade:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shade</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X-Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Y-Offset</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <location filename="" line="0"/>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <location filename="" line="0"/>
        <source>No Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit Styles...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <location filename="" line="0"/>
        <source>No Style</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <location filename="" line="0"/>
        <source>Edit Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Import</source>
        <translation>&amp;استيراد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;نسخ مطابق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy of %1</source>
        <translation>نسخ من %1</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>New Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <location filename="" line="0"/>
        <source>&amp;File</source>
        <translation>&amp;ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit</source>
        <translation>&amp;تحرير</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Settings</source>
        <translation>&amp;الخيارات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Story Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;New</source>
        <translation>&amp;جديد</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Clear All Text</source>
        <translation>حذف جميع النصوص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Reload Text from Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Save to File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Load from File...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Update Text Frame and Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cu&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Copy</source>
        <translation>&amp;نسخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Paste</source>
        <translation>&amp;لصق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;بحث/استبدال...</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Edit Styles...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Fonts Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Update Text Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Insert Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Background...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Display Font...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File</source>
        <translation>ملف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Current Paragraph:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Words: </source>
        <translation>الكلمات:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Chars: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Totals:</source>
        <translation>المجموع:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraphs: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Story Editor - %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you want to save your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to lose all your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save as</source>
        <translation>حفظ باسم</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <location filename="" line="0"/>
        <source>Auto</source>
        <translation>آلي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Linewidth</source>
        <translation>عرض الخط</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <location filename="" line="0"/>
        <source>More than one item selected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <location filename="" line="0"/>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Small Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Subscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Superscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not ask again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <location filename="" line="0"/>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>At the beginning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>At the end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Not Shown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Table of Contents %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Add</source>
        <translation>&amp;اضافة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <location filename="" line="0"/>
        <source>Ignore all errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic check before printing or exporting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for missing glyphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for objects not on a page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for overflow in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for transparencies used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for missing images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check image resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lowest allowed resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove Profile</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <location filename="" line="0"/>
        <source>Common Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Placing in Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>In the Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>In the Foreground</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Snapping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Snap Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> px</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color:</source>
        <translation>اللون:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Page Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Major Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minor Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guides are not visible through objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Guides are visible above all objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of the major grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color for the margin lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color for the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the basegrid on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance between the lines of the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance from the top of the page for the first baseline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the gridlines on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the guides on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Turns the margins on or off</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Manage Tabulators</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <location filename="" line="0"/>
        <source>Export Range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;All Pages</source>
        <translation>&amp;كل الصفحات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Rotation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Binding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left Margin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right Margin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Include La&amp;yers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image Compression Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lossy - JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lossless - Zip</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Compression Metho&amp;d:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Maximum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>High</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Low</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minimum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Compression &amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;General</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Available Fonts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fonts to embed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Effects</source>
        <translation>مؤثرات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>F&amp;rom the:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;irection:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> sec</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Blinds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dissolve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Glitter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Split</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Wipe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal</source>
        <translation>افقي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical</source>
        <translation>رأسي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Inside</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Outside</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Left to Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Top to Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bottom to Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right to Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>E&amp;xtras</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Page Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Double Page Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Visual Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use Full Screen Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display Bookmarks Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Display Layers Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Special Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Viewer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Passwords</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;User:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Owner:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;ecurity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>General</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Screen / Web</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Printer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Angle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Simple Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Round</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ellipse</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use ICC Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering-Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Images:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;olor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Info String:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Trim Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Type of the display effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export all pages to PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embed a color profile for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color profile for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rendering intent for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Embed All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fonts to outline:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Outline &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <location filename="" line="0"/>
        <source>Font:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> pt</source>
        <translation>نقطة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Stroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Stroke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tab Fill Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tab Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Line Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Shading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;hading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line &amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line S&amp;tyle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Arrows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Start:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>End:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>On Screen Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Full Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Normal Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Low Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Stepping:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Frame Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Picture Frame Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Drawing Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Font for new text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Size of font for new text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Gap between text frame columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sample of your font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Vertical scaling of images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill color of picture frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of fill</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line color of shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color of lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill color of shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line style of shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line width of shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color of lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Saturation of color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Style of lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Width of lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <location filename="" line="0"/>
        <source>Subscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Displacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Superscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>S&amp;caling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Auto</source>
        <translation>آلي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Strikethru</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Small Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic &amp;Line Spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <location filename="" line="0"/>
        <source>Left</source>
        <translation>يسار</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right</source>
        <translation>يمين</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Full Stop</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Comma</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Center</source>
        <translation>توسيط</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Fill Char:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Indentation for first line of the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Indentation from the left for the whole paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete all Tabulators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <location filename="" line="0"/>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Group </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Outline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Element</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <location filename="" line="0"/>
        <source>Auto</source>
        <translation>آلي</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Linewidth</source>
        <translation>عرض الخط</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <location filename="" line="0"/>
        <source>Add vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unlock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Resize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rotate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change Image Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change Image Scale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X: %1, Y: %2
X: %4, Y: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align/Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Items involved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cancel</source>
        <translation>الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Color1: %1, Color2: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flip horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Flip vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unlock</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Unlock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Disable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>From %1
to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Cut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set fill color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the style of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set the style of line join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Do not use custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set start arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set end arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Create table</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rows: %1, Cols: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font stroke color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font stroke color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set kerning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Align text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set font effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Image frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Polyline</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text on a Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Convert to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import SVG image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import EPS image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import OpenOffice.org Draw image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text flows around the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text flows around bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text flows around contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Set image scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Frame size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Free scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Keep aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Break aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Edit shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Rename layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send to layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Disable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Change name of the layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Get image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Multiple duplicate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset control point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Reset control points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Modify image effects</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <location filename="" line="0"/>
        <source>Initial State</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Action History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Show selected object only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Undo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Redo</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <location filename="" line="0"/>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UpgradeChecker</name>
    <message>
        <location filename="" line="0"/>
        <source>Attempting to get the Scribus version update file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Timed out when attempting to get update file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Error when attempting to get update file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>File not found on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>No updates are available for your version of Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>This list may contain development versions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Please visit www.scribus.net for details.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Minimum Margins for Page Size %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <location filename="" line="0"/>
        <source>Use Printer Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Select &amp;Printer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Top:</source>
        <translation>&amp;اعلى:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;اسفل:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Left:</source>
        <translation>&amp;يسار:</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;OK</source>
        <translation>&amp;موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Cancel</source>
        <translation>&amp;الغاء</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Insert value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enter a value then press OK.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Enter a value then press OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Send your value to the script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <location filename="" line="0"/>
        <source>Tools</source>
        <translation>ادوات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Properties...</source>
        <translation>الاعدادات...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <location filename="" line="0"/>
        <source>PDF Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Check Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert PDF Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Insert PDF Annotations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Text</source>
        <translation>النص</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Link</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Choose the importer to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Automatic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import Text Only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Import text without any formatting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Importer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Encoding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Open</source>
        <translation>فتح</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Choose the importer to use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remember association</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <location filename="" line="0"/>
        <source>New From Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Remove</source>
        <translation>&amp;ازالة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Open</source>
        <translation>&amp;فتح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>All</source>
        <translation>الكل</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colors</source>
        <translation>الالوان</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Usage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Created with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Date</source>
        <translation>التاريخ</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Author</source>
        <translation>المؤلف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Downloading Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Installing Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Preparing a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Removing a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Translating template.xml</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <location filename="" line="0"/>
        <source>Save as Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Name</source>
        <translation>الاسم</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Category</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Page Size</source>
        <translation>حجم الصفحة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Colors</source>
        <translation>الالوان</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Usage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Author</source>
        <translation>المؤلف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Email</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>More Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>OK</source>
        <translation>موافق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Less Details</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Legal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Tabloid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>landscape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>portrait</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>custom</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <location filename="" line="0"/>
        <source>Create filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>C&amp;lear</source>
        <translation>&amp;مسح</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>&amp;Delete</source>
        <translation>&amp;حذف</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Choose a previously saved filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Give a name to this filter for saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Give a name for saving</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <location filename="" line="0"/>
        <source>Disable or enable this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Add a new filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>and</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>do not remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>words</source>
        <translation>الكلمات</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Remove</source>
        <translation>ازالة</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Replace</source>
        <translation>استبدال</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Apply</source>
        <translation>تطبيق</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>Value at the left is a regular expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>with</source>
        <translation>مع</translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>all instances of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>all paragraphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>paragraphs starting with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>paragraphs with less than</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="" line="0"/>
        <source>paragraphs with more than</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
