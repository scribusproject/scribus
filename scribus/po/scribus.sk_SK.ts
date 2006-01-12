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
        <source>About Scribus %1</source>
        <translation>O Scribuse %1</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="obsolete">Používam GhostScript verziu %1</translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="obsolete">GS nie je dostupný</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="obsolete">Scribus verzia %1
%2 %3 (%4)</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;O programe</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Vývojársky tím:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Príspevky od:</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation>Mac OSX Aqua verzia:</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation>Windows verzia:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Oficiálna dokumentácia:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Ďalšia dokumentácia:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utori</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Oficiálne preklady a prekladatelia:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Predchádzajúci prispievatelia prekladov:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>Pre&amp;klady</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Domovská stránka</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Online referencie</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Chyby a požiadavky na vlastnosti</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>E-mailová konferencia</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;On-line</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Tento panel zobrazuje verziu, dátum vytvorenia a zakompilované podporu knižníc v Scribuse.
C-C-T-F vyjadruje podporu C=littlecms C=CUPS T=TIFF a F=Fontconfig. Posledné písmeno predstavuje renderer C=cairo alebo A=libart. Chýbajúca podpora knižnice je indikovaná znakom *. To tiež indikuje verziu Ghostscriptu, ktorú našiel Scribus.</translation>
    </message>
    <message>
        <source>%1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
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
        <translation>Áno</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nie</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: O moduloch</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation>Názov súboru:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Verzia:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Povolené:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Dátum vydania:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Autorské práva:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Autor(i):</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Popis:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licencia:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
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
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Otvoriť...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložiť &amp;ako...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Vrátiť sa k uloženému</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Zbie&amp;rať výstupy...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Získať text...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Pripojiť &amp;text...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Získať obrázok...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Uložiť &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Uložiť stranu ako &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Uložiť ako P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Na&amp;stavenie dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Tlačiť...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;Ukážka pred tlačou</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Koniec</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>S&amp;päť</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Opakovať</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Reži&amp;m úkonov položky</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vys&amp;trihnúť</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovať</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>V&amp;ymazať obsah</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Vybrať &amp;všetko</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>O&amp;dstrániť všetko</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hľadať/Nahradiť...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Upraviť obrázok...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Farby...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Štýly o&amp;dstavca...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Štý&amp;ly čiar...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>Ša&amp;blóny...</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaSkripty...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>Nas&amp;tavenia...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Iné...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Vľavo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>Do &amp;stredu</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>V&amp;pravo</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>Do &amp;bloku</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Vynútené</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Normálne</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Podčiarknuť</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Podčiarknuť s&amp;lová</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>Pre&amp;čiarknuť</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>Všetko k&amp;apitálkami</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapitálky</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Ho&amp;rný index</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;Dolný index</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Obrys</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Tieň</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>Obrázkové &amp;efekty</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulátory...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikovať</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Viacnásobné duplikovanie</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Zos&amp;kupiť</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Z&amp;rušiť zoskupenie</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Je &amp;zamknutý</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Veľkosť je &amp;zamknutá</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Premiestniť na &amp;spodok</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Pre&amp;miestniť na vrch</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Z&amp;nížiť</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Z&amp;výšiť</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Poslať do z&amp;ápisníka</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atribúty...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Viditeľný obrázok</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>Ak&amp;tualizovať obrázok</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Prispôsobiť rámec obrázku</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Rozšírené vlastnosti obrázka</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Nízke rozlíšenie</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>N&amp;ormálne rozlíšenie</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Vysoké rozlíšenie</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Je PDF &amp;záložkou</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Je PDF a&amp;notáciou</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>V&amp;lastnosti anotácie</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Vlas&amp;tnosti pola</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Upraviť tvar...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Pripojiť text k ceste</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>O&amp;dpojiť text z cesty</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Kombinovať mnohouholníky</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Ro&amp;zdeliť mnohouholníky</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bézierová krivka</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>R&amp;ámec obrázka</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Obrysy</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Mnohouholník</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textový rám</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Glyf...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Ukážkový text</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Vložiť...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;Import...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Odstrániť...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopírovať...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Presunúť...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>Použiť hl&amp;avnú stranu...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Konvertovať na hlavnú stranu...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Správa vo&amp;dítok...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Správa vlastností strany...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>Pr&amp;ispôsobiť oknu</translation>
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
        <translation>Minia&amp;túry</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Zobraziť o&amp;kraje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Zobraziť &amp;rámce</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Zobraziť &amp;obrázky</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Zobraziť m&amp;riežku</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Zobraziť &amp;vodítka</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Zo&amp;braziť pomocnú mriežku</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Zobraziť  &amp;textové reťazce</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Zobraziť riadiace znaky</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relatívne k strane</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Priti&amp;ahnuť k mriežke</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Pritiahnuť k vodítkam</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Vlastnosti</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Zápisník</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>V&amp;rstvy</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>Uspori&amp;adať strany</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>Zá&amp;ložky</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Meranie</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;História úkonov</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Predletová kontrola</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>Z&amp;arovnať a umiestniť</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Nástroje</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF nástroje</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Vybrať položku</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abuľka</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Čiara</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Čiara kreslená vo&amp;ľnou rukou</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Otočiť objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zväčšiť alebo zmenšiť</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zväčšiť</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zmenšiť</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Upraviť obsah rámca</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Upraviť text...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Prepojiť textové rámce</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Zrušiť prepojenie textových rámcov</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>Pip&amp;eta</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopírovať vlastnosti položky</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Upraviť text v Story editore</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Vložiť textový rámec</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Vložiť obrázková rámec</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vložiť tabuľku</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Vložiť tvar</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Vložiť mnohouholník</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Vložiť čiaru</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Vložiť bézierovú krivku</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Vložiť čiaru kreslenú rukou</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Správa obrázkov</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Deliť slová v texte</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Zrušiť delenie slov v texte</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Vytvoriť Obsah</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskáda</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Dláždenie</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;O Scribuse</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;O moduloch</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Informácie o &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Ti&amp;py pre nástroje</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;manuál...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Inteligentný spojovník</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Nedeliteľný spojovník</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Nedeliteľná  &amp;medzera</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Číslo stra&amp;ny</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Nový riadok</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Zalomenie rámca</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Zalomenie stĺpca</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Autorské práva</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registrovaná značka</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Obchodná značka</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Odrážka</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Em pomlčka</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En pomlčka</translation>
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
        <translation>Apostrof</translation>
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
        <translation>Prepnúť palety</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Prepnúť vodítka</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Zarovnať a umiestniť</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Zarovnať</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relatívne k:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Prvý výber</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Posledný výber</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Okraje</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Výber</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Zarovnať pravé strany objektov k ľavej strane ukotvenia</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Zarovnať ľavé strany k pravej strane ukotvenia</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Zarovnať dolné okraje</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Zarovnať pravé strany</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Zarovnať horné okraje objektov k dolnému okraju ukotvenia</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Centrovať na vertikálnej osi</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Zarovnať ľavé strany</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Centrovať na horizontálnej osi</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Zarovnať dolné okraje objektov k hornému okraju ukotvenia</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Zarovnať horné okraje</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Zvolené vodítka:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Umiestniť</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Vytvoriť rovnomerné vodorovné medzery medzi objektami</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Vytvoriť vodorovné medzery medzi objektami  v určenej veľkosti</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Umiestniť pravé strany v rovnakej vzdialenosti</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Umiestniť dolné strany v rovnakej vzdialenosti</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Umiestniť stredy v rovnakej vzdialenosti vodorovne</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Vytvoriť rovnomerné zvislé medzery medzi objektami</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Vytvoriť zvislé medzery medzi objektami v určenej veľkosti</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Umiestniť ľavé strany v rovnakej vzdialenosti</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Umiestniť stredy v rovnakej vzdialenosti zvislo</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Umiestniť horné strany v rovnakej vzdialenosti</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>Vz&amp;dialenosť:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Umiestniť položky v určenej vzdialenosti</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Nie je nič zvolené</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Niektoré objekty sú uzamknuté.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Odom&amp;knúť všetko</translation>
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
        <translation>Zarovnať text doľava</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Zarovnať text doprava</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Zarovnať text na stred</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Zarovnať text do bloku</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Zarovnať podľa okrajov</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Vlastnosti pola</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Tlačítko</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textové pole</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Políčko k zaškrtnutiu</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Výber</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Zoznam</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Názov:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Tipy pre nástroje:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Písmo použité s PDF-1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ohraničenie</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farba:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Šírka:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tenká</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálna</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Široká</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Štýl:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Bez prerušenia</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Prerušovaný</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčiarknuté</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Skosené</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Príloha</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Iné</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Iba na čítanie</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Požadované</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Neexportovať hodnotu</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Viditeľnosť:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Viditeľné</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skryté</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Bez tlače</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Bez náhľadu</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Vzhľad</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text pre tlačítko dole</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text pre pretočenie</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikony</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Použiť ikony</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstrániť</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Stlačené</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Pretočiť</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Umiestnenie ikon...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Zvýraznenie</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertovať</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Obkreslené</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Stlačiť</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Viacriadkový</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Heslo</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Obmedzenie</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Neposúvať</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Nekontrolovať pravopis</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Overiť štýl:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Kontrolovať</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kríž</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Kruh</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Hviezda</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Štvorec</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Štandardne je označené</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Upraviteľné</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>Java skript</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ísť na</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Odoslať formulár</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Vynulovať formulár</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importovať dáta</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Udalosť:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Pri rozmazaní</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Upraviť...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Odoslať na URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Odoslať údaje ako HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importovať dáta z:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cieľ</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Do súboru:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Zmeniť...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Strana:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-poz.:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-poz.:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Úkon</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Pole je naformátované ako:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Obyčajný</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Číslo</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Percento</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Čas</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formát čísla</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Desatinné:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Použiť symbol meny</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Symbol meny vpredu</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formátovanie</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formát percent</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formát dátumu</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formát času</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Vlastné skripty</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formát:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Stlačenie klávesy:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formát</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Hodnota nie je vyhodnotená</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Hodnota musí byť väčšia alebo rovná:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>a menšia alebo rovná:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Vlastný vyhodnocovací skript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Vyhodnotiť</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Hodnota nie je vypočítaná</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Hodnota je</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>súčet</translation>
    </message>
    <message>
        <source>product</source>
        <translation>súčin</translation>
    </message>
    <message>
        <source>average</source>
        <translation>priemer</translation>
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
        <translation>nasledujúcich polí:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Vybrať...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Vlastný počítací skript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Vypočítať</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Indikátor je v PDF-1.3 ignorovaný</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Sem vložte čiarkou oddelený zoznam polí</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Potrebujete najmenej ikonu pre Normal, aby ste mohli použiť ikony pre tlačítka</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Obrázky (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Príklad:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Zmena výberu</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF súbory (*.pdf);;Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Vlastnosti anotácie</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Externý odkaz</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Externý odkaz na web</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cieľ</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Strana:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Pos</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dokumenty (*.pdf);; Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Normálna</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Použiť šablónu</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Hlavná strana:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Použiť</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Aktuálnu s&amp;tranu</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Pá&amp;rne strany</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>N&amp;epárne strany</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Všetky str&amp;any</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;V rozsahu</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Použiť zvolenú šablónu na párne, nepárne alebo všetky strany v nasledujúcom rozsahu&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>to</source>
        <translation>na</translation>
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
        <source>Apply the selected template to even, odd or all pages within the following range</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Zápisník (*.scs);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Premenovať</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Odstrániť</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Názov &quot;%1&quot; nie je jedinečný.
Vyberte si, prosím, iný.</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nová položka</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Súbor</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Náhľad</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>Načí&amp;tať...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložiť &amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Malé</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Stredné</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Veľké</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Presunúť záložku</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Vložiť záložku</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Záložky</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Záložky</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Umiestnenie ikony</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Rozloženie:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Iba popisky</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Iba ikony </translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Popisky pod ikonami</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Popisky nad ikonami</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Popisky vpravo od ikon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Popisky vľavo od ikon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Popisky prekrývajú ikony</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Mierka:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Vždy</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Keď ikony sú príliš malé</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Keď ikony sú príliš veľké</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nikdy</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Ako zmeniť:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcionálne</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Neproporcionálne</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikona</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Vynulovať</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktivovať manažment farieb</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Systémové profily</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB Obrázky:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK Obrázky:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Plné farby (&amp;Solid colors):</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Tlačiareň:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Účel generovania</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Vnímateľné (Perceptual)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relatívna kolorimetria</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sýtosť</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolútna kolorimetria</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Tlač&amp;iareň:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulácia tlačiarne na obrazovke</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Označ netlačiteľné farby (&amp;gamut)</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>P&amp;oužiť kompenzáciu čierneho &amp;bodu</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Štandardný farebný profil pre importované CMYK obrázky</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Štandardný farebný profil pre importované RGB obrázky</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Štandardný farebný profil na plné farby na strane</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Farebný profil, ktorý máte vygenerovaný alebo dodaný od výrobcu  zariadenia.
Tento profil by mal byť nastavený na váš monitor a nie všeobecný profil (napr. s RGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Farebný profil pre váš model tlačiarne od výrobcu.
Tento profil by mal byť nastavený pre vašu tlačiareň a nie všeobecný profil (napr. s RGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Štandardný účel generovania pre váš monitor. Pokiaľ neviete, prečo ho meniť,
zvoľte relatívnu kolorimetrickú transformáciu alebo perceptuálnu (fotografickú) transformáciu.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Štandardný účel generovania pre vašu tlačiareň. Pokiaľ neviete prečo to meniť,
zvoľte relatívnu kolorimetrickú transformáciu alebo perceptuálnu (fotografickú) transformáciu.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Povoliť &apos;soft proofing&apos; pre tlač farieb vášho dokumentu,
založený na vybranom profile tlačiarne.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metóda zobrazovania farieb na obrazovke, ktoré nemusia byť správne vytlačené.
Toto vyžaduje veľmi presné profily a slúži iba ako varovanie.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Kompenzácia čierneho bodu je metóda zlepšovania kontrastu fotiek.
Odporúča sa, aby ste povolili túto možnosť, pokiaľ váš dokument obsahuje fotografie.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Upraviť farby</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farebný &amp;model</translation>
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
        <translation>Bezpečné RGB farby pre web</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Je bodová farba</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Je registračná farba</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nový</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Starý</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV- farebná mapa</translation>
    </message>
    <message>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>  %</translation>
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
        <translation>Plochy s dynamickými farbami</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Plochy so statickými farbami</translation>
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
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Nie je možné vytvoriť farbu s názvom &quot;%1&quot;.
Je rezervovaný pre transparentnú farbu</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Názov farby nie je jedinečný</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
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
        <translation type="obsolete">Ak stlačíte a bude podrží pravé tlačítko na myši,
uvidíte náhľady na obrázky.

Kláves Insert vloží glyf do výberu nižšie 
a kláves Delete odstráni posledný vložený</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Vybrať znak:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Písmo:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Trieda znaku:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vložiť</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Zmazať</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Vložiť znak na pozíciu kurzora</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Odstrániť označené.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Zatvorí tento dialóg a vráti sa do úprav textu.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Úplný súbor znakov</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Základná latinka</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 doplnky</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Rozšírenia-A pre latinku</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Rozšírenia-B pre latinku</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Všeobecná interpunkcia</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Horný a dolný index</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Symboly mien</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Písmenové symboly</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Formáty čísel</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Šípky</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematické operátory</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Kreslenie obdĺžnikov</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blokové prvky</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometrické tvary</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Rôzne symboly</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Varianty malých foriem</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligatúry</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Špeciálne</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Gréčtina</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Rozšírená gréčtina</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Cyrilika</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Doplnky k cyrilike</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabčina</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Rozšírenie A pre arabčinu</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Rozšírenie B pre arabčinu</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebrejčina</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Chýba glyf</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Pretečenie textu</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekt nie je na strane</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Chýbajúci obrázok</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Obrázok ma DPI-hodnotu nižšiu než %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objekt je priesvitný</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt je PDF anotácia alebo pole</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekt je umiestnené PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Neboli nájdené žiadne problémy</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Strana </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Voľné objekty</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Nájdené problémy</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Predletová kontrola</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Položky</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problémy</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Aktuálny profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorovať chyby</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Vybrať štýly</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Dostupné štýly</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrať priečinok</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Zbieram...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varovanie</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Nie je možné pozbierať všetky súbory pre výstup pre súbor:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monochromatický</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analogický</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementárny</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadický</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetradický (Dvojkomplementárny)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Základná farba</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monochromatická svetlá</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monochromatická tmavá</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>Prvá analogická</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Druhá analogická</translation>
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
        <translation>Vytvoriť &amp;farbu...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Importovanie existujúcich farieb...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Zlúčenie farieb</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>&amp;Nahradenie farieb</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Koniec</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farba</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Normálny zrak</translation>
    </message>
    <message>
        <source>Protanopia (red)</source>
        <translation type="obsolete">Protanopia (necitlivosť na červenú)</translation>
    </message>
    <message>
        <source>Deuteranopia (green)</source>
        <translation type="obsolete">Deuteranopia (necitlivosť na zelenú)</translation>
    </message>
    <message>
        <source>Tritanopia (blue)</source>
        <translation type="obsolete">Tritanopia (necitlivosť na modrú)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Celofarebná slepota</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Poškodenia zraku:</translation>
    </message>
    <message>
        <source>Saturation:</source>
        <translation type="obsolete">Sýtosť:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="obsolete">Hodnota:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Farebný kruh</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farba</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
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
        <translation>Vyberte metódu:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Uhol (0 -90 stupňov):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Splynúť farby</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Nahradiť farby</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Zlúčiť vytvorené farby do farieb dokumentu</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Nahradiť vytvorené farby farbami dokumentu</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Nechať farby nedotknutými</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Rozdiel medzi zvolenou hodnotou a vypočítanou. V dokumentácií je viac informácií.</translation>
    </message>
    <message>
        <source>Saturation component in HSV mode</source>
        <translation type="obsolete">Komponent sýtosti v režime HSV</translation>
    </message>
    <message>
        <source>Value component in HSV mode</source>
        <translation type="obsolete">Komponent hodnoty v režime HSV</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Kliknite na kruh a vyberte základnú farbu. Je to odtieň v režime HSV.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Ukážková farebná schéma</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Vyberte jednu z metód pre vytvorenie farebnej schémy. V dokumentácií je viac informácií.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Farby vami zvolenej farebnej schémy</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Tu sa simulujú bežné zrakové chyby. Vyberte si typ chyby.</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Zlúčenie farieb</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Chyba: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Farba %1 už existuje!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Farba %1 pridaná.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Teraz otváram správcu farieb.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Zlúčenie farieb</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nová farba</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Nie je možné nájsť požadovanú farbu. Pravdepodobne ste vybrali čiernu, šedú alebo bielu. Takúto farbu nie je možné spracovať.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
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
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Farebný kruh...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Pomocník s nastavením farieb</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Výber farieb spolu s teóriou farieb.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Použiť</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušiť</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varovanie</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
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
        <source>Shade:</source>
        <translation>Tieň:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Krytie:</translation>
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
        <translation>Presunúť vektor</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálna</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Horizontálny prechod</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Vertikálny prechod</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonálny prechod</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Krížovodiagonálny prechod</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radiálny prechod</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Voľný lineárny prechod</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Voľný radiálny prechod</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Upraviť vlastnosti farby čiary</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Upraviť vlastnosti farby výplne</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Sýtosť farby</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Bežná metóda vyplnenia alebo prechod</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Nastaviť priesvitnosť pre zvolenú farbu</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Stlačením ľavého tlačítka myši presuniete začiatok vektora prechodu a stlačením pravého tlačítka jeho koniec</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Nastavenia CSV importu</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Oddeľovač polí:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Oddeľovač hodnôt:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadny</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Prvý riadok je hlavička</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Možnosti tlačiarne</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Nastavenie strany</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Všetky strany</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Iba párne strany</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Iba nepárne strany</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Zrkadliť</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nie</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Áno</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientácia</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Up tlač</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Strana na list</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Strany na list</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Voľba</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Tento panel zobrazuje rôzne CUPS nastavená počas tlače.
Dostupnosť jednotlivých parametrov závisí na ovládači vašej tlačiarne.
Podpory CUPS zistíte v ponuke Pomocník &gt; O Scribuse.
Text: C-C-T znamená podporu C-CUPS, C-littlecms a T-Tiff.
Chýbajúce knižnice sú zobrazené ako *</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimovaný súbor</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Vrátanie písma</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kódovanie:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Presúva do vášho priečinku s dokumentmi.
V nastaveniach je toto možné určiť.</translation>
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
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished"></translation>
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
        <translation>Prerušené používateľom</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Odstrániť farbu</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Odstrániť farbu:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Nahradiť s:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Odstrániť strany</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Odstrániť od:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>po:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation type="unfinished">Nahradiť s:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Bez štýlu</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Informácie o dokumente</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Kľúčové slová:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Popis:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Vydavateľ:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Prispieva&amp;telia:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Dátum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;yp:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormát:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;fikátor:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Zdroj:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>V&amp;zťah:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Pokrytie:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>P&amp;ráva:</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Ďalšie &amp;informácie</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Osoba alebo organizácia primárne zodpovedná za vytvorenie obsahu dokumentu. Toto pole môže byť zabudované do Scribus dokumentu ako referencia, ako aj v metadátach PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Názov daný dokumentu. Toto pole môže byť zabudované do Scribus dokumentu ako referencia, ako aj v metadátach PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Správa o obsahu dokumentu. Toto pole obsahuje krátky popis alebo obsah dokumentu. Pri exporte je zabudované do PDF</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Téma dokumentu. Účelom tohoto pola je poskytnúť priestor pre kľúčové slová zabudované do PDF, ktoré asistujú pri vyhľadávaní a indexácií PDF súborov</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Osoba alebo organizácia zodpovedná za sprístupnenie dokumentu</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Osoba alebo organizácia zodpovedná za tvorbu príspevkov pre obsah dokumentu</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Dátum súvisiaci s udalosťou životného cyklu dokumentu vo formáte RRRR-MM-DD ako aj ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Charakter alebo žáner obsahu dokumentu, napr. kategórie, funkcie, žánre atď</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fyzický alebo digitálny prejav dokumentu. Typ média a rozmery sú vhodné zaznamenania. RFC2045,RFC2046 pre MIME typy je tiež vhodné tu uviesť</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Jednoznačný odkaz na dokument v danom kontexte ako je ISBN alebo URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Odkaz na dokument, z ktorého je aktuálny dokument odvodený, napr. ISBN alebo URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Jazyk, v ktorom je dokument napísaný - zvyčajne je to ISO-639 kód jazyka
prípadne je doplnený spojovníkom a ISO-3166 kódom krajiny napr. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Odkaz na príbuzný dokument, s použitím formálnej identifikácie ako je ISBN alebo URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Rozsah dokumentu, eventuálne vrátane umiestnenia, času a jurisdikcie</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informácie o právach držby dokumentu, napr. copyright, patent alebo obchodná značka</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1, 2, 3, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>i, ii, iii, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I, II, III, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>a, b, c, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A, B, C, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name: Optional name for section eg. Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Názov</translation>
    </message>
    <message>
        <source>From</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
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
        <source>Shown</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Vzťahuje sa k</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je rodičom</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je potomkom</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Textové rámce</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Obrázkové rámce</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Logická hodnota</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Celé číslo</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Reťazec</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="obsolete">Atribúty položiek dokumentu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Názov</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Vzťah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Vzťah k</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="obsolete">Automaticky pridať k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Pridať</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+P</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopírovať</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Odstrániť</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Zmazať</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+Z</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished">Atribúty položiek dokumentu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Názov</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished">Vzťah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished">Vzťah k</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="unfinished">Automaticky pridať k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopírovať</translation>
    </message>
    <message>
        <source>Alt+C</source>
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
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Zmazať</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Nastaviť tlačiareň</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Cieľ tlače</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Súbor</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>M&amp;ožnosti...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Súbor:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternatívny príkaz tlače</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Príkaz:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rozsah</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Tl&amp;ačiť všetko</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Tlačiť akt&amp;uálnu stranu</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Tlačiť &amp;rozsah</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>P&amp;očet kópií:</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Normálna tlač</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Oddelená tlač farebných zložiek</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Tlačiť farebne, ak je to možné</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Tlačiť v odtieňoch šedej</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všetky</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Azúrová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žltá</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Čierna</translation>
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
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Horizontálne zrkadlenie strán</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Vertikálne zrkadlenie strán</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Nastaviť veľkosť média</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farba</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Aplikovať  Under Color Removal</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished">Konvertovať bodové (spot) farby na procesné (process)</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Použiť ICC profily</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Pokročilé nastavenia</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Náhľad...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Tlač</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložiť čiarkou oddelený zoznam prvkov,
kde prvok môže byť * pre všetky strany,
1-5 pre rozsah strán alebo jedno číslo strany.</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Použiť alternatívneho správcu tlače ako je kprinter alebo gtklp,
na spracovanie prídavných nastavení tlače</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Nastaví PostScript Level.
Nastavenie úrovne 1 alebo 2 môže vytvoriť veľké súbory</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Spôsob prepínania niektorých šedých tieňov, ktoré sú zložené 
z azúrovej, fialovej a žltej farby namiesto použitia čiernej.
UCR naviac ovplyvňuje časti obrázkov, ktoré sú neutrálne a/alebo 
v tmavých tónoch, ktoré sú blízko šedej. Použitie tohoto postupu môže zlepšiť
tlač niektorých obrázkov - je však potrebné testovať jednotlivé prípady.
UCR znižuje možnosť presýtenosti s CMYK atramentami.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložiť ako</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Postscript súbory(*.ps);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Importovanie súboru:
%1
zlyhalo!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Upraviť štýl</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
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
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Vzdialenosti</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fixné riadkovanie</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatické riadkovanie</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnať text k pomocnej mriežke</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>Č&amp;iary:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Vzdialenosť textu:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulátory a odsadenie</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Náhľad na štýl odstavca</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Názov vášho štýlu odstavca</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Písmo označeného textu alebo objektu</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Veľkosť písma</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Farba výplne písma</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Farba obrysu písma</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Medzera nad odstavcom</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Medzere pod odstavcom</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Riadkovanie</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Prepína ukážkový text tohoto štýlu odstavca</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Názov štýlu nie je jedinečný</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Pozadie</translation>
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
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Otvoriť...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložiť &amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Uložiť a ukončiť</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>U&amp;končiť bez uloženia</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>V&amp;rátiť späť</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Opakovať vrátené</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vys&amp;trihnúť</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovať</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Zmazať</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Získať názvy polí</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Súbor</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>Java skripty (*.js);;Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Obrázkové efekty</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Možnosti:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farba:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tieň:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Jas:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Polomer:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Hodnota:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterizovať:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Dostupné efekty</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Rozmazanie</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Jas</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Vyfarbiť</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Odtiene šedej</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertovať</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterizovať</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Zaostriť</translation>
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
        <translation>Používané efekty</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Vybrať priečinok pre export</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportovať ako obrázok</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportovať do priečinka:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Typ obrázka:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalita:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Rozlíšenie:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
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
        <translation>Rozsah</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>A&amp;ktuálna strana</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Všetky str&amp;any</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Rozsah</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportovať rozsah strán</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložiť čiarkou oddelený zoznam prvkov,
kde prvok môže byť * pre všetky strany,
1-5 pre rozsah strán alebo jedno číslo strany.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportovať všetky strany</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportovať iba aktuálnu stranu</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Rozlíšenie obrázkov 
Použite 72 dpi pre obrázky, ktoré chcete zobrazovať</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Veľkosť obrázkov. 100% - bez zmien, 200% - pre dvojnásobnú veľkosť atď.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kvalita vašich obrázkov - 100% je najlepšia, 1% najhoršia</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Dostupné exportné formáty</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Výstupný priečinok - miesto pre odkladanie vašich obrázkov.
Názov exportovaného súboru bude &apos;názovdokumentu-číslostrany.typsúboru&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Zmeniť výstupný priečinok</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Rozšírené vlastnosti obrázkov</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálne</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Stmavené</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Zosvetlené</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Odtieň</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sýtosť</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farba</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Svetlosť</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Násobiť</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Obrazovka</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Rozpustenie</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Prekrytie</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Tvrdé svetlo</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Mäkké svetlo</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Rozdiel</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Vylúčenie</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farebný úskok</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farba</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Režim miešania:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Krytie:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadie</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Nepoužiť žiadnu cestu</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Veľkosť:</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Rozlíšenie:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznámy</translation>
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
        <translation>Odtiene šedej</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Farebný priestor:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titulok:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Bez titulku</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Farby</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikovať</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Odst&amp;rániť nepoužité</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Súbor farieb</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktuálna súbor farieb:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Uložiť farebnú &amp;súbor</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Nahrať súbor farieb</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Uloží aktuálny súbor farieb</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Odstrániť nepoužité farby z aktuálneho farebného súboru dokumentu</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importovať farby k aktuálnemu súboru z existujúceho dokumentu</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Vytvorí novú farbu v aktuálnom súbore</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Upraviť aktuálne zvolenú farbu</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Vytvoriť kópiu aktuálne zvolenej farby</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Odstrániť aktuálne zvolenú farbu</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Nastaviť aktuálny farebný súbor za štandardný</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Vybrať názov</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kópia %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nová farba</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished">Niektoré písma použité v tomto dokumente boli nahradené:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished">  bol nadhradený:</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Dostupné písma</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Substitúcia písem</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Dodatočné cesty</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Názov písma</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Použiť písmo</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Vložiť do:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Podmnožina</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Cesta k súborom písma</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Dostupné písm&amp;a</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Názov písma</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Náhrada</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Substitúcia písem</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Pridať...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Cestu, v ktorej budú hľadané písma, je možné určiť iba v Nastaveniach a iba vtedy ak nie je otvorený žiadny dokument. Zatvorte všetky otvorené dokumenty a potom použite Upraviť-&gt;Nastavenia a zmeňte cestu.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Dodatočné cesty</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrať priečinok</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Názov písma</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Podmnožina</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation>Prístup</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation>Veľkosť písma:</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation>Ukážka písma</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>Prid&amp;ať</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation>Rýchle hľadanie: </translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hľadať</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Pridať zvolené písmo do štýlu, ponuky písmo</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Zanechať náhľad</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation>Ak sem napíšete text, využijete rýchle hľadanie v názvoch písma. Napr. &apos;Bold&apos; zobrazí všetky písma, ktoré budú mať v názve Bold. Vyhľadávanie je citlivé na veľkosť písmen.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Spustenie hľadania</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Veľkosť zvoleného písma</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Príliš žltučký kôň spieval diabolské ódy</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Používateľ</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Systém</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Náhľad na písmo...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Dialóg náhľadu na písmo</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Triedenie, hľadanie a prezeranie dostupných písem.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Substitúcia písem</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Tento dokument obsahuje niektoré písma, ktoré nie sú inštalované na vašom systéme, prosím vyberte si namiesto nich náhradu. Tlačítko Zrušiť zastaví načítavanie dokumentu.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Pôvodné písmo</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Substitučné písmo</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Urobiť substitúciu permanentnou</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Zrušiť túto substitúciu písem a zastaviť načítavanie dokumentu.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Povolením tohto umožníte Scribusu používať permanetne tieto náhrady vo všetkých budúcich dokumentoch. Toto môžete zmenené v Upraviť &gt; Nastavenia... &gt; Písma.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Ak si zvolíte OK a potom uložiť, tieto substitúcie budú permanentné v dokumente.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Pozícia:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Tu môžete pridať, zmeniť alebo odstrániť Požadované farby (Stop-colors).</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Správa vodítok</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Vodorovné vodítka</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Pridať</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>O&amp;dstrániť</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Zvislé vodítka</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Pridať</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Riadky a stĺpce - Automatické vodítka</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Riadky:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Stĺpce:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>&amp;Odstup riadkov</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Odstup s&amp;tĺpcov</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>Odkazuje na:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Strana</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>Okra&amp;je</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Výber</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Uzamknúť vodítka</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>Ak&amp;tualizovať</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Nastaviť vodítka v dokumente. Správca vodítok je stále otvorený, ale zmeny sú trvalé</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished">Vodítka</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Náhľad</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Obsahy</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Obsahy</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hľadať</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Hľ&amp;adať</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Odst&amp;rániť všetko</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Tlačiť...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Koniec</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Súbor</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Hľadať...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>&amp;Nájsť ďalší</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Nájsť p&amp;redchádzajúci</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>Pridať zá&amp;ložku</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>Odst&amp;rániť všetko</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>Zá&amp;ložky</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus online pomocník</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Hľadanie je citlivé na veľkosť písmen</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Ľutujem, ale manuál nie je dostupný! Aktualizovanú dokumentáciu si môžete
stiahnuť z http://docs.scribus.net.</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>neznámy</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Hľadať</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Hľadaný výraz:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nová záložka</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Názov novej záložky:</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Možné delenie slov</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Akceptovať</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Preskočiť</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>N&amp;ávrhy pre delenie slov</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Automaticky &amp;deliť slová pri písaní</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Najmenšie slovo:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>M&amp;aximálny počet po sebe nasledujúcich delení:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Pri voľbe Extra -&gt; Delenie slov sa zobrazí dialóg, v ktorom budú zobrazené všetky možnosti delenia slova.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Povolí automatické delenie slov vášho textu počas písania.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Toto je dĺžka najmenšieho slova, ktoré sa bude deliť.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximálny počet delení slov za sebou.
Hodnota 0 znamená neobmedzené delenie slov.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Áno</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nie</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished">Popis:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished">Autorské práva:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Import &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importuje EPS súbory</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje väčšinu EPS súborov do aktuálneho dokumentu,
konvertuje ich vektorové dáta do Scribus objektov.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Vložiť stranu</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vložiť</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Strana(y)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>pred stranu</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranu</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na koniec</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Šablóny</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálne</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Hlavná strana:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntácia:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Presunúť objekty s ich stranami</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Vložiť tabuľku</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Počet riadkov:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Počet stĺpcov:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Upraviť Java skripty</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Upraviť...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Pridať...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nový skript:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nový skript</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Skutočne chcete zmazať tento skript?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nie</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Áno</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation type="unfinished"></translation>
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
        <translation>Úkon</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktuálny kláves</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Zvoľte kláves pre tento úkon</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>Žiad&amp;ny kláves</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Po&amp;užívateľom definovaný kláves</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Nastaviť &amp;kláves</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Načítateľný súbor klávesových skratiek</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>Načí&amp;tať</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importovať...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Exportovať...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Vynulovať</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Súr klávesových skratiek dostupný na načítanie</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Načítať zvolený súbor klávesových skratiek</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importovať súbor skratiek do aktuálnej konfigurácii</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Exportovať aktuálny súbor skratiek do importovateľného súboru</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Znovu načítať štandardné Scribus skratky</translation>
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
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Táto klávesová sekvencia sa už používa</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Odstrániť vrstvu</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Skutočne chcete odstrániť všetky objekty v tejto vrstve?</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Pridať novú vrstvu</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Odstrániť vrstvu</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Zvýšiť vrstvu</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Znížiť vrstvu</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Upraviť štýly čiary</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikovať</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kópia %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nový štýl</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Skutočne chcete zmazať tento štýl?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nie</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Áno</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished">Skutočne chcete zmazať tento štýl?</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Vybrať  Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Získať viac:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML súbory:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Odseky:</translation>
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
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Správa vlastností strany</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntácia:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Posunúť objekty v rámci strany</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodítka okrajov</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>Preset Layouts:</source>
        <translation>Predvolené rozloženia:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolný:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Horný:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>P&amp;ravý:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ľavý:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Vzdialenosť medzi horným vodítkom okraja a hranou strany</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Vzdialenosť medzi dolným vodítkom okraja a hranou strany</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdialenosť medzi ľavým vodítkom okraja a hranou strany.
Pri použití protiľahlých strán, táto medzera môže byť použitá na dosiahnutie správneho okraja pre väzbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdialenosť medzi pravým vodítkom okraja a hranou strany.
Pri použití protiľahlých strán, táto medzera môže byť použitá na dosiahnutie správneho okraja pre väzbu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Vnútorný:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>V&amp;onkajší:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Upraviť šablóny</translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation type="obsolete">Duplikovať zvolenú hlavnú stranu</translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="obsolete">Zmaže označenú hlavnú stranu</translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="obsolete">Pridá novú hlavnú stranu</translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="obsolete">Importuje hlavné strany z iného dokumentu</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Skutočne chcete zmazať túto šablónu?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nie</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Áno</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Názov:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Nová hlavná strana</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kópia %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Názov:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Nová hlavná strana</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kópia  #%1 z </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálna</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation type="unfinished"></translation>
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
        <translation>Viacnásobné duplikovanie</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Počet kópií:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vodorovné posunutie:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Zvislé posunutie:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Vzdialenosti</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>pt</translation>
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
        <translation>Uhol:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Dĺžka:</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Master Page</source>
        <translation>Importovať hlavnú stranu</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importovať stranu(y)</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Z dokumentu:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>Z&amp;meniť...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importovať stranu(y):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importovať hlavnú stranu</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vložiť čiarkou oddelený zoznam prvkov, kde prvok môže byť * pre všetky strany, 1-5 pre rozsah strán alebo jedno číslo strany.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> z 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Vytvoriť stranu(y)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Pred stranu</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Za stranu</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Na koniec</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);; Všetky súbory (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> z %1</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Chýbajúce písmo</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Písmo %1 nie je nainštalované.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Použiť</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>namiesto</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Presunúť strany</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopírovať stránku</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Presun strán</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">do:</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>pred stranu</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Za stranu</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Na koniec</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Presun strany:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fixné riadkovanie</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatické riadkovanie</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnať k pomocnej mriežke</translation>
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
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Vzdialenosť medzi stĺpcami</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Šírka stĺpca</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Bez štýlu</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Názov &quot;%1&quot; nie je jedinečný.
Vyberte si, prosím, iný.</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Obrázok</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Čiara</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Farby</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometria</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Otočenie:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Základný bod:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Úroveň</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Tvar:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Upraviť tvar...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Za&amp;obliť Rohy:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Vzdialenosť textu</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stĺpce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Horný:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolný:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ľavý:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>P&amp;ravý:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abulátory...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Vlastnosti cesty textu</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Zobraziť krivku</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Počiatočný posun:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Vzdialenosť od krivky:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Text o&amp;bteká okolo rámu</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Použiť &amp;viazané pole</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Po&amp;užiť vrstevnicu</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Š&amp;týl:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Voľ&amp;ná zmena veľkosti</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Aktuálne X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Aktuálne Y-DPI:</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-Mierk&amp;a:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-Mi&amp;erka:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Zmeniť veľ&amp;kosť na rozmer rámca</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roporcionálne</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Vložiť profil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Účel generovania:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Vnímateľné (Perceptual)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relatívna kolorimetria</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sýtosť</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolútna kolorimetria</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Ľavý bod</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Koncové body</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>Základný &amp;bod:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;yp čiary:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Začiatok šípky:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Koniec šípky:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Ší&amp;rka čiary:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Hrany:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Skosený spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Rovná hlavička</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Štvorcová hlavička</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>Ukonč&amp;enia:</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Čiary bunky</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Horná čiara</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Ľavá čiara</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Pravá čiara</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Dolná čiara</translation>
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
        <translation>Názov zvoleného objektu</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Vodorovná pozícia aktuálneho základného bodu</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Zvislá pozícia aktuálneho základného bodu</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Šírka</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Výška</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Otočenie objektu v aktuálnom základnom bode</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Bod, od ktorého sú odvodené vzdialenosti alebo uhly rotácie</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Zvoliť ľavý horný roh za základný bod</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Zvoliť pravý horný roh za základný bod</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Zvoliť ľavý dolný roh za základný bod</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Zvoliť pravý dolný roh za základný bod</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Zvoliť stred za základný bod</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Vodorovne zameniť</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Zvislo zameniť</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Presunúť o jednu úroveň vyššie</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Presunúť o jednu úroveň nižšie</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Presunúť dopredu</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Presunúť dozadu</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Ukazuje hladinu zvoleného objektu. 0 znamená, že objekt je na najnižšej úrovni</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Uzamknúť alebo odomknúť objekt</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Uzamknúť alebo odomknúť veľkosť objekt</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Povoliť alebo zakázať tlač objektu</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Povoliť obtekanie textu v nižších rámcoch okolo zvoleného objektu</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Použiť obklopujúce pole namiesto tvaru rámca pre tok textu</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Použiť druhú čiaru pôvodne založenú na tvare rámca pre tok textu</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Písmo pre zvolený text alebo objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Veľkosť písma</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Posun základnej čiary znakov</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zmena šírky znakov</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Zmena šírky znakov</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Farba obrysu písma</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Farba výplne písma</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Sýtosť farby obrysu písma</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Sýtosť farby výplne textu</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Písanie sprava doľava</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Riadkovanie</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Štýl aktuálneho odseku</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Jazyk rámca pre delenie slov</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Zmena nastavenia pre ľavé alebo koncové body</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Vzorka čiary</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Hrúbka čiary</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Typ spojenia čiar</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Zakončenie čiar</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Štýl čiary aktuálneho objektu</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Vyberte tvar rámca...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Upraviť tvar rámca...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Nastaví polomer zaoblenia rohov</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Počet stĺpcov v textovom rámci</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Prepína medzi medzerou alebo šírkou stĺpca</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Vzdialenosť textu od horného okraja rámca</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Vzdialenosť textu od dolného okraja rámca</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Vzdialenosť textu od ľavého okraja rámca</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Vzdialenosť textu od pravého okraja rámca</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Úprava nastavení tabulátorov textového rámca...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Povolí nastavenie iných rozmerov obrázku ako má rámec</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodorovný posun obrázku v rámci</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Zvislý posun obrázku v rámci</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Zmeniť obrázok vodorovne</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Zmeniť obrázok zvisle</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Použije sa rovnaká zmena veľkosti pre oba rozmery (X a Y)</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Zachovať pomer strán</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Obrázok zmení veľkosť podľa rozmeru rámca</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Použiť radšej pomer strán obrázka než rámca</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Zdrojový profil obrázka</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Účel generovania obrázka</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Upraviť štýl</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Rovná hlavička</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Štvorcová hlavička</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Skosený spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Šírka čiary:</translation>
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
        <translation>Plná čiara</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Čiarkovaná čiara</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Bodkovaná čiara</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Bodko-čiarkovaná čiara</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Čiara čiarka-bodka-bodka</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Názov &quot;%1&quot; nie je jedinečný.
Vyberte si, prosím, iný.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Zrušiť</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>&amp;Môj modul</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Moje moduly</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Modul funguje!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Nový dokument</translation>
    </message>
    <message>
        <source>Open Document</source>
        <translation type="obsolete">Otvoriť dokument</translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="obsolete">Nedávne dokumenty</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Už nezobraziť tento dialóg</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Veľkosť strany dokumentu, či už štandardná alebo vlastná</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientácia strán dokumentu</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Šírka strán dokumentu, upraviteľná, pokiaľ si zvolíte vlastnú veľkosť strany</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Výska strán dokumentu, upraviteľná, pokiaľ si zvolíte vlastnú veľkosť strany</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Prvé číslo strany dokumentu</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Úvodné číslo strany dokumentu</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Štandardná merná jednotka pre úpravu dokumentu</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Automaticky vytvorí textový rám pri pridaní novej strany</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Počet automaticky vytvorených stĺpcov pri vytvorení textového rámu</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Vzdialenosť medzi automaticky vytvorenými stĺpcami</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntácia:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodítka okrajov</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Č&amp;íslo prvej strany:</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>P&amp;očet strán:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Štan&amp;dardná jednotka:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Autom. textové rámy</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stĺpce:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenty (*.sla *sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenty (*.sla *.scd);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nový p&amp;odľa šablóny...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Uzly</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolútne súradnice</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Upraviť vrstevni&amp;cu</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Vynulovať v&amp;rstevnicu</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Koniec úprav</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Presunúť uzly</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Presunúť riadiace body</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Pridať uzly</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Odstrániť uzly</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Presúvať riadiace body nezávisle</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Presúvať riadiace body symetricky</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Vynulovať riadiace body</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Vynulovať tento riadiaci bod</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Otvorí polygón alebo oreže Bézierovú krivku</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Uzavrie túto Bézierovú krivku</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Horizontálne zrkadliť cestu</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Vertikálne zrkadliť cestu</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotácia cesty proti smeru hodinových ručičiek</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotácia cesty v smere hodinových ručičiek</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Zmenšiť veľkosť cesty o zobrazené %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Zväčšiť vekosť cesty o zobrazené %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Uhol otočenia</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% zväčšenia alebo zmenšenia</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivuje režim úprav vrstevnice</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Vrátiť vrstevnicu do pôvodného tvaru rámu</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Keď je označené, súradnice sú relatívne vzhľadom k strane, v opačnom prípade sú relatívne k objektu.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Import &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="obsolete">Importuje súbory OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="obsolete">Importuje väčšinu súborov OpenOffice Draw do aktuálneho dokumentu a konvertuje ich vektorové dáta na Scribus objekty.</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Nastavenia importu OpenDocumentu</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Prepísať štýl odstavca</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Pomocou tejto voľby prepíšete existujúce štýly v aktuálnom Scribus dokumente</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Zlúčiť štýly odstavcov</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Splynutie štýlov odstavcov podľa atribútov.Toto bude mať  za následok menej podobných štýlov odsekov, zachovanie atribútov štýlov aj v prípade, že pôvodné štýly sú rôzne pomenované.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použiť názov dokumentu ako prefix pre štýl odsekov</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Má pridať importér pridať názov dokumentu
na začiatok názvu štýlu odseku v Scribuse?</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Už sa znovu nepýtať</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Urobiť tieto nastavenia štandardnými a už sa nepýtať pri importe OASIS OpenDocumentu.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Zrušiť</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished">Scribus dokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
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
        <translation>Šírka čiary</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation type="unfinished">Uložiť ako PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished">Výst&amp;up do súboru:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished">Vy&amp;tvoriť pre každú stranu samostatný súbor</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Uložiť</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">Toto umožní exportovanie samostatne pomenovaného PDF súboru pre každú stranu dokumentu. Čísla strán sú pridávané automaticky. Táto vlastnosť je veľmi užitočná pri ukladaní PDF pre komerčnú tlač.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Uložiť ako</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished">PDF súbory (*.pdf);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Save as PDF</source>
        <translation type="obsolete">Uložiť ako PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">Výst&amp;up do súboru:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="obsolete">Vy&amp;tvoriť pre každú stranu samostatný súbor</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Uložiť</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">Toto umožní exportovanie samostatne pomenovaného PDF súboru pre každú stranu dokumentu. Čísla strán sú pridávané automaticky. Táto vlastnosť je veľmi užitočná pri ukladaní PDF pre komerčnú tlač.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Uložiť ako</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF súbory (*.pdf);;Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Ukážka pred tlačou</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Vyhladiť &amp;text</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Vyhladiť &amp;grafiku</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Zobraziť &amp;priesvitnosť</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Under Color Removal</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>Zo&amp;braziť CMYK</translation>
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
        <translation>Názov separácie</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Azúrová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žltá</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Čierna</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Zmena veľkosti:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Tlač...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Poskytuje krajší pohľad na textové položky v prehliadači, za čo zaplatíte
miernym spomalením prezerania. Toto sa týka iba Type 1 písma</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Poskytuje krajší pohľad na písma True Type, Open Type, EPS, PDF 
a vektorovej grafiky v náhľade, na úkor mierneho spomalenia prezerania</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Zobrazuje transparentnosť a transparentné položky vo vašom dokumente. Vyžaduje Ghostscript 7.07 alebo vyšší</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Vytvorí náhľad tlače simuláciou CMYK atramentov namiesto RGB farieb</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Spôsob prepínania niektorých šedých tieňov, ktoré sú zložené 
z azúrovej, fialovej a žltej farby namiesto použitia čiernej.
UCR naviac ovplyvňuje časti obrázkov, ktoré sú neutrálne a/alebo 
v tmavých tónoch, ktoré sú blízko šedej. Použitie tohoto postupu môže zlepšiť tlač niektorých obrázkov - je však potrebné testovať jednotlivé prípady. UCR znižuje možnosť presýtenosti s CMYK atramentami.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Zmena mierky strany.</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Povolí/zakáže C (azúrovú) zložku</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Povolí/zakáže M (fialovú) zložku</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Povolí/zakáže Y (žltú) zložku</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Povolí/zakáže K (čiernu) zložku</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všetky</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Zavrieť</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Obrázok</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Čiara</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnohouholník</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomená čiara</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Text na cestu</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kópia z </translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Vzťahuje sa k </translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je rodičom</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je potomkom</translation>
    </message>
    <message>
        <source>Page Item Attributes</source>
        <translation type="obsolete">Atribúty položiek strany</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Názov</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Vzťah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Vzťah k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">Prid&amp;ať</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopírovať</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">O&amp;dstrániť</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Zmazať</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+Z</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušiť</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation type="unfinished">Atribúty položiek strany</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Názov</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished">Vzťah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished">Vzťah k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopírovať</translation>
    </message>
    <message>
        <source>Alt+C</source>
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
        <source>C&amp;lear</source>
        <translation type="unfinished">&amp;Zmazať</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Zrušiť</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Rozloženie strany</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Prvá strana je:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation type="unfinished">Dvojstranné</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished">V strede vpravo</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished">Strany alebo šablóny zmažete potiahnutím do odpadkového koša</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished">Tu sú všetky vaše šablóny; na vytvorenie novej strany potiahnite šablónu do náhľadu strany</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="unfinished">Usporiadať strany</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished">Dostupné šablóny:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="unfinished">Strany dokumentu:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %1</source>
        <translation>%1 z %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 - %2</translation>
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
        <translation>Letter</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
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
        <translation>Stredná</translation>
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
        <translation>Výsledok</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Hľadať výsledok pre: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Náhľad</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Výber</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Manage Pictures</source>
        <translation>Správa obrázkov</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Cesta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Tlač</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Stav</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Áno</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Chýbajúci</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Hľadať</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Zrušiť hľadanie</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ísť na</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - vyhľadávanie obrázkov</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Vyhľadávanie zlyhalo: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Obrázok s názvom &quot;%1&quot; nebol nájdený.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Uložiť ako &amp;obrázok...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportovať ako obrázok</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exportovať označené strany ako bitmapové obrázky.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Nemôžem nájsť modul</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>neznáma chyba</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Nie je možné nájsť symbol (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Modul: načítavanie %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>inicializácia zlyhala</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>neznámy typ modulu</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 načítaný</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 sa nepodarilo načítať: %2</translation>
    </message>
    <message>
        <source>PostScript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">PostScript súbory (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG obrázky (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG obrázky (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Správca modulov</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Modul</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Ako spustiť</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Načítať?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Modul ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Súbor</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Áno</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nie</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Musíte reštartovať aplikáciu, aby sa zmeny prejavili.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Vlastnosti mnohouholníkov</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>R&amp;ohy:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Otočenie:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Použiť &amp;faktor</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Počet uhlov pre mnohouholník</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Stupne otočenia pre mnohouholníky</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Použiť konvexný/konkávny faktor na zmenu tvaru mnohouholníkov</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Ukážka mnohouholníka</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Záporná hodnota nastaví mnohouholník ako konkávny (alebo v tvare hviezdy),
a kladná na konvexný</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Nastavenia</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Téma:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">V&amp;eľkosť písma:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Krok kolieska:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nedávne dokumenty:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenty:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profily:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skripty:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Z&amp;meniť...</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Šablóny dokum&amp;entov:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Zmeniť...</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Všeobecné</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntácia:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Jednotky:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodítka okrajov</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatické ukladanie</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Povolené</translation>
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
        <translation>Späť/Opakovať</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Dĺžka histórie úkonov</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Delenie slov</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Predletová kontrola</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Správa farieb</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Atribúty položiek dokumentu</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsahy a Indexy</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Klávesové skratky</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Ostatné nastavenia</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Pri zmene u&amp;ložiť obsah</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Náhľad</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Malý</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Stredný</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Veľký</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Zobrazenie strany</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farba:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zobraziť &amp;netlačiteľnú oblasť vo farbe okrajov</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Zobraziť obrázky</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Zobraziť textové reťazce</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Zobraziť textové riadiace znaky</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Zobraziť rámce</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relatívne k strane</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Hore:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vľavo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dole:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>Vp&amp;ravo::</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Medzery medzi stranami</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodorovne:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Zvisle:</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>P&amp;rispôsobiť veľkosť zobrazenia</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Obrazovku si prispôsobíte pomocou jazdca na dolnom pravítku.</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Zobrazenie</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Interpret postscriptu</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Príkaz pre spustenie programu:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Vyhladiť &amp;text</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Vyhladiť &amp;grafiku</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Rozlíšenie:</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Nástroj na úpravu obrázkov</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>P&amp;ríkaz pre spustenie programu:</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externé nástroje</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Tlač</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Pripnúť k okrajo&amp;m strany</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Aplikovať  &amp;Under Color Removal</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Vždy sa spýtať, či majú byť písma nahradené pri otváraní dokumentu</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Pri úprave štýlov robiť viditeľný náhľad na aktuálny štýl odseku</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Zobraziť štartovací dialóg</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Vždy použiť štandardný Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Počet odsekov:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Rôzne</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Moduly</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Povoliť alebo zakázať zobrazenie prepojených textových rámcov.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobraziť netlačiteľné znaky ako je označenie odstavca v textových rámcoch</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Zapnutie alebo vypnutie zobrazenia rámcov</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Zapnutie alebo vypnutie zobrazenia obrázkov</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Vyberte si štandardný jazyky, s ktorým bude Scribus spustený. Pokiaľ položku necháte prázdnu použije sa premenná prostredia. Vždy ju budete môcť potlačiť pomocou voľby v príkazovom riadku pri spustení Scribusu</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Vyberte štandardnú dekoráciu okien a vzhľadu.
Scribus dedí dostupné KDE a QT témy</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Štandardná veľkosť písma pre ponuky a okná</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Štandardná merná jednotka pre úpravu dokumentu</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Počet riadkov, ktoré posunie Scribus pri každom pohybe kolieska na myši</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Počet nedávno upravovaných dokumentov, ktoré sa zobrazia v ponuke Súbor</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Štandardný priečinok pre dokumenty</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="obsolete">Štandardný priečinok s ICC profily- Táto voľba
nemôže byť zmenená pri otvorenom dokumente.</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Štandardný priečinok Scriptera pre skripty</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Ďalší priečinok pre šablóny dokumentov</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Štandardná veľkosť papiera, či už vlastné alebo štandardné rozmery</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Štandardná orientácia strán dokumentu</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Šírka strán dokumentu, upraviteľná, ak ste si zvolili vlastnú veľkosť strán</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Výška strán dokumentu, upraviteľná, ak ste si zvolili vlastnú veľkosť strán</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Keď je povolené, Scribus uloží záložnú kópiu s príponou .bak 
vždy, keď vyprší časová lehota</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Časový interval medzi automatickými uloženiami</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Nastaví dĺžku histórie úkonov v krokoch.
Ak použijete 0, uloží sa nekonečné množstvo činností.</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Vyberte veľkosť náhľadu v zápisníku</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Uložiť obsah zápisníku pri každej zmene</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farba pre papier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplniť plochu za hranicami strany farbou okrajov</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Nastavenie štandardnej úrovne zväčšenia</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Umiestnite pravítko oproti vašej obrazovke a potiahnite jazdca na nastavenie úrovne zväčšenia, tak aby Scribus zobrazoval vaše strany a objekty v správnej veľkosti</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Umiestnenie Ghostscript interpreta v súborovom systéme</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Vyhladenie textu pre EPS a PDF pri generovaní na obrazovku</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Vyhladenie grafiky pre EPS a PDF pri generovaní na obrazovku</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="obsolete">Umiestnenie grafického editora. Ak používate gimp 
a vaša distribúcia ho obsahuje, odporúčame &apos;gimp-remote&apos;,
ktorý vám umožní upraviť obrázok v už spustenej inštancii gimpu.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Neukazovať objekty mimo hraníc tlačiteľnej strany alebo exportovaného súboru</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Spôsob prepínania niektorých šedých tieňov, ktoré sú zložené 
z azúrovej, fialovej a žltej farby namiesto použitia čiernej.
UCR naviac ovplyvňuje časti obrázkov, ktoré sú neutrálne a/alebo 
v tmavých tónoch, ktoré sú blízko šedej. Použitie tohoto postupu môže zlepšiť
tlač niektorých obrázkov - je však potrebné testovať jednotlivé prípady.
UCR znižuje možnosť presýtenosti s CMYK atramentami.</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definuje množstvo priestoru vľavo od plátna dokumentu, ktoré je dostupné ako lepenka na vytváranie a úpravu elementov a ich potiahnutie na aktívnu stranu</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definuje množstvo priestoru vpravo od plátna dokumentu, ktoré je dostupné ako lepenka na vytváranie a úpravu elementov a ich potiahnutie na aktívnu stranu</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definuje množstvo priestoru nad plátnom dokumentu, ktoré je dostupné ako lepenka na vytváranie a úpravu elementov a ich potiahnutie na aktívnu stranu</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definuje množstvo priestoru pod plátnom dokumentu, ktoré je dostupné ako lepenka na vytváranie a úpravu elementov a ich potiahnutie na aktívnu stranu</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrať priečinok</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Lokalizovať Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Lokalizovať váš editor obrázkov</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Spôsob prepínania niektorých šedých tieňov, ktoré sú zložené 
z azúrovej, fialovej a žltej farby namiesto použitia čiernej.
UCR naviac ovplyvňuje časti obrázkov, ktoré sú neutrálne a/alebo 
v tmavých tónoch, ktoré sú blízko šedej. Použitie tohoto postupu môže zlepšiť tlač niektorých obrázkov - je však potrebné testovať jednotlivé prípady. UCR znižuje možnosť presýtenosti s CMYK atramentami.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>Štan&amp;dardy</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Single Page</source>
        <translation>Jednoduchá strana</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation>Dvojstranné</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Ľavá strana</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Pravá strana</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3 prehyby</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Stred</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4 prehyby</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>V strede vľavo</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>V strede vpravo</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Migrovať staré nastavenia Scribusu?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus našiel nastavenia pre Scribus 1.2.
Chcete ich migrovať na novú verziu?</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Nie je možné otvoriť súbor s nastaveniami &quot;%1&quot; pre zápis: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Zápis do súbora s nastaveniami &quot;%1&quot; zlyhalo: QIODevice stavový kód %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Nepodarilo sa otvoriť súbor nastavení  &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Nepodarilo sa prečítať XML nastavenia &quot;%1&quot;: %2 na riadku %3, stĺpec %4</translation>
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
        <source>Error Writing Preferences</source>
        <translation>Chýba zápisu nastavení</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus nebol schopný uložiť svoje nastavenia: &lt;br&gt;%1&lt;br&gt;Prosím skontrolujte práva súbora a priečinku a dostupný priestor na disku.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Chyba načítavania nastavení</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus nebol schopný načítať svoje nastavenia:&lt;br&gt;%1&lt;br&gt; Načítajú sa štandardné nastavenia.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Book</source>
        <translation type="obsolete">Kniha</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Časopis</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, &apos;Book&apos; sets margins classically (Gutenberg). &apos;Book&apos; is proposed for two-sided documents. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Tu si môžete vybrať preddefinované rozloženie strany. &apos;Žiadne&apos; nechá okraje, tak ako sú, &apos;Kniha&apos; nastaví okraje klasicky (Gutenberg). &apos;Kniha&apos; je určená pre obojestranné dokumenty. &apos;Časopis&apos; nastaví všetky okraje na rovnakú hodnotu. Rozhodujúca je ľavá vnútorná hodnota.</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Otvoriť...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložiť &amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Koniec</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Súbor</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Spustiť</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Spustiť ako &amp;konzolu</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Uložiť výstup...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus Python konzola</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Je odvodené od štandardnej Python konzoly a tak obsahuje niektoré obmedzenia zvlášť v prípade medzier. V Scribus  manuály nájdete viac informácií.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Konzola skriptu</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Tu napíšte vaše príkazy.Výber je spracovaný ako skript</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Výstup vášho skriptu</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python skripty (*.py)</translation>
    </message>
    <message>
        <source>Open File With Python Commands</source>
        <translation type="obsolete">Otvorí súbor s Python príkazmi</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Uloží Python príkazy ako súbor</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varovanie</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Textové súbory (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Uloží aktuálny výstup</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Od&amp;tieň:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sýtosť:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Hodnota:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>Če&amp;rvená:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>Ze&amp;lená:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Mo&amp;drá:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lfa kanál:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Základné farby</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Vlastné farby</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definovať vlastné farby&gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>Pridať k &amp;vlastným farbám</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Výber farby</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopírovať alebo presunúť súbor</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Čítať: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Zapísať: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Názov súboru:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Typ súboru:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>O priečinok vyššie</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Veľkosť</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atribúty</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Pozrieť &amp;do:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Dozadu</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Vytvoriť nový priečinok</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Pohľad zoznam</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Podrobný pohľad</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Informácie o súbore</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Ukázať obsah súboru</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Čítanie-Zápis</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Iba pre čítanie</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Iba zápis</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Nedostupné</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symbolický odkaz na súbor</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symbolický odkaz na priečinok</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symbolický odkaz na špeciálne</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Súbor</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Priečinok</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Špeciálne</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Uložiť ako</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Otvoriť</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložiť</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Premenovať</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>N&amp;ačítať znovu</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Zoradiť podľa &amp;mena</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Zoradiť podľa &amp;veľkosti</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Zoradiť podľa &amp;dátumu</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nezoradené</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Zoradiť</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Zobraziť &amp;skryté súbory</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>súbor</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>priečinok</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>symbolický odkaz</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Odstrániť %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ste si istý, že chcete odstrániť %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Áno</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nie</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nový priečinok 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nový priečinok</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nový priečinok %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Nájsť priečinok</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Priečinky</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Uložiť</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Chyba</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Súbor nenájdený.
Overte, cestu a názov súboru.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Všetky súbory (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Vyberte priečinok</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Priečinok:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Písmo</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Š&amp;týl písma</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Veľkosť</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Prečiar&amp;knuť</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Podčiarknuť</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farba</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Vzorka</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Skr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Použiť</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavrieť</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Vybrať písmo</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Vymazať</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vybrať všetko</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>V&amp;rátiť späť</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Opakovať vrátené</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vys&amp;trihnúť</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovať</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Zoradiť</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Prispôsobiť...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;O Qt&lt;/h3&gt;&lt;p&gt;Tento program používa Qt verziu %1.&lt;/p&gt;&lt;p&gt;Qt je C++ knižnica pre multiplatformový vývoj GUI &amp;amp; aplikácii.&lt;/p&gt;&lt;p&gt;Poskytuje portabilitu jednotného kódu na MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, a všetky hlavné komerčné varianty Unixu.&lt;br&gt;Qt je dostupné aj pre vstavané zariadenia.&lt;/p&gt;&lt;p&gt;Qt je produkt firmy TrollTech. Ďalšie informácie nájdete na &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kópia  #%1 z </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadie</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importovanie textu</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Všetky podporované formáty</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikánsky</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albánsky</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskitsky</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulharský</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brazílsky</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalánsky</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Čínsky</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Katalánsky</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Český</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dánsky</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandský</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Anglický</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Anglický (Britský)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Nemecký</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Fínsky</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francúzsky</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Halíčsky</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grécky</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Maďarský</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonézsky</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Taliansky</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Kórejsky</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litovský</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Nórsky (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Nórsky (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Nórsky</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Poľský</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugalský</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugalský (BR)</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruský</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Švédsky</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Španielsky</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Španielsky (Latinská Amerika)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovenský</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovinský</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Srbsky</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turecký</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrajinský</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Welský</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Pád Scribusu</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus spadol kvôli signálu #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation> Šablóna </translation>
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
        <translation>Nie je možné otvoriť výstupný súbor %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Výstupný prúd nie je možné zapísať</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Verifikácia nastavení zlyhala: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Nie je možné otvoriť vstupný súbor %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Nie je možné načítať nastavenia XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (riadok %2 stĺpec %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Nie je možné načítať nastavenia XML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; neplatné</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>nájdených %1 &lt;%2&gt; uzlov, potrebujem 1.</translation>
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
        <source>All</source>
        <translation>Všetky</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Čierna</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Azúrová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žltá</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Knižnica Freetype2 nie je dostupná</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Písmo %1 je poškodené, nebude vložené</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Písmo %1 je poškodené (prúd čítania), nebude vložené</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Písmo %1 je poškodené (FreeType2), odkladám ho</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Písmo %1 je poškodené, odkladám ho</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Písmo %1 má poškodený glyf %2 (kód znaku %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Písmo %1 je poškodené, odkladám ho</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Písmo %1 nie je možné prečítať, nebude vložené</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Písmo %1 je poškodené, odkladám ho</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Nepodarilo sa načítať písmo %1 - neznáme písmo</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Písmo %1 načítané z %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Písmo %1(%2) je duplikát %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Načítavanie písma %1 (nájdené s použitím fontconfig)</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation>Písmo %1 je poškodené (nájdené s použitím fontconfig), odkladám ho</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Neporadilo sa načítať písmo - freetype2, nie je možné nájsť súbor písma</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Písmo %1 je poškodené (FreeType), odkladám ho</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>  Písmo %1 má neplatný glyf %2 (kód znaku %3), odkladám ho</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>memcpy hlavička: %°1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabuľka &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>memcpy tabuľka: %°1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy posun: %°1 %2 %3</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Inicializácia...</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribus vývojárska verzia</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Spustili ste vývojársku verziu Scribusu 1.3.x. Aktuálny dokument, na ktorom pracujete bol vytvorený v Scribuse 1.2.2 alebo nižšom. Ak tento súbor uložíte, už s ním nebudete môcť pracovať v Scribuse 1.2.2, iba ak by ste použili Súbor-&gt; Uložiť ako. Ste si istý, že chcete vykonať túto operáciu?</translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="obsolete">&amp;Pokračovať</translation>
    </message>
    <message>
        <source>Invalid argument: </source>
        <translation type="obsolete"> Neplatný argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation type="obsolete">Súbor %1 neexistuje, končím.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="obsolete">Použitie: scribus [možnosti ...] [súbor]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="obsolete">Možnosti:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="obsolete">Vytlačiť pomocníka (táto správa) a skončiť</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language</source>
        <translation type="obsolete">Použije xx ako skratku pre jazyk</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation type="obsolete">Zoznam aktuálne nainštalovaných jazykových rozhraní</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="obsolete">Zobraziť informácie na konzole, keď budú načítané písma</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation type="obsolete">Nezobraziť úvodnú obrazovku pri spúšťaní</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation type="obsolete">Vytlačiť informácie o verzií a skončiť</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="obsolete">Nainštalované jazykové rozhrania pre Scribus sú tieto:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation type="obsolete">Prepísať štandardnú voľbu jazyka:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="obsolete">scribus -l xx alebo scribus --lang xx, kde xx je zvolený jazyk.</translation>
    </message>
    <message>
        <source>Scribus Version </source>
        <translation type="obsolete">Scribus verzia </translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="obsolete">Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="obsolete">---------------------------------------</translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="obsolete">Domovská stránka:       http://www.scribus.net </translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="obsolete">Dokumentácia:  http://docs.scribus.net</translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="obsolete">Wiki:           http://wiki.scribus.net</translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="obsolete">Chyby:         http://bugs.scribus.net</translation>
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
        <translation> mm</translation>
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
        <translation>Body (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Milimetre (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Palce (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Piká (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimetre (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Súbor existuje</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="obsolete">Súbor s názvom &apos;%1&apos; už existuje.
Chcete ho nahradiť so súborom, ktorý ukladáte?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Nahradiť</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>strana</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Farebný kruh</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Náhľad na písmo</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Môj modul</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nový podľa šablóny</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation> Šablóna dokumentu:</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Informačné letáky</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brožúry</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalógy</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Reklamné letáky</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Karty</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Hlavičkové papiere</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Obálky</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Vizitky</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendáre</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Reklamy</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Popisy</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Ponuky</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programy</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF formuláre</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF prezentácie</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Časopisy</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plagáty</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Oznámenia</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Textové dokumenty</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Zložky</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Obaly médií</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Vlastné šablóny</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportovať ako obrázok</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Uložiť ako obrázok</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Chyba pri zapisovaní výstupného súboru.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Export bol úspešný.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Súbor existuje. Prepísať?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>už existuje. Prepísať?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nie</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Áno</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Áno všetko</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS Importér</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Všetky podporované formáty (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Uložiť ako šablónu</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať farbu bez názvu.</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Farba nebola nájdená.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nie je možné zmeniť farbu bez názvu.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Farba nebol nájdená v dokumente.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Farba nebol nájdená medzi štandardnými farbami.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nie je možné vytvoriť farbu bez názvu.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nie je možné odstrániť farbu bez názvu.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nie je možné nahradiť farbu bez názvu.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Nepodarilo sa otvoriť dokument.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Nepodarilo sa uložiť dokument.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Jednotka je mimo rozsahu. Použiť jednu z scribus.UNIT_* konštánt.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Farba nebol nájdená - python chyba</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argument musí byť názov položky strany, alebo PyCObject inštancia</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Vlastnosť nebola nájdená</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Potomok nebol nájdený</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Nie je možné konvertovať výsledný typ &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Vlastnosť &apos;%1&apos; nie je podporovaná</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Nie je možné konvertovať  &apos;%1&apos; na vlasnosť typu &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Typy sú zhodné, ale nastavnie vlastnosti sa nepodarilo.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Cieľ nie je obrázkový rámec.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Určená položka nie je obrázkový rámec.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Nie je možné zoskupiť menej ako dve položky</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Nie je možné zoskupiť menej ako dve položky</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Potrebný je výber alebo zoznam položiek na zoskupenie</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Nie je možné zmeniť mierku o 0%.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Písmo nebolo nájdené.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Nie je možné vygenerovať prázdnu vzorku.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Nie je možné uložiť pixmapu</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Nie je možné mať vrstvu bez názvu.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Vrstva nebola nájdená.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Nie je možné odstrániť poslednú vrstvu.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Nie je možné vytvoriť vrstvu bez názvu.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Objekt s požadovaným názvom už existuje.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Bodový zoznam musí obsahovať najmenej dva body (štyri hodnoty).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Bodový zoznam musí obsahovať párny počet hodnôt.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Bodový zoznam musí obsahovať najmenej tri body (šesť hodnôt).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Bodový zoznam musí obsahovať najmenej štyri body (osem hodnôt).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Bodový zoznam musí byť násobok šiestich hodnôt.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekt nebol nájdený.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Štýl nebol nájdený.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné použiť štýl na netextový rámec.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Nepodarilo sa uložiť EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Počet strán je mimo rozsah.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument nie je zoznam: musí byť zoznamom plávajúcich hodnôt.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nenumerické hodnoty: musí to byť zoznam plávajúcich hodnôt.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nenumerické hodnoty: musí to byť zoznam plávajúcich hodnôt.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Šírka čiary je mimo hraníc, musí byť 0 &lt;= šírka_čiary &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Tieň čiary je mimo hraníc, musí byť 0 &lt;= tieň &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Tieň výplne čiary je mimo hraníc, musí byť 0 &lt;= tieň &lt;= 100.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Polomer rohu musí byť kladné číslo.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Štýl čiary nebol nájdený.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať veľkosť písma netextového rámec.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať písmo netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať veľkosť textu netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať počet stĺpcov netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať riadkovanie z netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť medzeru medzi stĺpcami v netextovom rámca.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné získať text netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť text netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné vložiť text do netextového rámca.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Vkladanie indexu mimo hraníc.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Zarovnanie je mimo rozsahu. Použite jednu zo scribus.ALIGN* konštánt.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné použiť zarovnanie textu na netextový rámec.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Veľkosť písma je mimo hraníc - musí byť 1 &lt;= veľkosť &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť veľkosť písma pre netextový rámec.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť písmo na netextový rámec.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Riadkovanie je mimo hraníc, musí byť &gt;=0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť riadkovanie na netextový rámec.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Vzdialenosť medzi stĺpcami je mimo hraníc, musí byť kladná.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť vzdialenosť medzi stĺpcami v netextovom rámci.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Počet stĺpcov je mimo hraníc, musí byť &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť počet stĺpcov v netextovom rámci.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Index výberu je mimo hraníc</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Nie je možné označiť text v netextovom rámci</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné odstrániť text z netextového rámca.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť vyplnenie textom na netextový rámec.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť štýl textu na netextový rámec.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť tieňovanie textu na netextový rámec.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Prepojiť je možné len textové rámce.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Cieľový rámec musí byť prázdny.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Cieľový rámec je prepojený na iný rámec.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Cieľový rámec je prepojený k inému rámcu.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Zdroj a cieľ je rovnaký objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nie je možné odstrániť prepojenie z netextového rámca.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt nie je pripojený textový rámec - nie je možné ho odpojiť.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objekt je posledný rámec v sérií, nie je možné ho odpojiť. Odpojte namiesto neho predchádzajúci rámec.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Nie je možné konvertovať netextový rámec na obrysy.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Iba textové rámce môžu byť skontrolovanie na pretekanie</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Nie je možné nastaviť záložku na netextový rámec</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Nie je možné získať info z netextového rámca</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Názov súboru musí byť reťazec.</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Názov súboru nesmie byť prázdny reťazec.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Nie je možné zmazať nastavenia typu obrázok.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Typ obrázka musí byť reťazec.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>&apos;allTypes&apos; atribút je iba na ČÍTANIE</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Nepodarilo sa exportovať obrázok</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Spustiť skript...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Zobraziť &amp;konzolu</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;O skripte...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus skripty</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Posledné skripty</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>O skripte</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skripter</translation>
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
        <translation> Vlastné (voliteľné) nastavenia:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation> Štandardné nastavenia:</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Krátke slová</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Spracovanie krátkych slov. Čakajte prosím...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Spracovanie krátkych slov. Ukončené.</translation>
    </message>
    <message>
        <source>Short Words Manual</source>
        <translation type="obsolete">Krátke slová manuálne</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG Export</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložiť ako</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG obrázky (*.svg *.svgz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG obrázky (*.svg);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Skutočne chcete prepísať súbor:
%1 ?</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVG Import</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Importér pre OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Súbory s hodnotami oddeľovanými čiarkami (CSV)</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_dáta</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_hlavička</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML súbory</translation>
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
Externé odkazy
</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Textové dokumenty OpenDocument</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Dokumenty OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Textové filtre</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Textové súbory</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Hľadanie písem</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
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
        <translation>Vymazať</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vybrať všetko</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>V&amp;rátiť späť</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Opakovať vrátené</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vys&amp;trihnúť</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovať</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Systémová ponuka</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tieň</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Zrušiť tieň</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizovať</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimalizovať</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximalizovať</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavrieť</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Obnoviť</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Presunúť</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Veľkosť</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimalizovať</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximalizovať</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavrieť</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Nechať navr&amp;chu</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimalizovať</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavrieť</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>S&amp;kryť</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Zobraziť</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Nastavenie dokumentu</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntácia:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šírku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šír&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Jednotky:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodítka okrajov</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Možnosti</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="obsolete">Použiť nastavenia veľkosti na všetky strany</translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="obsolete">Použiť nastavenia okrajov na všetky strany</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Č&amp;íslo prvej strany:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatické ukladanie</translation>
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
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Informácie o dokumente</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Zobraziť stranu</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farba:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zobraziť &amp;netlačiteľnú oblasť vo farbe okrajov</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Zobraziť obrázky</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Zobraziť textové reťaze</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Zobraziť riadiace znaky</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Zobraziť rámce</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relatívne k strane</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Hore:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ľavý:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolný:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>P&amp;ravý:</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Medzery medzi stranami</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodorovné:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Zvislé:</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Zobraziť</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Delenie slov</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Predletová kontrola</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Atribúty položiek dokumentu</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsahy a Indexy</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Správa farieb</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Povoliť alebo zakázať zobrazenie prepojených textových rámcov.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobraziť netlačiteľné znaky ako je  označenie odstavca v textových rámcoch</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Zapnutie alebo vypnutie zobrazenia rámcov</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Zapnutie alebo vypnutie zobrazenia obrázkov</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farba pre papier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplniť plochu za hranicami strany farbou okrajov</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Prispôsobenie farieb</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python skripty (*.py);; Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Spustiť ako rozširujúci skript</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Nastavenie štýlu</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Štýl aktuálneho odseku</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Nastavenia farby výplne</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Farba výplne textu</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Sýtosť farby výplne textu</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Nastavenia farby obrysu</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Farba obrysu písma</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Sýtosť farby obrysu písma</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Nastavenie písma</translation>
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
        <translation>Písmo označeného textu</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Veľkosť písma</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zmena šírky znakov</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Zmena šírky znakov</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Nastavenia znakov</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Uložiť stranu ako &amp;SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Exportovať SVG súbory</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Exportovať aktuálnu stranu do SVG súboru.</translation>
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
        <translation>Import SVG súborov</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje väčšinu SVG súborov do aktuálneho dokumentu.
konvertuje ich vektorové dáta na Scribus objekty.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Krátke slová</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Použiť nezalomiteľnú medzeru na:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Zvolené rámce</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Aktívna st&amp;rana</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Všetky str&amp;any</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Spracovať iba označené rámce.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Spracovať iba aktuálnu stranu.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Všetky položky dokumentu boli spracované.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Uložiť</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Vynulovať</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Krátke slová</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Uložiť ako ša&amp;blónu...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Nie je možné nájsť cieľový súbor %1: %2</translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Nie je možné otvoriť cieľový súbor %1: %2</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Trvalý</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Import</translation>
    </message>
    <message>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Export</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Úkon</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Neznámy</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation> Kópia  #%1 z </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadie</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Žiadne</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Inicializácia zásuvných modulov</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Správa klávesových skratiek</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Načítavanie nastavení</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="obsolete">Inicializácia Story Editora</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">Čítanie ICC profilov</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="obsolete">Inicializácia delenia slov</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Čítanie zápisníka</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Nastavovanie klávesových skratiek</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Súbor</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Upraviť</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Hľadanie písem</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">Vo vašom systéme nie sú dostupné písma.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Končím.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Kritická chyba</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Systém písem bol inicializovaný</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Súbor</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">Otvoriť n&amp;edávne</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Import</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Export</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Upraviť</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">Š&amp;týl</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Farba</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Veľkosť</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Tieň</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">&amp;Písmo</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Efekty</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">Po&amp;ložka</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="obsolete">Nastavenie náhľadu</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Úroveň</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Poslať do vrstv&amp;y</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Nastavenia &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Tvar</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="obsolete">Konve&amp;rzia na</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="obsolete">&amp;Vložiť</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="obsolete">Úvodzovka</translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="obsolete">Medzera</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Strana</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Zobraziť</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">&amp;Okná</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Pomocník</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Zarovnanie</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normálne</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Pripravený</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenty (*.sla *sla.gz *.scd *scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenty (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Otvoriť</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Importovanie strán...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Importovať stranu</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Pokúšate sa importovať viac strán, než sú dostupné v aktuálnom dokumente počítajúc od aktívnej strany.&lt;/p&gt;Vyberte si nasledovné:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Vytvoriť&lt;/b&gt; chýbajúce strany&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importovať&lt;/b&gt; strany až po poslednú stranu&lt;/li&gt;&lt;li&gt;&lt;b&gt;Zrušiť&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Vytvoriť</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importovať</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Import je dokončený</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Nebolo nájdené nič na importovanie</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="obsolete">Súbor %1 nie je v akceptovateľnom formáte</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Načítavam...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="obsolete">Niektoré písma použité v tomto dokumente boli nahradené:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="obsolete">  bol nadhradený:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="obsolete">Niektoré ICC profily použité v tomto dokumente neboli nainštalované:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="obsolete">(konvertované)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Všetky podporované formáty</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="obsolete"> Nie je možné zapísať súbor: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenty (*.sla *.sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumenty (*.sla *.scd);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Uložiť ako</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Ukladám...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
        <translation type="obsolete">Scribus našiel niekoľko chýb.
Použite predletovú kontrolu na ich opravu</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="obsolete">&amp;Zrušiť</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="obsolete">&amp;Ignorovať</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Tlačím...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Tlač sa nepodarila!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="obsolete">Nie je možné vystrihnúť používanú položku</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="obsolete">Položka %1 práve upravovaná v Story Editore. Operácia vystrihnutie bola zrušená</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">O Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Scribus manuál</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Uložiť ako</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Textové súbory (*.txt);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Názov:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="obsolete">Konvertovať na šablónu</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="obsolete">Nová šablóna</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Veľkosť</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Tieň:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Tieň</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="obsolete">Tieto programy chýbajú:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="obsolete">Ghostscript : Nemôžete používať EPS obrázky alebo tlačiť náhľad</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="obsolete">Scribus našiel nejaké chyby.
Skúste použiť Predletovú kontrolu na ich opravu.</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Všetky</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS obrázky (*.eps);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Boli nájdené nejaké chyby.
Skúste použiť Predletovú kontrolu na ich opravu</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="obsolete">-Strana%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Niektoré objekty sú uzamknuté.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="obsolete">&amp;Uzamknúť všetko</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">&amp;Odomknúť všetko</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="obsolete">Chorvátsky</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Nemecký</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Poľský</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Anglický</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Španielsky</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Taliansky</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Francúzsky</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Ruský</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Dánsky</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slovenský</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Maďarský</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Český</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Holandský</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugalský</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Portugalský (BR)</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrajinský</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Grécky</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalánsky</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Fínsky</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Írsky</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Litovský</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Švédsky</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovinský</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikánsky</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulharský</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Informácie</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="obsolete">Program %1 je už spustený!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="obsolete">Tieto program %1 chýba!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="obsolete">Označené farby neexistujú v súbore farieb dokumentu. Vložte prosím názov pre novú farbu.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="obsolete">Farba nebola nájdená</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="obsolete">Názov, ktorý ste si zvolili už existuje. Vložte prosím iný názov pre túto novú farbu.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="obsolete">&amp;Úroveň</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="obsolete">Poslať do vrstvy</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="obsolete">Nas&amp;tavenia náhľadu</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Nástroje</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-poz.:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-poz.:</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation>Vzorka</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farba</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation>Farby dokumentu</translation>
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
        <translation>Pozadie</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nová vrstva</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normálna</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Inicializácia zásuvných modulov</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished">Správa klávesových skratiek</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="unfinished">Načítavanie nastavení</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="unfinished">Inicializácia Story Editora</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished">Čítanie ICC profilov</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished">Inicializácia delenia slov</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="unfinished">Čítanie zápisníka</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="unfinished">Nastavovanie klávesových skratiek</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Súbor</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished">Upraviť</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Hľadanie písem</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished">Vo vašom systéme nie sú dostupné písma.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="unfinished">Končím.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Kritická chyba</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished">Systém písem bol inicializovaný</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Súbor</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="unfinished">Otvoriť n&amp;edávne</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Import</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="unfinished">&amp;Export</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Upraviť</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="unfinished">Š&amp;týl</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Farba</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Veľkosť</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="unfinished">&amp;Tieň</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Písmo</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="unfinished">&amp;Efekty</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="unfinished">Po&amp;ložka</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished">Nastavenie náhľadu</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">Úroveň</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Poslať do vrstv&amp;y</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">Nastavenia &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Tvar</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="unfinished">Konve&amp;rzia na</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="unfinished">&amp;Vložiť</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished">Úvodzovka</translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="unfinished">Medzera</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Strana</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="unfinished">&amp;Zobraziť</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="unfinished">&amp;Okná</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">&amp;Pomocník</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="unfinished">&amp;Zarovnanie</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="unfinished">Pripravený</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Otvoriť</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="unfinished">Importovanie strán...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="unfinished">Import je dokončený</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="unfinished">Nebolo nájdené nič na importovanie</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished">Súbor %1 nie je v akceptovateľnom formáte</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="unfinished">Načítavam...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished">Niektoré ICC profily použité v tomto dokumente neboli nainštalované:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished">  bol nadhradený:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished">(konvertované)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">Všetky podporované formáty</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"> Nie je možné zapísať súbor: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenty (*.sla *.sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Uložiť ako</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="unfinished">Ukladám...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="unfinished">&amp;Ignorovať</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="unfinished">&amp;Zrušiť</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="unfinished">Tlačím...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="unfinished">Tlač sa nepodarila!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="unfinished">Nie je možné vystrihnúť používanú položku</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="unfinished">Položka %1 práve upravovaná v Story Editore. Operácia vystrihnutie bola zrušená</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="unfinished">O Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="unfinished">Scribus manuál</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Uložiť ako</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Textové súbory (*.txt);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Názov:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished">Konvertovať na šablónu</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Veľkosť:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Veľkosť</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished">&amp;Tieň:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Tieň</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Bez štýlu</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="unfinished">Tieto programy chýbajú:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished">Ghostscript : Nemôžete používať EPS obrázky alebo tlačiť náhľad</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Všetky</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished">Scribus našiel nejaké chyby.
Skúste použiť Predletovú kontrolu na ich opravu.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished">EPS obrázky (*.eps);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished">Boli nájdené nejaké chyby.
Skúste použiť Predletovú kontrolu na ich opravu</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="unfinished">-Strana%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished">Niektoré objekty sú uzamknuté.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="unfinished">&amp;Uzamknúť všetko</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Informácie</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="unfinished">Program %1 je už spustený!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="unfinished">Tieto program %1 chýba!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="unfinished">Označené farby neexistujú v súbore farieb dokumentu. Vložte prosím názov pre novú farbu.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="unfinished">Farba nebola nájdená</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="unfinished">Názov, ktorý ste si zvolili už existuje. Vložte prosím iný názov pre túto novú farbu.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="unfinished">&amp;Úroveň</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="unfinished">Poslať do vrstvy</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="unfinished">Nas&amp;tavenia náhľadu</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="unfinished">&amp;Nástroje</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="unfinished">X-poz.:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="unfinished">Y-poz.:</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation type="unfinished"> Neplatný argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation type="unfinished">Súbor %1 neexistuje, končím.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="unfinished">Použitie: scribus [možnosti ...] [súbor]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="unfinished">Možnosti:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="unfinished">Vytlačiť pomocníka (táto správa) a skončiť</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation type="unfinished">Zoznam aktuálne nainštalovaných jazykových rozhraní</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="unfinished">Zobraziť informácie na konzole, keď budú načítané písma</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation type="unfinished">Nezobraziť úvodnú obrazovku pri spúšťaní</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation type="unfinished">Vytlačiť informácie o verzií a skončiť</translation>
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
        <translation type="unfinished">Nainštalované jazykové rozhrania pre Scribus sú tieto:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation type="unfinished">Prepísať štandardnú voľbu jazyka:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="unfinished">scribus -l xx alebo scribus --lang xx, kde xx je zvolený jazyk.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished">Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation type="unfinished">Domovská stránka</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished">Wiki</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Vrstva</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopírovať sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Sem presunúť</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Obrázok</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Súbor: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation> Pôvodné PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation> Aktuálne PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Prepojený text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Textový rámec</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text na cestu</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation> Odseky:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation> Slová:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation> Znaky:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation> Tlačiť: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Povolené</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Zakázané</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Nastavenie náhľadu</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Nastavenia &amp;PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Poslať do vrstv&amp;y</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Úro&amp;veň</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Konve&amp;rzia na</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>O&amp;dstrániť</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Prepojiť textové rámce</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Pokúšate sa prepojiť vyplnené rámce, alebo rámec na samého seba.</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Skutočne chcete odstrániť celý text?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Nie je možné zmazať používanú položku</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Položka %1 práve upravovaná v Story Editore. Operácia odstránenie bola zrušená</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Strana %1 až %2 </translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Nie je možné konvertovať používanú položku</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Položka %1 práve upravovaná v Story Editore. Operácia konvertovanie na obrysy pre túto položku bude preskočená</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Neznámy</translation>
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
        <translation type="unfinished">Odtiene šedej</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">Zaho&amp;diť</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">&amp;Zavrieť</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>bol od posledného uloženia zmenený.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Uložiť</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Vstavaná podpora pre skriptovanie v Pythone.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skripter</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Chyba skriptu</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Ak ste spustili oficiálny skript, tak nahláste toto na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Táto správa je tiež vo vašej schránke (clipboard). Použite Ctrl+V a vložte ju do stopovača chýb (bug tracker).</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation> Došlo k internej chybe, keď ste sa pokúsili vložiť príkaz. Detaily budú zobrazené na stderr.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Otestovať skript</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python skripty (*.py)</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation> Nastavenie Python modulu zlyhalo. Detaily chyby budú zobrazené v stderr.</translation>
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
        <translation>Nastavenie Scriptera</translation>
    </message>
    <message>
        <source>Extension Scripts</source>
        <translation type="obsolete">Rozširujúce Skripty</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Povoliť rozširujúce skripty</translation>
    </message>
    <message>
        <source>Startup Script</source>
        <translation type="obsolete">Skript pri spustení</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Zobraziť náhľady strany</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Master Page Names</source>
        <translation type="obsolete">Zobraziť názvy šablón</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Nájsť/Nahradiť</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Hľadať:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Štýl odstavca</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Písmo</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Veľkosť písma</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efekty písma</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Farba výplne</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Tieň výplne</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Farba obrysu</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Tieň obrysu</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vľavo</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Na stred</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Do bloku</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Vynútené</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Nahradiť s:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Celé slovo</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorovať veľkosť písma</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hľadať</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>Nah&amp;radiť</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>N&amp;ahradiť všetko</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Zmazať</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Za&amp;vrieť</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Vyhľadávanie ukončené</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="obsolete">Strany alebo šablóny zmažete potiahnutím do odpadkového koša</translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="obsolete">Náhľady na všetky strany vášho dokumentu</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="obsolete">Tu sú všetky vaše šablóny; na vytvorenie novej strany potiahnite šablónu do náhľadu strany</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normálna</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Usporiadať strany</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="obsolete">Dostupné šablóny:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Strany dokumentu:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Dvojstrany</translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="obsolete">Prvá strana vľavo</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Zvoľte polia</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Dostupné polia</translation>
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
        <translation>Zvolené polia</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Iné...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Tieň:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tieň</translation>
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
        <translation>X-posun</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-posun</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Krátke s&amp;lová...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Krátke slová</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words.</source>
        <translation type="obsolete">Špeciálny modul pre pridávanie nezalomiteľných medzier pred a za krátke slová.</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Bez štýlu</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Upraviť štýly...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Bez štýlu</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Upraviť štýly</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikovať</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kópia %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nový štýl</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="obsolete">Skutočne chcete zmazať tento štýl?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nie</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Áno</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *sla.gz *.scd *scd.gz);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všetky súbory (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Znova načítať text z rámca</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Uložiť do súboru...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Načítať zo súboru...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Uložiť &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Aktualizovať textový rámec a ukončiť</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Ukončiť b&amp;ez aktualizácie textového rámca</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Vybrať &amp;všetko</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vys&amp;trihnúť</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovať</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>V&amp;ložiť</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Zmazať</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hľadať/Nahradiť...</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Vložiť glyf...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Upraviť štýly...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Ná&amp;hľad na písmo...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Aktualizovať textový rámec</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Pozadie...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>Písm&amp;o zobrazenia...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Inteligentný výber textu</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Súbor</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Upraviť</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Nas&amp;tavenia</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Story Editor</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Súbor</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Odstrániť celý text</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Načítať text zo súboru</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Uložiť text do súboru</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Aktualizovať textový rámec a ukončiť</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Ukončiť bez aktualizácie textového rámca</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Znovu načítať text z rámca</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Aktualizovať textový rámec</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Hľadať/Nahradiť</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktuálny odsek:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Slová: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaky: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Celkom:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odseky: </translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Story Editor - %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varovanie</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Chcete uložiť zmeny?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Skutočne chcete stratiť všetky zmeny?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Skutočne chcete odstrániť celý text?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otvoriť</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové súbory (*.txt);;Všetky súbory (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložiť ako</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Posunutie</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Šírka čiary</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Podčiarknuté</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation>Podčiarknuté iba slová</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Všetko kapitálky</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Malé kapitálky</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Dolný index</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Horný index</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Prečiarknuté</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Obrys</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>Tieň</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Nastavenia importu OpenOffice.org Writer dokumentu</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Prepísať štýl odstavca</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Povolením tohoto prepíšete existujúce štýly v aktuálnom Scribus dokumente</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Splynutie štýlov odstavca</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Splynutie štýlov odstavca podľa atribútov. Toto bude mať za následok zmenšenie počtu podobných štýlov, hoci v pôvodnom dokumente boli štýly pomenované rôzne.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použiť názov dokumentu ako prefix pre štýl odsekov</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Pripojiť názov dokumentu pred názov štýlu odseku v Scribuse.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Už sa znovu nepýtať</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Nech sú tieto nastavenia štandardnými a už sa viac nepýtať pri importe OpenOffice.org 1.x dokumentu.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Zrušiť</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Na začiatku</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Na konci</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Nezobrazené</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsahy a Indexy</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Obsahy</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>Prid&amp;ať</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>O&amp;dstrániť</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rámec tabuľky obsahu bude umiestnený do</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Umiestnenie čísla strany:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Názov atribútu položky:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Atribút položky bude nastavený na použitý rámec ako základ pre vytváranie záznamov</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Umiestniť čísla strán záznamov na začiatok alebo koniec riadku, alebo ich neumiestniť vôbec</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Zoznam netlačiteľných položiek</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Vrátane rámcov, ktoré sú označené ako netlačiť</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Štýl odstavca použitý pre riadok záznamu</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Štýl odstavca:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Cieľový rámec:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">Obsahy a Indexy</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished">Obsahy</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Rámec tabuľky obsahu bude umiestnený do</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished">Umiestnenie čísla strany:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished">Názov atribútu položky:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished">Atribút položky bude nastavený na použitý rámec ako základ pre vytváranie záznamov</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished">Umiestniť čísla strán záznamov na začiatok alebo koniec riadku, alebo ich neumiestniť vôbec</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished">Zoznam netlačiteľných položiek</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished">Vrátane rámcov, ktoré sú označené ako netlačiť</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished">Štýl odstavca použitý pre riadok záznamu</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished">Štýl odstavca:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished">Cieľový rámec:</translation>
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
        <translation>Ignorovať všetky chyby</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Automatická kontrola pre tlačou alebo exportom</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Kontrolovať chýbajúce glyfy</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Kontrolovať objekty mimo strany</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Kontrolovať pretečenie v textových rámcoch</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Kontrolovať použitie priesvitnosti</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Kontrolovať chýbajúce obrázky</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Kontrolovať rozlíšenie obrázkov</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Najnižšie povolené rozlíšenie</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Kontrolovať umiestnené PDF súbory</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrolovať PDF anotácie a polia</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Pridať profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Odstrániť profil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Bežné nastavenia</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Umiestnenie v dokumentoch</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>V pozadí</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>V popredí</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Priťahovanie</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Vzdialenosť pritiahnutia:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Zachytiť polomer:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Zobraziť vodítka</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farba:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Zobraziť okraje</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Ukázať mriežku strany</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Hlavná mriežka</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Rozostup:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Vedľajšia mriežka</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Zobraziť základnú čiaru</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Nastavenia základnej čiary</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Mriežka zá&amp;kladných čiar:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>P&amp;osun základných čiar:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Vodítka nie sú viditeľné cez objekty na strane</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Vodítka  sú viditeľné nad objektami na strane</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Vzdialenosť medzi čiarami vedľajšej mriežky</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Vzdialenosť medzi čiarami hlavnej mriežky</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Vzdialenosť, v ktorej sa objekt bude pritiahnutý k umiestneným vodítkam</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Polomer oblasti, ktorú Scribus považuje za oblasť daného objektu </translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farba vedľajších čiar mriežky</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farba hlavných čiar mriežky</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farba vodítok, ktoré ste vložili</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farba pre čiary okrajov</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Farba pre mriežku základných čiar</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Zapnutie/Vypnutie základnej mriežky</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Vzdialenosť medzi čiarami mriežky základných čiar</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Vzdialenosť od horného okraja strany k prvej základnej čiare</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Zapnutie/Vypnutie čiar mriežky</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Zapnutie/Vypnutie mriežky</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Zapnutie/Vypnutie okrajov</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Správa tabulátorov</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Exportovať rozsah</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Všetky str&amp;any</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Zvo&amp;liť strany</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Otočenie:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Súborové nastavenia</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Kompatibili&amp;ta:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Viazanie:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Ľavý okraj</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Pravý okraj</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Vy&amp;tvoriť náhľady</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>U&amp;ložiť prepojené textové rámce ako PDF články</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Vrátane zálož&amp;iek</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="obsolete">Vrátane vrstiev</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Rozlíšenie pre EPS grafiku:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primovať text a vektorovú grafiku</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Nastavenie obrázkov</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatické</translation>
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
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metóda:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kvalita:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maximum</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Vysoká</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Stredná</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Nízka</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimum</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="obsolete">Zmena vzorkovania obrázkov na:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Všeobecné</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Vložiť vš&amp;etky písma</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Podmnožina všetkých pí&amp;sem</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Vkladanie</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Dostupné písma:</translation>
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
        <translation>Písma na vloženie:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Písma do podmnožiny:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Písma</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>&amp;Povoliť prezentačné efekty</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Zo&amp;braziť náhľady pre strany</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Doba zobrazenia:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>&amp;Trvanie efektu:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>T&amp;yp efektu:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Pres&amp;úvanie riadkov:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Z:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>S&amp;mer:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Bez efektu</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Pruhy</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Box</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Rozpustenie</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Lesk</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Rozdeliť</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Zotrieť</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodorovne</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Zvisle</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Vnútorný</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Vonkajší</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Zľava doprava</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Zhora dole</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Zdola hore</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Sprava doľava</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Zľava hore doprava dole</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Použiť efekt n&amp;a všetky strany</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Po&amp;užiť šifrovanie</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Heslá</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Používateľ:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>V&amp;lastník:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Nastavenia</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>&amp;Povoliť tlač dokumentu</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>P&amp;ovoliť zmeny dokumentu</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Povoliť kopírovanie textu a grafik&amp;y</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Povoliť pridávanie &amp;anotácií a polí</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>Be&amp;zpečnosť</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Všeobecné</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Plánovaný &amp;výstup pre:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Obrazovka/Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Tlačiareň</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Odtiene šedej</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertovať bodové (spot) farby na procesné (process)</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>Po&amp;užiť vlastné nastavenia generovania</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Nastavenie generovania</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Fre&amp;kvencia:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Uhol:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Funkcia &amp;bodu:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Bodka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Čiara</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Kruh</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipsa</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Plné farby (Solid colors):</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Použiť ICC-profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Účel generovania:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Vnímateľné (Perceptual)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relatívna kolorimetria</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sýtosť</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolútna kolorimetria</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Obrázky:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Nepoužívať vložené ICC profily</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farba</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Výstup do PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informačný reťazec:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Výstupný &amp;profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Orezanie</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Vložiť písma do PDF. Vloženie písma
zachová rozloženie a vzhľad vášho dokumentu.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation>Povolenie prezentačných efektov pri celoobrazovkovom použití Adobe&amp;#174; Reader&amp;#174.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Ukázať náhľad každej strany v zozname uvedenom hore.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Ako dlho je strana zobrazená, než sa spustí prechod na ďalšiu stranu.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Ako dlho beží prezentačný efekt.
Menšia hodnota efekt zrýchli, väčšia spomalí.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Typ efektu.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Smer efektu presúvania riadkov pre efekty pruhy a rozdeliť.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Štartovacia pozícia efektov box a rozdeliť.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Smer efektu lesk alebo zotrieť.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Použiť zvolený efekt na všetky strany.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportovať všetky strany do PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportovať rozsah strán do PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vložiť čiarkou oddelený zoznam prvkov,
kde prvok môže byť * pre všetky strany,
1-5 pre rozsah strán alebo jedno číslo strany.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Určuje PDF kompatibilitu.
Štandardne je to PDF 1.3, ktoré poskytuje najširšiu kompatibilitu.
PDF 1.4 si vyberte v prípade, že váš súbor obsahuje vlastnosti ako napríklad priesvitnosť, alebo požaduje 128 bitové šifrovanie.
PDF/X-3 je pre export PDF, keď chcete manažovať RGB pre komerčnú tlač 
  a je voliteľné len v prípade aktivácie správy farieb.
Použite iba keď je to odporúčané vašou tlačiarňou, alebo v niektorých prípadoch tlače na 4 farebnej digitálnej laserovej tlačiarni.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Určuje viazanie strán v PDF. Ak nepotrebujete špeciálne nastavenia,
použite štandardné nastavenie - Vľavo.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Generuje náhľady všetkých strán v PDF.
Niektoré prehliadače ich používajú k navigácií.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Generuje PDF články, čo umožňuje navigáciu odkazov v PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="obsolete">Vrstvy vo vašom dokumente sú exportovane do PDF.
Dostupné je to, len ak je zvolené PDF 1.5.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Vložiť záložky obsiahnuté v dokumente.
Tie sú užitočné pri navigácií v rozsiahlych dokumentoch.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Rozlíšenie textu a vektorovej grafiky pri exporte.
Neovplyvňuje rozlíšenie bitmapových obrázkov ako sú napr. fotografie.</translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Povolenie bezstratovej kompresia textu a grafiky.
Pokiaľ na to nemáte dôvod, tak nechajte túto voľbu povolenú. Znižuje veľkosť PDF súboru.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Metóda kompresie pre obrázky. Automaticky - Scribus vyberie najvhodnejšiu metódu. ZIP - bezstratová kompresia vhodná pre obrázky s plnými farbami. JPEG - najlepšia voľba pre vytváranie malých PDF súborov, ktoré majú veľa fotiek (malá strata kvality). Pokiaľ nepotrebujete špeciálnu kompresiu, tak nechajte túto voľbu nastavenú na Automaticky.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Úroveň kompresie: Minimálna (25%), Nízka (50%), Stredná (75%), Vysoká(85%), Maximálna (95%)</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="obsolete">Prevzorkovanie vašich bitmapových obrázkov na zvolené DPI.
Ak voľbu nepovolíte, bude im ponechané pôvodné rozlíšenie.
Toto zvýši použitie pamäte a spomalí export.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Body na palec) pre export obrázkov.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Povoliť bezpečnostné vlastnosti exportovaného PDF.
Pokiaľ ste si vybrali PDF 1.3, PDF bude chránené 40 bitovým šifrovaním.
Pokiaľ ste si vybrali PDF 1.4, PDF bude chránené 128 bitovým šifrovaním.
Poznámka: PDF šifrovanie nie je tak spoľahlivé ako GPG alebo PGP a má svoje obmedzenia.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Zvoľte si hlavné heslo, ktoré povolí alebo zakáže všetky
bezpečnostné vlastnosti exportovaného PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Zvoľte heslo pre používateľov, ktorí budú môcť čítať PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation> Povolí tlač PDF. Ak je odznačené, tlač bude zakázaná. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Povolí úpravu PDF. Ak je odznačené, úprava PDF bude zakázaná. </translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete"> Povolí kopírovanie textu a grafiky z PDF.
Ak je odznačené, kopírovanie textu a grafiky z PDF bude zakázané. </translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete"> Povolí pridávanie anotácií a polí do PDF.
 Ak je odznačené, úprava anotácií a polí bude zakázaná. </translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Farebný model výstupu PDF.
Zvoľte Monitor/Web pre PDF, ktoré sú určené na prezeranie na monitore a tlačené na bežnej atramentovej tlačiarni.
Zvoľte Tlačiareň, ak bude dokument tlačiť na skutočnej farebnej CMYK tlačiarni.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Toto je pokročilé nastavenie, ktoré nie je štandardne povolené. Malo by byť povolené len v tom prípade
pokiaľ je špeciálne vyžadované tlačiarňou a máte k dispozícií presné detaily.
Inak vytvoríte PDF, ktoré sa nebude dať správne vytlačiť a nebude skutočne prenosné medzi systémami.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Vložiť farebný profil pre plné farby</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farebný profil pre plné farby</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Účel generovania pre plných farieb</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Vložiť farebný profil pre obrázky</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Nepoužívať farebné profily, ktoré sú vložené do zdrojových obrázkov</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farebný profil pre obrázky</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Účel generovania pre obrázky</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Výstupný profil pre tlač. Ak je to možné, použite príručku k tlačiarni pri výbere profilu.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Povinný reťazec pre PDF/X-3 - inak PDF nebude
spĺňať špecifikáciu PDF/X-3. Doporučujeme použiť názov dokumentu.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Vzdialenosť orezania od horného fyzického okraja strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Vzdialenosť orezania od dolného fyzického okraja strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Vzdialenosť orezania od ľavého fyzického okraja strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Vzdialenosť orezania od pravého fyzického okraja strany</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Vodorovne zrkadliť stranu(y)</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Zvislo zrkadliť stranu(y)</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished">Rozloženie strany</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished">Jednoduchá strana</translation>
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
        <translation>Písmo:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Veľkosť:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Farba textu:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Tieňovanie:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Obrys textu:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Farba výplne:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Farba obrysu:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Bodka</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Spojovník</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podtržítko</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Znaky výplne tab:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Šírka tab:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stĺpce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Príliš žltučký kôň spieval diabolské ódy</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Farba čia&amp;ry:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Tieňovanie:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Farba výplne:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Tie&amp;ňovanie:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Štýl čiary:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Ší&amp;rka čiary:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Š&amp;týl čiary:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Šípky:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Začiatok:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Koniec:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Voľ&amp;ná zmena veľkosti</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vodorovné zväčšenie:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Zvislé zväčšenie:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Z&amp;meniť veľkosť obrázka na rozmer rámca</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Zachovať pome&amp;r strán</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Farba výplne:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Náhľad na obrazovke</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Náhľad v plnom rozlíšení</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Náhľad v bežnom rozlíšení</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Náhľad v nízkom rozlíšení</translation>
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
        <translation>&amp;Krokovanie:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Vlastnosti textového rámca</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Vlastnosti rámca obrázka</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Vlastnosti kreslenia tvarov</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Štandardné úrovne zväčšenia</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Vlastnosti kreslenia čiar</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Vlastnosti kreslenia mnohouholníkov</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Písmo pre nový textový rámec</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Veľkosť písma pre nový textový rámec</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Farba písma</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Počet stĺpcov v textovom rámci</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Odstupy medzi stĺpcami textu</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Ukážka vášho písma</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Rámce obrázkov umožňujú zmenu na ľubovoľnú veľkosť</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Vodorovné zväčšenie obrázkov</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Zvislé zväčšenie obrázkov</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Dodržať rovnakú vodorovné aj zvislú zmenu veľkosti</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Obrázky v obrázkových rámcoch budú menené podľa rozmerov rámu</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automaticky nastavovaná veľkosť obrázkov dodržuje originálne proporcie</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Farba výplne obrázkových rámcov</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Sýtosť farby výplne</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Farba čiar tvarov</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Sýtosť farby čiary</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Farba výplne tvarov</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Štýl čiary tvarov</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Šírka čiar tvarov</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Minimálne povolené zväčšenie</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Maximálne povolené zväčšenie</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Zmena zväčšenia pre každý krok lupy</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Farba čiar</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Sýtosť farby</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Štýl čiar</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Šírka čiar</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Vlastné:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation> Vlastné:</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Dolný index</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Posunutie:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Zmena veľkosti:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Horný index</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>P&amp;osunutie:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Z&amp;mena veľkosti:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčiarknuté</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Posunutie:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Šírka čiary:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Prečiarknuté</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Malé kapitálky</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Zmen&amp;a veľkosti:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automa&amp;tické riadkovanie</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Riadkovanie:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Posunutie nad základnú čiaru písma na riadku</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relatívna veľkosť horného indexu v porovnaní s bežným písmom</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Posunutie pod základnú čiaru bežného písma na riadku</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relatívna veľkosť dolného indexu v porovnaní s bežným písmom</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relatívna veľkosť malých kapitálok v porovnaní s bežným písmom</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Percentuálny nárast riadkovania podľa veľkosti písma</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Posunutie pod základnú čiaru bežného písma vyjadreného ako percento poklesu písma</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Šírka riadku vyjadrená ako percento z veľkosti písma</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Posunutie nad základnú čiaru bežného písma vyjadreného ako percento vystúpenia písma</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Vľavo</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Bodka</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Čiarka</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Na stred</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Pozícia:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žiadne</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Bodka</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Spojovník</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podtržítko</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastné</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Znak výplne:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Odstrániť všetko</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Odsadenie pre prvý riadok odseku</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Odsadenie zľava pre celý odsek</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Odstrániť všetky tabulátory</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Vlastné:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation> Vlastné:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Free Objects</source>
        <translation>Voľné objekt</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Zoskupiť </translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Strana </translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Prehľad</translation>
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
        <translation>Auto</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Posunutie</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Šírka čiary</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Pridať zvislé vodítko</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Pridať vodorovné vodítko</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Odstrániť zvislé vodítko</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Odstrániť vodorovné vodítko</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Presunúť zvislé vodítko</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Presunúť vodorovné vodítko</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Uzamknúť vodítka</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Odomknúť vodítka</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Presunúť</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Zmena veľkosti</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Otočiť</translation>
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
        <translation>Š1: %1, V1: %2
Š2: %3, V2: %4</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Výber</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Skupina</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Výber/Skupina</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Vytvoriť</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
Š: %3, V: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Zarovnať/Umiestniť</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Zahrnuté položky</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušiť</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Nastaviť farbu vyplnenia</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Farba1: %1, Farba2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Nastaviť farbu tieňa vyplnenia</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Nastaviť farbu čiary</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Nastaviť farbu tieňa čiary</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Vodorovne zameniť</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Zvislo zameniť</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Uzamknúť</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Odomknúť</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Uzamknúť veľkosť</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Odomknúť veľkosť</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Zrušiť zoskupenie</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Odstrániť</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Premenovať</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Z %1
do %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Použiť šablónu</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Vložiť</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Vystrihnúť</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Nastaviť farbu priesvitnosti</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Nastaviť farbu priesvitnosti čiary</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Nastaviť štýl čiary</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Nastaviť štýl konca čiary</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Nastaviť štýl spojenia čiary</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Nastaviť šírku čiary</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Bez štýlu</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Nastaviť vlastný štýl čiary</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Nepoužívať vlastný štýl čiar</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Nastaviť začiatok šípky</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Nastaviť koniec šípky</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Vytvoriť tabuľku</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Riadky: %1, stĺpci: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Nastaviť písmo</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Nastaviť veľkosť písma</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Nastaviť šírku písma</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Nastaviť výšku stĺpca</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Nastaviť farbu vyplnenia písma</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Nastaviť farbu obrysu písma</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Nastaviť farbu tieňa vyplnenia písma</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Nastaviť farbu tieňa obrysu písma</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Nastaviť párovanie (kerning)</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Nastaviť riadkovanie</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Nastaviť štýl odstavca</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Nastaviť jazyk</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Zarovnať text</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Nastaviť efekty písma</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Rámec obrázka</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Textový rámec</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnohouholník</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bézierová krivka</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomená čiara</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Konverzia na</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Import SVG obrázok</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importovať EPS obrázok</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Import OpenOffice.org Draw obrázok</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Text obteká okolo rámca</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Text obteká okolo viazaného pola</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Text obteká okolo vrstevnice</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Bez obtekania textu</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Bez viazaného pola</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Bež vrstevnice</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Strana  %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Nastaviť mierku obrázka</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Veľkosť rámca</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Voľná zmena veľkosti</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Zachovať pomer strán</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Porušiť pomer strán</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Upraviť vrstevnicu</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Upraviť tvar</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Vynulovať vrstevnicu</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Pridať stranu</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Pridať strany</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Odstrániť stranu</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Odstrániť strany</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Pridať vrstvu</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Odstrániť vrstvu</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Premenovať vrstvu</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Zvýšiť vrstvu</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Znížiť vrstvu</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Poslať do vrstvy</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Povoliť tlač vrstvy</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Zakázať tlač vrstvy</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Zmeniť názov vrstvy</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Získať obrázok</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Text na cestu</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Počiatočný status</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>História úkonov</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Zobraziť iba zvolený objekt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>V&amp;rátiť späť</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Opakovať vrátené</translation>
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
        <translation>Vložiť hodnotu</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Vložte hodnotu a potom stlačte OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Vložte hodnotu a potom stlačte OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Pošle vaše hodnoty do skriptu</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="obsolete">Upraviť &amp;systémovú konfiguráciu...</translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="obsolete">Upraviť po&amp;užívateľskú konfiguráciu...</translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="obsolete">Nastavenie &amp;editora...</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Upraviť</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="obsolete">&amp;Info a jazyky...</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Pomocník</translation>
    </message>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Krátke slová</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Použiť nezalomiteľnú medzeru na:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Zvolené rámce</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktívna st&amp;rana</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Všetky str&amp;any</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Nahradiť štandardné používateľkou konfiguráciou</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete"> Ak existuje používateľská konfigurácia
(%1)
môžete si vybrať, či chcete pridať vašu konfiguráciu 
ku globálnej konfigurácií pomocou odškrtnutia tlačítka.

Môžete nahradiť preddefinované hodnoty vašimi
tiež s pomocou tlačidla. </translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Spracovať iba označené rámce.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Spracovať iba aktuálnu stranu.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Všetky položky dokumentu boli spracované.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Scribus - krátke slová </translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Dostupné v nasledovných jazykoch</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">O krátkych slovách</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="obsolete">Krátke slová</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="obsolete">Začínate upravovať súbor, ktorý iba na čítanie.
%1</translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="obsolete">Chyba aplikácie &apos;%1&apos;. Nie je možné začať.</translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="obsolete">Nastavenie Krátkych slov</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Vlastnosti...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF nástroje</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Tlačítko</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textové pole</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Políčko k zaškrtnutiu</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Výber</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Zoznam</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Vložiť PDF polia</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Vložiť PDF anotáciu</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vyberte import, ktorý sa má použiť</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatický</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importovanie iba textu</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importovať text bez formátovania</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importér:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kódovanie:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vyberte import, ktorý sa má použiť</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Zapamätať si asociáciu</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Pamätať si príponu súboru tohoto typu ako asociáciu a nepýtať sa znova na typ importu.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nový podľa šablóny</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Otvoriť</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všetky</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farby</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Použitie</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Vytvorené v</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dátum</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Stiahnutie šablón</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Šablóny dokumentov môžete nájsť na http://www.scribus.net/ v časti Stiahnutie (Downloads).</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Inštalácia šablón</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extrahujte balík to priečinka šablón ~/.scribus/templates pre aktuálneho používateľa alebo do PREFIX/share/scribus/templates pre všetkých používateľov.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Príprava šablóny</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Uistite sa, že písma a obrázky, ktoré ste použili je možné voľne používať. Ak písma nie je možné zdieľať s inými, tak ich nevyberte pri ukladaní šablóny.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Tvorca šablóny by sa mal uistiť, či sa sekcia Inštalácia šablón, ktorá je uvedená vyššie, týka tiež aj jeho šablón. To znamená, že používateľ by mal byť schopný si stiahnuť balík šablón, extrahovať ich do priečinku šablón a používať ich.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Odstránenie šablóny</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Odstránenie šablóny z dialógu Nový podľa šablóny iba odstráni položku zo súboru template.xml a nespôsobí odstránenie súborov dokumentu. Pomocou dialógu je možné iba odstránenie zobrazenia šablóny, ak mate prístupové práva k zápisu súboru template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Preloženie template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Skopírujte existujúci súbor template.xml do súbora s názvom template.jazyk_KRAJINA.xml (použite rovnaký kód jazyka, ako ten, ktorý je použitý pre qm súbor vášho jazyka), napríklad  template.fi.xml pre fínsky preklad template.xml. Kópia musí byť umiestnená v rovnakom priečinku ako pôvodný template.xml, aby ho Scribus mohol načítať.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Uložiť ako šablónu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Názov</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategória</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Veľkosť strany</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farby</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Použitie</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-mail</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Viac detailov</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Menej detailov</translation>
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
        <translation>na šírku</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>na výšku</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>vlastné</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Vytvoriť filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Zmazať</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Odstrániť</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Vybrať uložený filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Zadajte názov pre tento filter pre ukladanie</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Zadajte názov pre ukladanie</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Povoliť alebo zakázať teraz tento filter</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Odstrániť tento riadok filtra</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Pridať nový riadok filtra</translation>
    </message>
    <message>
        <source>to</source>
        <translation>na</translation>
    </message>
    <message>
        <source>and</source>
        <translation>a</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>odstrániť zhodné</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>neodstraňovať zhodné</translation>
    </message>
    <message>
        <source>words</source>
        <translation>slová</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstrániť</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Nahradiť</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Použiť</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Hodnota vľavo je regulárny výraz</translation>
    </message>
    <message>
        <source>with</source>
        <translation>s</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>štýl odstavca</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>všetky inštancie</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>všetky odstavce</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>odstavce začínajúce s </translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>odstavce s menej než</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>odstavce s viac než</translation>
    </message>
</context>
</TS>
