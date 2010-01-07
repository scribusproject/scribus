<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
s.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
ed.
</translation>
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

numPage = Number of pages to be created.

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</source>
        <translation>newDocument(size, margins, orientation, firstPageNumber,
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
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
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
        <source>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
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
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</translation>
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

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
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

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB color
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
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
        <translation>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
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

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</translation>
    </message>
    <message>
        <source>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the color &quot;name&quot;. Every occurrence of that color is replaced by the
color &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the color
&quot;None&quot; - transparent.

deleteColor works on the default document colors if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">deleteColor(&quot;name&quot;, &quot;replace&quot;)

Deletes the color &quot;name&quot;. Every occurrence of that color is replaced by the
color &quot;replace&quot;. If not specified, &quot;replace&quot; defaults to the color
&quot;None&quot; - transparent.

deleteColor works on the default document colors if there is no document open.
In that case, &quot;replace&quot;, if specified, has no effect.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurrence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurrence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
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
        <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -&gt; string with filename

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
        <translation>messageBox(&quot;caption&quot;, &quot;message&quot;,
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
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation>newStyleDialog() -&gt; string

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
        <translation>newDoc(size, margins, orientation, firstPageNumber,
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
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

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
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

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
        <translation>setMargins(lr, rr, tr, br)

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
        <translation>setUnit(type)

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
        <translation>getUnit() -&gt; integer (Scribus unit constant)

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
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

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

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

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
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

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
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

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

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string

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
        <translation>getPosition([&quot;name&quot;]) -&gt; (x,y)

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
        <translation>getSize([&quot;name&quot;]) -&gt; (width,height)

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
        <translation>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</translation>
    </message>
    <message>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation>getPropertyCType(object, property, includesuper=True)

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
        <translation>getPropertyNames(object, includesuper=True)

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
        <translation>getProperty(object, property)

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
        <translation>setProperty(object, property, value)

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
        <translation>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

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
        <translation>getChild(object, childname, ofclass=None, recursive=True)

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
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

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
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

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
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

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
        <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

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
        <translation>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list of tuples

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
        <translation>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -&gt; bool

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
        <translation>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

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
        <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

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
        <translation>setLayerVisible(&quot;layer&quot;, visible)

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
        <translation>setLayerPrintable(&quot;layer&quot;, printable)

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
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

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
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

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
        <translation>deleteLayer(&quot;layer&quot;)

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
        <translation>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

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
        <translation>createEllipse(x, y, width, height, [&quot;name&quot;]) -&gt; string

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
        <translation>createImage(x, y, width, height, [&quot;name&quot;]) -&gt; string

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
        <translation>createText(x, y, width, height, [&quot;name&quot;]) -&gt; string

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
        <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

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
        <translation>createPolyLine(list, [&quot;name&quot;]) -&gt; string

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
        <translation>createPolygon(list, [&quot;name&quot;]) -&gt; string

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
        <translation>createBezierLine(list, [&quot;name&quot;]) -&gt; string

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
        <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -&gt; string

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
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; bool

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
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

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
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation>gotoPage(nr)

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
        <translation>pageCount() -&gt; integer

Returns the number of pages in the document.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

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
        <translation>setHGuides(list)

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
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

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
        <translation>getPageItems() -&gt; list

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
        <translation>getPageMargins()

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

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
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

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
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
        <translation>setLineJoin(join, [&quot;name&quot;])

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
        <translation>setLineEnd(endtype, [&quot;name&quot;])

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
        <translation>setLineStyle(style, [&quot;name&quot;])

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

Sets the shading of the fill color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full Color intensity). If &quot;name&quot; is not given the currently selected
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

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

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
        <translation>getFont([&quot;name&quot;]) -&gt; string

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
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

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
        <translation>getText([&quot;name&quot;]) -&gt; string

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
        <translation>getAllText([&quot;name&quot;]) -&gt; string

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
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

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
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

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

May throw ValueError for a font size that&apos;s out of bounds.
</source>
        <translation>setFontSize(size, [&quot;name&quot;])

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

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
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

Sets the shading of the text color of the object &quot;name&quot; to &quot;shade&quot;. If
there is some text selected only the selected text is changed. &quot;shade&quot; must
be an integer value in the range from 0 (lightest) to 100 (full color
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
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;name&quot;)

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
        <translation>traceText([&quot;name&quot;])

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
        <translation>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

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
        <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

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
        <translation>isPDFBookmark([&quot;name&quot;]) -&gt; bool

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

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

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
        <translation>progressSet(nr)

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
        <translation>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
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
        <source>getFillTransparency([&quot;name&quot;]) -&gt; float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getFillTransparency([&quot;name&quot;]) -&gt; float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</translation>
    </message>
    <message>
        <source>getLineTransparency([&quot;name&quot;]) -&gt; float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineTransparency([&quot;name&quot;]) -&gt; float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
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
        <source>getTextDistances([&quot;name&quot;]) -&gt; tuple

Returns the text distances of the text frame &quot;name&quot; expressed in points. The
distances are returned as a tuple like (left, right, top, bottom). If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setTextDistances(left, right, top, bottom, [&quot;name&quot;])

Sets the text distances of the text frame &quot;name&quot; to the values &quot;left&quot;
&quot;right&quot;, &quot;top&quot; and &quot;bottom&quot;. If &quot;name&quot; is not given the currently
selected item is used.

May throw ValueError if any of the distances are out of bounds (must be positive).
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation>Acerca de Scribus %1</translation>
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
        <translation>Usando Ghostscript versión %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ninguna versión de Ghostscript disponible</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus versión %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>ID de compilación:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Acerca de</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Equipo de desarrollo:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contribuidores:</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Adaptación a Mac OS&amp;#174; X Aqua:</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Adaptación a Windows&amp;#174;:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Documentación oficial:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Otra documentación:</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation>Iconos del proyecto Tango:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utores</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Traducción oficial y traductores:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Anteriores contribuidores de la traducción:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Traducciones</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Página principal</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencia en línea</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Errores y peticiones de características</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation type="obsolete">Lista de correo</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;En línea</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Este panel muestra la versión y fecha de compilación de Scribus. C-C-T-F significa el soporte para c=littlecsm C=CUPS T=soporte TIFF y F=soporte fontconfig. La última letra es el motor de renderizado C=cairo o A=libart. Las bibliotecas de soporte no disponibles están indicadas mediante un *. También indica la versión de Ghostscript que Scribus ha detectado.</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>Adaptación a OS/2&amp;#174;/eComStation&amp;#8482;:</translation>
    </message>
    <message>
        <source>Developer Blog</source>
        <translation>Blog del desarrollador</translation>
    </message>
    <message>
        <source>Mailing Lists</source>
        <translation>Listas de correo</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Filename:</source>
        <translation>Nombre de archivo:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Versión:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Activado:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Fecha de publicación:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Autor/es:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licencia:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: Acerca de las extensiones</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
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
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Guardar c&amp;omo...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Re&amp;vertir al guardado</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Recolectar para sa&amp;lida...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Obtener texto...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Añadir &amp;texto...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Obtener imagen...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Guardar &amp;texto...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Guardar página como &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Guardar como P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Con&amp;figuración del documento...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Im&amp;primir...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;Vista previa de impresión</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rehacer</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Modo de acción de objeto</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Co&amp;rtar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Pegar (&amp;absoluto)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Seleccion&amp;ar todo</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Deseleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Bu&amp;scar/Reemplazar...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Editar imagen...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>C&amp;olores...</translation>
    </message>
    <message>
        <source>S&amp;tyles...</source>
        <translation>Es&amp;tilos...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Estilos de &amp;párrafo...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Estilos de &amp;línea...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>Páginas &amp;maestras...</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScript...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>P&amp;referencias...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Otros...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Izquierda</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Centro</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Derecha</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Justificado</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Forzado</translation>
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
        <translation>S&amp;ubrayado</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Subrayar pala&amp;bras</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Tachar</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Capitales</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Versalitas</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Su&amp;períndice</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Su&amp;bíndice</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Contorno</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>So&amp;mbras</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>Efectos de &amp;imagen</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabuladores...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Duplicado &amp;múltiple</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>A&amp;grupar</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Desagr&amp;upar</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>B&amp;loqueado</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Tama&amp;ño bloqueado</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>Activada im&amp;presión</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>&amp;Voltear horizontalmente</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>&amp;Voltear verticalmente</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Bajar al &amp;fondo</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Subir al su&amp;perior</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Subir</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Enviar al álbum de re&amp;cortes</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atributos...</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Más información...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>I&amp;magen visible</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>Act&amp;ualizar imagen</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Ajustar marco a imagen</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Propiedades de imagen adicionales</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>Baja reso&amp;lución</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>Resolución &amp;normal</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>Alta re&amp;solución</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Es un &amp;marcador PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Es una a&amp;notación PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>P&amp;ropiedades de anotación</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>P&amp;ropiedades de campo</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editar forma...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Añadir texto a trazado</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Separar texto de trazado</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Combinar polígonos</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Separar &amp;polígonos</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Curva &amp;Bézier</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>Marco de &amp;imagen</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Contornos</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polígono</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Marco de &amp;texto</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Carácter...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Texto de ejemplo</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Insertar...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;portar...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Borrar...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Copiar...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Mover...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Aplicar página maestra...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Convertir a página maestra...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Organizar &amp;guías...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Organizar propiedades de página...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>A&amp;justar a ventana</translation>
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
        <translation>Minia&amp;turas</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Mostrar &amp;márgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Mostrar &amp;marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Mostrar &amp;imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Mostrar &amp;cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Mostrar g&amp;uías</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Mostrar columnas de marcos de texto</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Mostrar cuadrícula de línea de &amp;base</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Mostrar encadenación de &amp;texto</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Mostrar caracteres de control</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Mostrar reglas</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Reglas relativas a la página</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Ajustar a cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>A&amp;justar a guías</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Propiedades</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>C&amp;ontorno</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>Al&amp;bum de recortes</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Capas</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>Orden&amp;ar páginas</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>Marca&amp;dores</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Mediciones</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Historial de acciones</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Verificación previa</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Alinear y distribuir</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Herramientas</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Herramientas P&amp;DF</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Seleccionar objeto</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abla</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Línea</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Línea a mano al&amp;zada</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotar objeto</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Ampliar</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Reducir</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Editar contenido del marco</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Editar texto...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Enlazar marcos de texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Desenlazar marcos de texto</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>Cu&amp;entagotas</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Copiar propiedades del objeto</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Editar el texto en el editor interno</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Insertar marco de texto</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Insertar marco de imagen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Insertar tabla</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Insertar forma</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Insertar polígono</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Insertar línea</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Insertar curva Bézier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Insertar línea a mano alzada</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Dividir texto por síla&amp;bas</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Deshacer la división por sílabas</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Generar tabla de contenido</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Cascada</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Mosaico</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Acerca de Scribus</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;Acerca de las extensiones</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Acerca de &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Su&amp;gerencias</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Manual de Scribus...</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Des/activar paletas</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Des/activar guías</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Di&amp;visión por sílabas inteligente</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Guión de no separación</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>E&amp;spacio de no separación</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;Numeración de página</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Nueva línea</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Salto de marco</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Salto de columna</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Marca registrada</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Marca comercial</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Barra</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Viñeta</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Punto medio</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Raya</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Semirraya</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Raya gráfica</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Raya introductoria</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apóstrofe</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Dobles rectas</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Simples de apertura</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Simples de cierre</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Dobles de apertura</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Dobles de cierre</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Simples invertidas</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Dobles invertidas</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Españolas simples de apertura</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Españolas simples de cierre</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Españolas de apertura</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Españolas de cierre</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Coma baja simple
</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Coma baja doble</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>CJK simples de apertura</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>CJK simples de cierre</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>CJK dobles de apertura</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>CJK dobles de cierre</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Espacio corto</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Espacio largo</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Insertar guión inteligente</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Insertar guión de no separación</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Insertar espacio de no separación</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Insertar numeración de página</translation>
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
        <source>&amp;400%</source>
        <translation>&amp;400%</translation>
    </message>
    <message>
        <source>&amp;Manage Images</source>
        <translation>&amp;Gestión de imágenes</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Alinear y distribuir</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Alinear</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativo a:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Primer seleccionado</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Último seleccionado</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Márgenes</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Selección</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Alinear lados derechos de los objetos al lado izquierdo del anclaje</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Alinear lados izquierdos de los objetos al lado derecho del anclaje</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Alinear los lados inferiores</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Alinear lados derechos</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Alinear los lados superiores de los objetos al lado inferior del anclaje</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Centrar con el eje vertical</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Alinear lados izquierdos</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Centrar con el eje horizontal</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Alinear los lados inferiores de los objetos con el lado superior del anclaje</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Alinear los lados superiores</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>Guía &amp;seleccionada:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Distribuir</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Igualar espaciado horizontal entre objetos</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Hacer espaciado horizontal entre objetos igual al valor especificado</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Distribuir lados derechos equidistantemente</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Distribuir lados inferiores equidistantemente</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Distribuir centros equidistantemente horizontalmente</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Igualar espaciado vertical entre objetos</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Hacer espaciado vertical entre objetos igual al valor especificado</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Distribuir lados izquierdos equidistantemente</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Distribuir centros equidistantemente verticalmente</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Distribuir lados superiores equidistantemente</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Distancia:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Distribuir los objetos con la distancia especificada</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Nada seleccionado</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Algunos objetos están bloqueados.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Desbloquear todos</translation>
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
        <translation>Alinear texto a la izquierda</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinear texto a la derecha</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Centrar texto</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Justificar texto</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Justificación forzada del texto</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Propiedades de campo</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tipo:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de marcado</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Casilla combo</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Casilla de lista</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Sugerencia:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Tipografía a utilizar con PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Borde</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Ancho:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Fina</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Ancha</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Estilo:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>A rayas</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Biselado</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Recuadro</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Otra</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Sólo lectura</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Requerido</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>No exportar valor</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilidad:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visible</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Oculto</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>No imprimir</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>No mostrar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Apariencia</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Texto al presionar botón</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Texto al pasar por encima</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Iconos</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Usar iconos</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pulsado</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Cursor encima</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Posición del icono...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Resaltar</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertir</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Contorno</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Pulsar</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multilínea</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Contraseña</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Límite de</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Caracteres</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>No desplazar</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>No comprobar ortografía</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Comprobar estilo:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Comprobar</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Cruz</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamante</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Círculo</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Estrella</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Marcado por defecto</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editable</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Ninguna</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ir a</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Enviar formulario</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Reiniciar formulario</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importar datos</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Se suelta el ratón</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Se presiona el ratón</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>El cursor entra</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>El cursor sale</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Se obtiene el foco</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Se pierde el foco</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editar...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Enviar a URL:</translation>
    </message>
    <message>
        <source>Submit format</source>
        <translation type="obsolete">Formato de envío</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importar datos de:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>A archivo:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Cambiar...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Página:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Posición X:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Posición Y:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>El campo tiene el formato:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Sin formato</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Número</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Porcentaje</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Hora</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formato de número</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimales:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Usar unidad monetaria</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Anteponer unidad monetaria</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formato de porcentaje</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formato de fecha</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formato de hora</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Script personalizado</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Pulsación:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>El valor no está validado</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>El valor debe ser mayor o igual a:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>y menor o iqual que:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script personalizado de validación:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>El valor no está calculado</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>El valor es</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>suma</translation>
    </message>
    <message>
        <source>product</source>
        <translation>producto</translation>
    </message>
    <message>
        <source>average</source>
        <translation>promedio</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>mínimo</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>máximo</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>de los siguientes campos:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Elegir...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script de cálculo personalizado:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calcular</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Opción ignorada por PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Introducir una lista de campos separados por coma aquí</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Se necesita al menos el icono normal para usar iconos en los botones</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Imágenes (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Ejemplo:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Cambio de selección</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Submit format:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>FDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HTML</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>XFDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF</source>
        <translation type="unfinished">PDF</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Propiedades de anotación</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Enlace externo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Enlace web exteno</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Página:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>Posición &amp;X</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posición &amp;Y:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Documentos PDF (*.pdf);;Todos los archivos (*)</translation>
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
        <translation>Aplicar página maestra</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>Página &amp;maestra:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Aplicar a</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;Página actual</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Páginas par&amp;es</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>Páginas i&amp;mpares</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Tod&amp;as las páginas</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>Den&amp;tro del rango</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Aplicar la página maestra seleccionada a las página pares, impares o a todas ellas dentro del siguiente rango</translation>
    </message>
    <message>
        <source>to</source>
        <translation>a</translation>
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
        <translation>Ninguno</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>Generador de código de &amp;barras...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Interfaz de Scribus para Pure Postscript Barcode Writer</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 ó 13 dígitos</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 dígitos</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 ó 12 dígitos</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 u 8 dígitos</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 dígitos</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 dígitos</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Número variable de caracteres, dígitos y cualquiera de los símbolos -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Número variable de caracteres ASCII y símbolos de funciones especiales, comenzando con el apropiado carácter de inicio para el set de caracteres inicial. UCC/EAN-128s obliga el uso del símbolo FNC 1 inmediatamente posterior del carácter inicial.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Número variable de dígitos y cualquiera de los símbolos -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Número variable de dígitos</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Número variable de dígitos. Un ITF-14 son 14 caracteres sin dígito de control</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Número variable de dígitos y letras mayúsculas</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Número variable de caracteres hexadecimales</translation>
    </message>
    <message>
        <source>Error opening file: %1</source>
        <translation>Error al abrir el archivo: %1</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Código de barras incompleto</translation>
    </message>
    <message>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation>12 ó 13 dígitos con guiones. El formato antiguo ISBN-10 acepta 9 ó 10 dígitos con guiones, pero este estándar fue marcado como obsoleto para el uso público después del 1 de enero de 2007. (Nota: para convertir un antiguo ISBN-10 al nuevo ISBN-13, anteponga el prefijo 978- a los 9 dígitos, p. ej. 1-56592-479-7 -&gt; 978-1-56592-479. El dígito final de comprobación se calculará automáticamente.)</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Creador de código de barras</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Código de barras</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Có&amp;digo:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Seleccione uno de los tipos de códigos de barras disponibles aquí</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>La representación numérica del código mismo. Mira el mensaje de ayuda siguiente</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Reiniciar los ejemplos de código de barras</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Incluir texto en el código de barras</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>También se incluirán números en el código de barras si está marcada</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Guardar espacio en blanco</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Dibujar flechas para asegurarse del espacio próximo al código</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>I&amp;ncluir suma de control</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Generar e incluir una suma de control  en el código de barras</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Incl&amp;uir dígito de suma de control</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Incluir dígito de suma de control en el texto del código de barras</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Fondo</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Color de fonto - bajo las líneas de código</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Líneas</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Color de las líneas en el código de barras</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Texto</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Color de texto y números</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Los consejos y la ayuda se muestran aquí</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Vista previa del resultado. Muestra a 72dpi.</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Main</source>
        <translation>Principal</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Elige un directorio para el álbum de recortes</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Álbum de recortes (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Elige un archivo álbum de recortes para importar</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.
Por favor, elige otro.</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objeto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nueva entrada</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Álbum de recortes</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>Vista &amp;previa</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Cargar...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Guardar c&amp;omo...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation>&amp;Importar archivo álbum de recortes...</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Mover marcador</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Insertar marcador</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Posición del icono</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Distribución:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Sólo la leyenda</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Sólo el icono</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Leyenda debajo del icono</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Leyenda sobre el icono</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Leyenda a la derecha del icono</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Leyenda a la izquierda del icono</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Leyenda sobre el icono</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Escala:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Siempre</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Cuando el icono es demasiado pequeño</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Cuando el icono es demasiado grande</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nunca</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Modo de escalado:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcional</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>No proporcional</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icono</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Reiniciar</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Activar gestión de color</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Perfiles del sistema</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Colores &amp;sólidos:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Imp&amp;resora:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Prueba de renderizado</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimetría relativa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimetría absoluta</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Impresora:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ular impresora en la pantalla</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Indicar colores fuera de &amp;gama</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Usar compensación de punto &amp;negro</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Perfil de color por defecto para imágenes CMYK importadas</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Perfil de color por defecto para imágenes RGB importadas</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Perfil de color por defecto para colores sólidos en la página</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color que ha generado o recibido del fabricante.
Este perfil debe ser específico para el monitor y no ser un perfil genérico (p. ej. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color del modelo de impresora del fabricante.
Este perfil debe ser específico de la impresora y no ser un perfil genérico (p. ej. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Intento de renderizado por defecto del monitor. A menos que sepas por qué
cambiarlo, deberías elegir colorimetría relativa o perceptual.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Intento de renderizado por defecto de la impresora. A menos que sepas por
qué cambiarlo, deberías elegir colorimetría relativa o perceptual.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Activar comprobación por software de cómo se imprimirán los
colores del documento, basada en el perfil de impresora seleccionado.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Método para mostrar los colores en pantalla que pueden no imprimirse
correctamente. Requiere unos perfiles muy exactos y sirve sólo como aviso.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Compensación de punto negro es un método de mejora de contraste en las
fotografías. Se recomienda activarlo si tiene fotografías en el documento.</translation>
    </message>
    <message>
        <source>&amp;RGB Images:</source>
        <translation>Imágenes &amp;RGB:</translation>
    </message>
    <message>
        <source>&amp;CMYK Images:</source>
        <translation>Imágenes &amp;CMYK:</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Editar color</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Modelo de color</translation>
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
        <translation>RGB seguro para la web</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Es color de tinta plana</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Es un color de registro</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Antiguo</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Mapa de color HSV</translation>
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
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation>Al elegir esta opción, activará la impresión en todos los platos. Los colores de registro se utilizan para marcas de impresora como marcas de recortado, marcas de registro y similares. No se suelen utilizar en la disposición misma.</translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation>Al elegir esta opción, convertirá este color en un color de tinta plana, y por lo tanto, creando otra tinta plana al crear platos o separaciones. Esto se suele usar principalmente cuando un logotipo u otro color necesita una representación exacta o no puede ser replicado con tintas CMYK. Las tintas metálicas y fluorescentes son un buen ejemplo de tintas que no son fácilmente replicables con tintas CMYK.</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Barras de color dinámicas</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barras de color estáticas</translation>
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
        <translation>No puedes crear un color de nombre &quot;%1&quot;.
Es un nombre reservado para el color transparente</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>El nombre del color no es único</translation>
    </message>
    <message>
        <source>You cannot create a color without a name
Please give it a name</source>
        <translation>No puedes crear un color sin nombre.
Por favor, provéalo de uno</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Puede ver una miniatura si mantiene pulsado el botón derecho del ratón. La tecla Insert añade un carácter a la selección siguiente y el botón Supr elimina el último introducido</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Seleccione carácter:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Tipografía:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Clase de carácter:</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>&amp;Insertar código:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insertar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Insertar los caracteres en la posición del cursor</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Eliminar la/s selección/es actuales.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Cerrar este diálogo y volver a la edición de texto</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Escribe un valor unicode de cuatro dígitos directamente aquí</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Set de caracteres completo</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Latín básico</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latín-1 suplemento</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin extendido A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latín extendido B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Puntuación general</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Superíndices y subíndices</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Sistema monetario</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Símbolos tipo carta</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Formatos de números</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Flechas</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Operadores matemáticos</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Bordes de cuadros</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Elementos de bloque</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Formas geométricas</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Símbolos variados</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Figuras geométricas</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Variantes pequeñas</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligaduras</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Especiales</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griego</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Griego extendido</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Cirílico</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Cirílico suplemento</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Árabe</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Árabe extendido A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Árabe extendido B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebreo</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Carácter no disponible</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Desbordamiento de texto</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>El objeto no está en la página</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Imagen no disponible</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>La imagen tiene un valor DPI menor de %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>El objeto tiene transparencia</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>El objeto es una anotación o campo PDF</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>El objeto es un PDF ubicado</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Ningún problema encontrado</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Página </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Liberar objetos</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Se encontraron problemas</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Verificación previa</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Objetos</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problemas</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Perfil actual:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorar errores</translation>
    </message>
    <message>
        <source>Annotation uses a non TrueType font</source>
        <translation>La anotación usa una tipografía no TrueType</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Elegir estilos</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Estilos disponibles</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Recolectando...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>No se pudieron recolectar todos los archivos de salida para el archivo:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>No se pudo recolectar el archivo: 
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Borar no usados</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Sets de color</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Set de color actual:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Guardar set de color</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Elige un set de color a cargar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Guardar el set de color actual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Eliminar colores no usados del set de color del documento actual</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importar colores al set actual de un documento existente</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Crear un nuevo color en el set actual</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Editar el color actual seleccionado</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Hacer una copia del color actual seleccionado</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Eliminar el color actual seleccionado</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Hacer el set de color actual el set de color por defecto</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Si la gestión de color está activada, un indicador triangular de aviso es un aviso de que el color probablemente esté fuera de la gama de color del perfil de impresora actual seleccionado. Esto significa que el color podría no imprimirse exactamente como mostrado en pantalla. Los colores de tinta plana se indican por mediante un círculo rojo. Los colores de registro tendrán una marca de registro al lado del color. En la ayuda en línea, en la sección Gestión de color, hay más información sobre avisos de gama.</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Elige un nombre</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos los documentos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nuevo color</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monocromático</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Análogos</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Complementario</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Complementario dividido</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triádico</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetrádico (dobles complementarios)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Color base</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monocromático claro</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monocromático oscuro</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>1er análogo</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>2º análogo</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>1ª división</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>2ª división</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>3ª división</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>4ª división</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>1er triádico</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>2º triádico</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>1er tetrádico (complementario al base)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>2º tetrádico (angulado)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>3er tetrádico (complementario al angulado)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>Cr&amp;ear color...</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>Componentes de c&amp;olor...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Importar color existente...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Mezclar colores</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>&amp;Reemplazar colores</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>C&amp;olor</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Visión normal</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Protanopia (rojo)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Deuteranopia (verde)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Tritanopia (azul)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Monocromático</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Defectos de visión:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Rueda de color</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Color</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
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
        <translation>Seleccionar método:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Ángulo (0 - 90 grados):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Unir colores</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Reemplazar colores</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Unir colores creado con los colores del documento</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Reemplazar colores creados en los colores del documento</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>No modificar colores</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Diferencia entre el valor seleccionado y los valores contados. Para más información, ver la documentación.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Pulsa en la rueda para obtener el color base. Es la saturación en modo HSV.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Esquema de color de muestra</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Selecciona uno de los métodos para crear un esquema de color. Ver la documentación para más información.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Colores del esquema de color elegido</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Simular defectos de la visión comunes aquí. Selecciona el tipo de defecto.</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Uniendo colores</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Error: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>¡El color %1 ya existe!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Color %1 adicionado.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Abriendo el gestor de color.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Unión de colores</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Color nuevo</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Imposible encontrar el color solicitado. Probablemente, has seleccionado negro, gris o blanco. No hay ningún modo de procesar este color.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>Rueda de &amp;color...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Ayudante de configuración de color</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Selector de color con teoría del color incluida.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Aplicar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Ningún</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Página simple</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Página doble</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>Página triple (tríptico)</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>Página cuáduple</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Página izquierda</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Página central</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Página central izquierda</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Página central derecha</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Página derecha</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Página normal</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation>Página normal izquierda</translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation>Página normal central</translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation>Página normal derecha</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Lunes</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Martes</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>Miércoles</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>Jueves</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>Viernes</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>Sábado</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Domingo</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Enero</translation>
    </message>
    <message>
        <source>February</source>
        <translation>Febrero</translation>
    </message>
    <message>
        <source>March</source>
        <translation>Marzo</translation>
    </message>
    <message>
        <source>April</source>
        <translation>Abril</translation>
    </message>
    <message>
        <source>May</source>
        <translation>Mayo</translation>
    </message>
    <message>
        <source>June</source>
        <translation>Junio</translation>
    </message>
    <message>
        <source>July</source>
        <translation>Julio</translation>
    </message>
    <message>
        <source>August</source>
        <translation>Agosto</translation>
    </message>
    <message>
        <source>September</source>
        <translation>Septiembre</translation>
    </message>
    <message>
        <source>October</source>
        <translation>Octubre</translation>
    </message>
    <message>
        <source>November</source>
        <translation>Noviembre</translation>
    </message>
    <message>
        <source>December</source>
        <translation>Diciembre</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
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
        <translation>Saturación:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidad:</translation>
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
        <translation>Mover vector</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Gradiente horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Gradiente vertical</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Gradiente diagonal</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Degradado diagonal cruzado</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Gradiente radial</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Degradado lineal libre</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Degradado radial libre</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Editar propiedades del color de línea</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editar propiedades del color de relleno</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturación del color</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Método de relleno normal o gradiente</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Asigna la transparencia del color seleccionado</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Desplaza el comienzo del vector gradiente mientras presionas el botón izquierdo del ratón y mueve el final del vector gradiente mientras presionas el botón derecho del raton</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Opciones de importación de CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Delimitador de campo:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Delimitador de valor:</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Ningún</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>La primera fila es una cabecera</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Opciones de impresora</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Set de páginas</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Todas las páginas</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Sólo páginas pares</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Sólo páginas impares</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Relejar</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientación</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Impresión de n-hojas</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Página por hoja</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Páginas por hoja</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opción</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Este panel muestra varias opciones de impresión de CUPS. Los parámetros exactos disponibles dependerán del controlador de la impresora. Puedes confirmar el soporte de CUPS seleccionado Ayuda -&gt; Acerca de. C-C-T equivale a C=CUPS C=littlecms T=soporte TIFF. Las bibliotecas no disponibles se indican mediante un *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Comprimir archivo</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Incluir tipografías</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Ir al directorio de documentos.
Se puede configurar en las preferencias.</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Asignar componentes del color</translation>
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
        <translation>Asignar &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Asignar C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Asignar &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Cancelado por el usuario</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Elimiar color</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Eliminar color:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Reemplazar con:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Eliminar páginas</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Eliminar desde:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>a:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Eliminar estilo</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Eliminar estilo:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Reemplazar con:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ningún estilo</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Error al importar</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Error al importar documento Word 
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Información del documento</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Título:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Palabras clave:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Descri&amp;pción:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>E&amp;ditor:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Contribuidores:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>F&amp;echa:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ipo:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormato:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;ficador:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Fuente:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Idio&amp;ma:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relación:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>P&amp;ortada:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Derec&amp;hos:</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Documen&amp;to</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Más &amp;información</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>La persona u organización principal responsable de la creación del contenido de este documento. Este campo puede incrustarse en el documento Scribus como referencia, al igual que en los metadatos de un PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>El nombre del documento. Este campo puede incrustarse en el documento Scribus como referencia, al igual que en los metadatos de un PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Una explicación del contenido del documento. Este campo es para una descripción breve o resumen del documento. Se incrusta en el PDF al exportar</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>El tópico del contenido del documento. Este campo es para las palabras clave del documento que quieras incrustar en el PDF, para ayudar a los buscadores e indizar los archivos PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>La persona u organización responsable de hacer disponible el documento</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>La persona u organización responsable de contribuir al contenido del documento</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Una fecha asociada con un evento en el ciclo de vida del documento, en formato AAA-MM-DD, como indica ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>La naturaleza o género del contenido del documento, p. ej. categorías, funciones, géneros, etc</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>La manifestación física o digital del documento. El tipo de media y las dimensiones no deberían importar. RFC2045 y RFC2046 para tipos MIME también son útiles aquí</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Una referencia única para el documento dentro de un contexto dado como ISBN o URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Una referencia al documento del que el documento presente deriva, p. ej. ISBN o URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>El idioma en el que el contenido del documento está escrito, usualmente un código de idioma ISO-639 opcionalmente seguido de un guión y un código de país ISO-3166, p. ej. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Una referencia a un documento relacionado, posiblemente usando un identificador formal como ISBN o URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>El alcance o ámbito del contenido del documento, posiblemente incuyendo la localización, fecha y rangos de jurisdicción</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Información sobre los derechos aplicables dentro y fuera del documento, p. ej. copyright, patentes o registros</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Añadir una numeración de página al documento. La nueva sección se añadirá tras la sección actual seleccionada.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Eliminar la sección actual seleccionada.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Nombre:&lt;/b&gt;Nombre opcional para la sección, p. ej. Índice&lt;br/&gt;&lt;b&gt;Mostrar:&lt;/b&gt;Seleccionar para mostrar los números de página en esta sección si hay uno o más marcos de textos configurados para ello.&lt;br/&gt;&lt;b&gt;Desde:&lt;/b&gt;El índice de página desde el que comienza esta sección.&lt;br/&gt;&lt;b&gt;A:&lt;/b&gt;El índice de página en el que finaliza esta sección.&lt;br/&gt;&lt;b&gt;Estilo:&lt;/b&gt;Seleccionar el estilo de número de página a utilizar.&lt;br/&gt;&lt;b&gt;Inicio:&lt;/b&gt;El índice dentro del rango de estilos en el que comienza, p. ej. Si Inicio=2 y Estilo=a,b,c, ..., los números comenzarán en b. Para la primera sección del documento, esto reemplaza la antigua numeración de primera página en la ventana de archivo nuevo.</translation>
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
        <translation>Numeración de página fuera de límites</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>El valor introducido está fuera del rango de numeración de páginas en el documento actual (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Secciones de documento</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Mostrar</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Desde</translation>
    </message>
    <message>
        <source>To</source>
        <translation>A</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Estilo</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Comienzar</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
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
        <translation>Ninguna</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Depende de</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Es padre de</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Es hijo de</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Marcos de texto</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Marcos de imagen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Ningún</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Booleano</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Entero</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Número real</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Cadena</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Atributos del objeto de documento</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parámetro</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Relación</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Relacionado con</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Autoañadir a</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
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
        <translation>Configurar impresora</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Destino de impresión</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Opciones...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Archivo:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Comando de impresora a&amp;lternativo</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Co&amp;mando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Imprimir tod&amp;as</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Imprimir pá&amp;gina actual</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Imprimir &amp;rango</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Número de copias:</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Imprimir normal</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Imprimir separaciones</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Imprimir en color si disponible</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Imprimir en escala de grises</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Amarillo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Negro</translation>
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
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Reflejar página/s horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Reflejar página/s verticalmente</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Asignar tamaño de media</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Ajustar a márgenes de página</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Color</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Aplicar eliminación del color subyacente</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convertir colores de tinta plana en colores de proceso</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Aplicar perfiles ICC</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Opciones avanzadas</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Vista previa...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>Im&amp;primir</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>No mostrar objetos fuera de los márgenes en la página impresa</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Insertar una lista de elementos separados por coma
donde un elemento puede ser * para todas las páginas,
1-5 para un rango de página o sólo una página simple.</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Usar un gestor de impresión alternativo, como kprinter o gtkpl, para utilizar opciones de impresión adicionales</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Ajusta el nivel de PostScript.
 Configurando a nivel 1 ó 2 se pueden crear archivos enormes</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Un método de evitar algunas de las sombras grises compuestas de cian, amarillo y magenta y utilizar sólo negro. UCR afecta principalmente a las parte sde las imágenes que son neutras y/o de tonos oscuros próximos al gris. Utilizar esta opción puede mejorar la impresión de algunas imágenes y es necesario experimentar y pruebas caso por caso. UCR reduce la posibilidad de sobresaturación con tintas CMY.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Activa la conversión de colores de tinta plana a colores de composición. A menos que esté planeando imprimir colores de tinta plana en una impresora comercial, es probablemente mejor dejarlo activado.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Te permite incrustar perfiles ICC en el flujo de impresión cuando el gestor de color está activado</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Te permite configurar explícitamente el tamaño de media del archivo PostScript. No se recomienda utilizar a menos que sea requerido por el impresor.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Error al obtener las configuraciones de la impresora</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Archivos PostScript (*.ps);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing PostScript</source>
        <translation>Importando PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Analizando PostScript:</translation>
    </message>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>¡Error al importar archivo:
%1!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Error fatal</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Generando objetos</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>¡Error al convertir %1 imágenes!</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Error</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar estilo</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Carácter</translation>
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
        <translation>Distancias</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Interlineado ajustado</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Interlineado automático</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Alinear a la cuadrícula de la línea de base</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Letra capital</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Líneas:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Distancia al texto:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabuladores y sangría</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Vista previa del estilo de párrafo</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Seleccione para una lectura más adecuada de los estilos con color de texto claro</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Nombre del estilo de párrafo</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Tipografía del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de tipografía</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color del relleno de texto</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Determina la altura total, en números de línea, de la letra capital</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Determina el espaciado entre la letra capital y el texto</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Espaciado por encima del párrafo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Espaciado por debajo del párrafo</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Interletrado manual</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Desplazamiento a la línea de base de caracteres</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Interlineado</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Des/activa el texto de ejmplo de este estilo de párrafo</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>El nombre del estilo no es único</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Clic y mantén y para seleccionar el modo de interlineado.</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
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
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Guardar c&amp;omo...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Guardar y salir</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Salir sin guardar</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rehacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Co&amp;rtar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>O&amp;btener nombres de campos</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Efectos de imagen</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Saturación:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Brillo:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Contraste:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Radio:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Valor:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterizar:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Efectos disponibles</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Desenfocar</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Brillo</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Colorear</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Contraste</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Escala de grises</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertir</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterizar</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Enfocar</translation>
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
        <translation>Efectos en uso</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Elige un directorio para exportar</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportar como imagen/es</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportar al directorio:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Tipo de imagen:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Calidad:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolución:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
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
        <translation>Rango</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Página a&amp;ctual</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Tod&amp;as las páginas</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Rango</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportar rango de páginas</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Insertar una lista de elementos separados por coma
donde un elemento puede ser * para todas las páginas,
1-5 para un rango de página o sólo una página simple.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportar todas las páginas</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportar sólo la página actual</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolución de las imágenes
Usar 72 dpi para imágenes destinadas a la pantalla</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Tamaño de las imágenes. 100% para no cambiar, 200% para dos veces mayor etc.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>La calidad de las imágenes - 100% para la mejor, 1% para la menor calidad</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formatos de exportación disponibles</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>El directorio de salida - el lugar donde se almacenan las imágenes.
El nombre del archivo exportado será &apos;nombredeldocumento-númerodepágina.tipodearchivo&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Cambiar el directorio de salida</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Propiedades de imagen adicionales</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Oscurecer</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Aclarar</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Tono</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Color</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Luminosidad</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Multiplicar</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Pantalla</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Disolver</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Superponer</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Luz dura</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Luz suave</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Diferencia</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Exclusión</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Sobreexponer color</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Subexponer color</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Exclusión</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Modo de fusión:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Capas</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>No usar ningún trazado</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Trazados</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Resolución:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
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
        <translation>Escala de grises</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Espacio de color:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Sin título</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Documento Scribus</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Versión:</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Algunas tipografías usadas en este documento han sido sustituidas:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> fue reemplazada por: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Tipografías disponibles</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Sustituciones de tipografías</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Rutas adicionales</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Nombre de tipografía</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Utilizar tipografía</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Incustrada:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Subset</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Ruta al archivo de tipografía</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Tipogr&amp;afías disponibles</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nombre de la tipografía</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Reemplazo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Sustituciones de tipografías</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>Aña&amp;dir...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Elimina&amp;r</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Rutas adicionales</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation>Las rutas de búsqueda de tipografías sólo pueden configurarse cuando no hay documentos abiertos. Cierre cualquier documento abierto, después ejecute Archivo &gt; Preferencias &gt; Tipografías  para cambiar la ruta de búsqueda de tipografías.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña tocaba el saxofón detrás del palenque de paja. 1234567890</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Añadir la tipografía seleccionada en el menú Estilo y Tipografía</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Abandonar vista previa</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Escribiendo el texto aquí permite una búsqueda rápida de los nombres de las tipografías. La búsqueda no distingue entre mayúsculas y minúsculas. Puedes utilizar comodies (*, ?, [...]) en la frase. Ejemplos: t* listará todas las tipografías que comiencen con t o T. *fe* listará todas las tipografías con la palabra fe, feliz, etc. en el nombre.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Comenzar búsqueda</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Tamaño de la tipografía seleccionada</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>El ejemplo se mostrará al soltar la tecla</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Usuario</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Sistema</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>Bús&amp;queda rápida:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Bu&amp;scar</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nombre de tipografía</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Subset</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Acceso</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Tamaño de tipogra&amp;fía:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Texto de ejemplo a mostrar</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>Asi&amp;gnar</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Reiniciar el texto</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Vista previa de la tipografía</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>Vista previa de la tipogra&amp;fía...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Diálogo de vista previa de la tipografía</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Ordenar, buscar y navegar entre las tipografías disponibles.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Sustitución de tipografía</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Este documento contiene algunas tipografías que no están instaladas en el sistema. Por favor, elige un reemplazo adecuado para ellas. Cancelar detendrá la carga del documento.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Tipografía original</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Tipografía de sustitución</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Hacer estas sustituciones permanentes</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Cancela la sustitución de tipografías y detiene la carga del documento.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Activar esta opción, permite a Scribus utilizar estas sustituciones para tipografías no disponibles en futuras disposiciones. Se puede revertir o cambiar en Editar &gt; Preferencias &gt; Tipografías.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Si selecciona Aceptar, y guarda, estas sustituciones serán permanentes en el documento.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Añadir, cambiar o eliminar puntos de color aquí</translation>
    </message>
    <message>
        <source>Position:</source>
        <translation>Posición:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Gestor de guías</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Guías horizontales</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Unidad</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posición &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Eliminar</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Guías verticales</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Aña&amp;dir</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>E&amp;liminar</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Filas y columnas - guías automáticas</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Filas:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>C&amp;olumnas:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>Despla&amp;zamiento de filas</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Desplazamiento de colum&amp;nas</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>Referido a:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Página</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Márgenes</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Selección</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>Act&amp;ualizar</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vista previa</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>B&amp;loquear guías</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;Aplicar a todas las páginas</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Asignar las guías del documento. El gestor de guía todavía está abierto pero los cambios son persistentes</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Ya hay una guía vacía (0.0)</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Contenidos</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Contenidos</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Bu&amp;scar</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Relevancia</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Busc&amp;ar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Bo&amp;rrar todo</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Marcadores</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Im&amp;primir...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Buscar...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Buscar siguie&amp;nte</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Buscar an&amp;terior</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Añadir marcador</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>&amp;Eliminar todo</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>Marca&amp;dores</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Ayuda en línea de Scribus</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>La búsqueda no distingue entre mayúsculas y minúsculas</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>¡Lo siento, no hay manual disponible! Por favor, ver: http://docs.scribus.net para la documentación
actualizada y www.scribus.net para descargas.</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Término de búsqueda:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nuevo marcador</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Nuevo título del marcador:</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Posible división de palabras</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Ignorar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Sugerencias de &amp;división por palabras</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Dividir texto por sílabas automáticamente &amp;durante la escritura</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Idio&amp;ma:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Palabra má&amp;s corto:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Divisiones por síl&amp;abas consecutivas permitidas:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Una caja de diálogo mostrando todas las posibles divisiones de cada palabra se mostrará cuando use la opción Extras &gt; División de texto por sílabas.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Activar división automática por sílabas del texto mientras escribes.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Longitud de la palabra más pequeña que puede ser dividida por sílabas.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Número máximo de divisiones por sílabas consecutivas.
Un valor de 0 significa divisiones ilimitadas.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Información de imagen</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Información general</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Fecha / Hora:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Tiene un perfil incrustado:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Nombre del perfil:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Tiene trazados incrustados:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Tiene capas:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>Información EXIF</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Artista:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Comentario:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Comentario del usuario:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Modelo de cámara:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Fabricante de la cámara:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Modelo de escáner:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Fabricante del escáner:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importar &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importa archivos EPS</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importa la mayoría de los archivos EPS en el documento
actual, convirtiendo los datos vectoriales en objetos Scribus.</translation>
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
        <translation>Insertar página</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insertar</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Página/s</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes de página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>tras página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>al final</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Páginas maestras</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>Página &amp;maestra:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Mover objetos con su página</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Insertar tabla</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Número de filas:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Número de columnas:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Edotar JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Editar...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Añadir...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Añade un nuevo script, predefine una función con el mismo nombre. Si quieres usar este script como una &quot;Acción abierta&quot; asegúrate de no cambiar el nombre de la función.</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nuevo script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nuevo script</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>¿Realmente quieres eliminar este script?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Tecla actual</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Selecciona una tecla para esta acción</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ninguna tecla</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Tecla definida por el &amp;usuario</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+MAYÚS+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Asignar te&amp;cla</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Sets de teclas rápidas para cargar</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Cargar</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importar...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Exportar...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Reiniciar</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Sets de teclas rápidas de teclado disponibles para cargar</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Cargar el set de teclas rápidas seleccionado</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importar un set de teclas rápidas en la configuración actual</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Exportar las teclas rápidas actuales en un archivo importable</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Recargar las teclas rápida por defecto de Scribus</translation>
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
        <translation>Mayús</translation>
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
        <translation>Mayús+</translation>
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
        <translation>Esta secuencia de teclas ya está en uso</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Archivos set de teclas XML (*.ksxml)</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Eliminar capa</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>¿Realmente quieres eliminar todos los objetos en esta capa también?</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Capas</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Añadir nueva capa</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Eliminar capa</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Subir capa</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Bajar capa</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Editar líneas de estilo</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo estilo</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>¿Realmente quieres eliminar este estilo?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos los documentos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ancho de línea:</translation>
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
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Línea sólida</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Línea a rayas</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Línea a puntos</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Línea raya punto</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Línea punto punto raya</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Seleccionar Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Obtener más:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>Archivo XML:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Párrafos:</translation>
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
        <translation>Gestionar propiedades de página</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Mover objetos con su página</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tipo:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de margen</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>Preset Layouts:</source>
        <translation>Disposiciones predefinidas:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>I&amp;zquierda:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Aplicar configuraciones de margen a todas las páginas</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Aplicar los cambios de margen a todas las páginas existentes en el documento</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Márgenes de impresora...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importar los márgenes para el tamaño de página seleccionado de las impresoras disponibles.</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distancia entre la guía del inferior superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen izquierdo y el borde de la página. Si está activado páginas enfrentadas, este espacio de margen se puede utilizar para conseguir los márgenes correcto para la encuadernación</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distancia entre la guía del margen derecho y el borde de la página. Si está activado páginas enfrentadas, este espacio de margen se puede utilizar para conseguir los márgenes correcto para la encuadernación</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Exterior:</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Editar páginas maestras</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Duplicar la página maestra seleccionada</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Eliminar la página maestra seleccionada</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Añadir una nueva página maestra</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Importar páginas maestras de otro documento</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>¿Realmente quieres eliminar esta página maestra?</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Nueva página maestra</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Nueva página maestra</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nueva página maestra %1</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>Imposible renombrar página maestra</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>No está permitido renombrar la página normal.</translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>Renombrar página maestra</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>Nuevo nombre:</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Varias copias</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Número de copias:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Desplazamiento &amp;horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Desplazamiento vertical:</translation>
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
        <translation>Distancias</translation>
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
        <translation>Ángulo:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Longitud:</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Master Page</source>
        <translation>Insertar página maestra</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar página/s</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Desde el documento:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importar página/s:</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importar página maestra</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Insertar una lista de elementos separados por coma donde un elemento puede ser * para todas las páginas, 1-5 para un rango de página o solo un número de página.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> desde 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Crear página/s</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Antes de página</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Después de página</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Al final</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos los documentos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> desde %1</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Tipografía no disponible</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>La tipografía %1 no está instalada.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Utilizar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>en vez de</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Mover páginas</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copiar página</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Mover página/s</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>A:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Número de copias:</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Antes de página</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Después de página</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Al final</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Mover página/s:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Interlineado ajustado</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Interlineado automático</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Alinear a la cuadrícula de la línea de base</translation>
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
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posición &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Distancia entre columnas</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Ancho de columna</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ningún estilo</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.&lt;br/&gt;Por favor, elige otro.</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Texto</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Imagen</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Línea</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Colores</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometría</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Punto base:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editar forma...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Esquinas
red&amp;ondeadas:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distancia del texto</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Espaciado:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>Su&amp;perior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>I&amp;zquierda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abuladores...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Propiedades del texto sobre trazado</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Mostrar curva</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Desplazamiento inicial:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distancia a la curva:</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Regla de relleno</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Par-impar</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Distinta de cero</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>El texto &amp;fluye alrededor del marco</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Usar cuadro circ&amp;unscrito</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Usar línea de contorno</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Est&amp;ilo:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>I&amp;dioma:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>E&amp;scalado libre</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>X DPI actuales:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Y DPI actuales:</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Esc&amp;ala X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Escala Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Escalar al &amp;tamaño del marco</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roporcional</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Perfil de entrada:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Prueba de renderizado:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimetría relativa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimetría absoluta</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Punto izquierdo</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Extremos</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>Punto de &amp;base:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;ipo de línea:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Flecha inicial:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Flecha final:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>A&amp;ncho de línea:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Es&amp;quinas:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Terminaciones:</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Bordes de celda</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Línea superior</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Línea a la izquierda</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Línea a la derecha </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Línea inferior</translation>
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
        <translation>Nombre del objeto seleccionado</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Posición horizontal del punto de base actual</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Posición vertical del punto de base actual</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Ancho</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Alto</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotación del objeto respecto al punto de base actual</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punto origen de medidas y ángulos de rotación</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Selecionar parte superior izquierda como punto de base</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Selecionar parte superior derecha como punto de base</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Selecionar parte inferior izquierda como punto de base</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Selecionar parte inferior derecha como punto de base</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Selecionar parte central como punto de base</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Subir un nivel</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Bajar un nivel</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Enviar al frente</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Enviar al fondo</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indica el nivel en el que el objeto se encuentra, 0 significa que el objeto está al fondo</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Bloquear o desbloquear el objeto</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Bloquear o desbloquear el tamaño del objeto</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation>Hacer que el texto en los marcos inferiores fluya alrededor del objeto. Las opciones siguientes define cómo se realiza esto.</translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Usar el cuadro circunscrito, que es siempre rectancular, en vez de la forma del marco para que el texto fluya por los marcos de texto inferiores al objeto. </translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Usar una segunda línea basada originalmente en la forma del marco para que el texto fluya  por los marcos de texto inferiores al objeto. </translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Tipografía del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de tipografía</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Desplazamiento a la línea de base de caracteres</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar la anchura de los caracteres</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Escalar la altura de los caracteres</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Color del trazo del texto y/o sombreado, dependiendo de lo elegido. Si ambos son elegidos, comparten el mismo color.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Color del texto seleccionado. Si la decoración del contorno de texto está activada, este color será el color de relleno. Si el sombreado de texto está activado, este color será el color principal.</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color del trazo del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color del relleno de texto</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Escritura de derecha a izquierda</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Interletrado manual</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Interlineado</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo actual</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Idioma de división por sílabas del marco</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Cambiar configuración de punto izquierdo y extremos</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Patrón de línea</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Grosor de línea</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipo de uniones de las líneas</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipo de extremos</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Estilo de línea del objeto actual</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Elige la forma del marco...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editar forma del marco...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Definir el radio de las esquinas redondeadas</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Número de columnas en el marco de texto</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Cambia entre espaciado o anchura de las columnas</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distancia del texto al la parte superior del marco</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distancia del texto a la parte inferior del marco</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distancia del texto a la parte izquierda del marco</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distancia del texto a la parte derecha del marco</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editar configuración de pestañas del marco de texto...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Permite que la imagen sea de un tamaño distinto al del marco</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Desplazamiento horizontal de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Desplazamiento vertical de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionar imagen horizontalmente</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionar imagen verticalmente</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Mantener la misma escla en X e Y</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Mantener el ratio de aspecto</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Ajustar la imagen al tamaño del marco</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Usar las proporciones de la imagen en vez de las del marco</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Perfil de origen de la imagen</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Prueba de renderizado para la imagen</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Clic y mantén y para seleccionar el modo de interlineado.</translation>
    </message>
    <message>
        <source>Enable or disable exporting of the object</source>
        <translation>Activar o desactivar la exportación del objeto</translation>
    </message>
    <message>
        <source>Arrow head style for start of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrow head style for end of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effective horizontal DPI of the image after scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effective vertical DPI of the image after scaling</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar estilo</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ancho de línea:</translation>
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
        <source>Solid Line</source>
        <translation>Línea continua</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Línea a rayas</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Línea a puntos</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Línea raya punto</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Línea punto punto raya</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>El nombre &quot;%1&quot; no es único.&lt;br/&gt;Por favor, elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>pt</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Progreso</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Progreso total:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Mi e&amp;xtensión</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Mi extensión</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>¡La extensión funciona!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nuevo documento</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nuevo documento</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Abrir documento &amp;existente</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Abrir &amp;documento reciente</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>No mostrar más este diálogo</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Tamaño de página del documento, tamaño estándar o personalizado</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientación de las páginas del documento</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas del documento, editable si has seleccionado tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas del documento, editable si has seleccionado un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Número inicial de páginas del documento</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medidas por defecto para la edición del documento</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Crear marcos de texto automáticamente cuando se añadan nuevas páginas</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Número de columnas a crear en los marcos de texto creados automáticamente</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Distancia entre las columnas creadas automáticamente</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de margen</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Nú&amp;mero de páginas:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Unidad por &amp;defecto:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Marcos de texto &amp;automáticos</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Espaciado:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nuevo &amp;desde plantilla...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Cargar documentos con disposición predefinida</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Crear un documento desde una plantilla hecha por otros usuarios o por ti mismo (p. ej. para documentos con un estilo contante).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodos</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Coordenadas &amp;absolutas</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posición &amp;Y:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Editar línea de &amp;contorno</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Reiniciar línea de contorno</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>T&amp;erminar edición</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Mover nodos</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Mover puntos de control</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Añadir nodos</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Eliminar nodos</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Mover puntos de control independientemente</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Mover puntos de control simétricamente</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Reiniciar este punto de control</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Abrir un polígono o cortar una curva Bézier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Cerrar esta curva Bézier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Reflejar el trazado horizontalmente</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Reflejar el trazado verticalmente</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Cizallar horizontalmente el trazado a la derecha</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Cizallar horizontalmente el trazado a la izquierda</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Cizallar verticalmente el trazado hacia arriba</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Cizallar verticalmente el trazado hacia abajo</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotar el trazado en sentido antihorario</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotar el trazado en sentido horario</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Contraer el tamaño del trazado por el % mostrado</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Aumentar el tamaño del trazado por el % mostrado</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Reducir el tamaño del trazado por el valor mostrado</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Aumentar el tamaño del trazado por el valor mostrado</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Ángulo de rotación</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>% a aumentar o contraer</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Valor para aumentar o contraer</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Activar modo de edición de la línea de contorno</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Reiniciar la línea de contorno a la forma original del marco</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Si está marcada esta opción, usar coordenadas relativas a la página, si no, las coordenadas son relativas al objeto.</translation>
    </message>
    <message>
        <source>to Canvas</source>
        <translation>al lienzo</translation>
    </message>
    <message>
        <source>to Page</source>
        <translation>a la página</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Este documento no parece ser un archivo OpenOffice.org Draw.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importar &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importa archivos OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importa la mayoría de archivos OpenOffice.org Draw en el documento actual, convirtiendo los datos vectoriales en objetos Scribus.</translation>
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
        <translation>Este archivo contiene algunas características no soportadas</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Opciones de importación de OpenDocument</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Sobreescribir estilos de párrafo</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Al activar esta opción, se sobreescribirán los estilos existentes en el documento Scribus actual</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionar estilos de párrafo</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionar estilos de párrafo por atributos. Producirá menos estilos de párrafo similares, reteniendo los atributos de estilo, incluso si los estilos del documento original tienen nombres diferentes.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Usar el nombre del documento como prefijo de los estilos de párrafo</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Anteponer el nombre del documento al nombre del estilo de párrafo en Scribus.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>No volver a preguntar</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Hacer esta configuración la configuración por defecto y no volver a preguntar otra vez al importar un OASIS OpenDocument.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Documento Scribus</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Documento Scribus 1.2.x</translation>
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
        <translation>Ancho de línea</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Guardar como PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>E&amp;xportar a archivo:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ca&amp;mbiar...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Exportar un arc&amp;hivo por cada página</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Esta opción permite exportar un archivo PDF de nombre único por cada página del documento. Los números de página son añadidos automáticamente. Esto es más útil para la imposición de PDF para impresiones comerciales.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>El botón guardar se desactivará si intenta exportar a PDF/X-3 y la cadena información no está presente en la pestaña PDF/X-3.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Guardando PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Exportando páginas maestras:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Exportando páginas:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Exportando objetos de la página actual:</translation>
    </message>
    <message>
        <source>A write error occured, please check available disk space</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Vista previa de impresión</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation>&amp;Activar suavizado</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Mostrar trans&amp;parencia</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Eliminación de color s&amp;ubyacente</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>M&amp;ostrar CMYK</translation>
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
        <translation>Nombre de separación</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Amarillo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Negro</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Escala:</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Imprimir...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Provee una vista más agradable de las tipografías Type1, TrueType y OpenType, EPS, PDF y los gráficos vectoriales en la vista previa, a costa de una pequeña ralentización en la vista previa</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Muestra la transparencia y los objetos trasparente del documento. Requiere Ghostscript 7.07 ó superior</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Muestra una vista previa de impresión simulando las tintas CMYK genéricas, en vez de colores RGB</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Un método de evitar algunas de las sombras grises compuestas de cian, amarillo y magenta y utilizar sólo negro. UCR afecta principalmente a las parte sde las imágenes que son neutras y/o de tonos oscuros próximos al gris. Utilizar esta opción puede mejorar la impresión de algunas imágenes y es necesario experimentar y pruebas caso por caso. UCR reduce la posibilidad de sobresaturación con tintas CMY.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Redimensionar la escala de la página.</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Des/activar el plato de tinta C (cian)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Des/activar el plato de tinta M (magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Des/activar el plato de tinta Y (amarillo)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Des/activar el plato de tinta K (negro)</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Procesando páginas maestras:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Exportando páginas:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Imagen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polilínea</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Trazado de texto</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copia de</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Ninguna</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Depende de</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Es padre de</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Es hijo de</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Atributos de objetos de la página</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parámetro</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Relación</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Relacionado con</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>First Page is:</source>
        <translation>La primera página es:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Disposición del documento</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Doble cara</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Página central derecha</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Arrastrar páginas o páginas maestra en la papelera de reciclaje para eliminarlas</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Aquí están todas las páginas maestras. Para crear una nueva página, arrastra una página maestra a la vista de página siguiente</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Ordenar páginas</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Páginas maestras disponibles:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Páginas del documento:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %1</source>
        <translation>%1 de %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 de %2</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation>Cuarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation>Carta gubernamental</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Oficio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Libro de contabilidad</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Ejecutivo</translation>
    </message>
    <message>
        <source>Post</source>
        <translation></translation>
    </message>
    <message>
        <source>Crown</source>
        <translation></translation>
    </message>
    <message>
        <source>Large Post</source>
        <translation>Post grande</translation>
    </message>
    <message>
        <source>Demy</source>
        <translation></translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Media</translation>
    </message>
    <message>
        <source>Royal</source>
        <translation></translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation></translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation></translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation></translation>
    </message>
    <message>
        <source>STMT</source>
        <translation></translation>
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
        <translation>Reesultados</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Buscar resultados para: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vista previa</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Seleccionar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Ruta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Estado</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>No disponible</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Cancelar búsqueda</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Seleccionar un directorio base para búsqueda</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Búsqueda de imagen</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Error en la búsqueda: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>No se encontró ninguna imagen llamada &quot;%1&quot;.</translation>
    </message>
    <message>
        <source>Manage Images</source>
        <translation>Organizar imágenes</translation>
    </message>
    <message>
        <source>Go to</source>
        <translation>Ir a</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Guardar como &amp;imagen...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportar como imagen</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exporta las páginas seleccionadas como imágenes de mapas de bits.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>No se pudo encontrar la extensión</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>error desconocido</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>No se pudo encontrar el símbolo (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Extensión: cargando %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>error al iniciar</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>tipo de extensión desconocido</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Extensión: %1 cargado</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Extensión: error al cargar %1: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Gestor de extensiones</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Extensión</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Cómo ejecutar</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>¿Cargarlo?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>ID de extensión</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Necesitas reiniciar la aplicación para aplicar los cambios.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Propiedades del polígono</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Esquinas:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplicar &amp;factor</translation>
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
        <translation>Número de esquinas para los polígonos</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grados de rotación para los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplicar factor de convexidad/concavidad para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Un valor negativo creará un polígono cóncavo (o con forma de estrella), y un valor positivo lo creará convexo</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Preferencias</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>IGU</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>Idio&amp;ma:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>Tamaño de tipogra&amp;fía (menús):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Tamaño de tipografía (&amp;paletas):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Desplazamiento de rueda:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Documentos &amp;recientes:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Rutas</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Documentos:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Cambiar...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>Perfiles &amp;ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>C&amp;ambiar...</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Plantillas d&amp;e documento:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ca&amp;mbiar...</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntación:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Unidades:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de margen</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autoguardado</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervalo:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Deshacer/Rehacer</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Longitud del historial de acciones</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografía</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>División por sílabas</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Tipografías</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Verificación previa</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Gestión de color</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Exportar a PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Atributos de objetos del documento</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabla de contenido e índices</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Teclas rápidas</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Visionado de página</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Mostrar &amp;el área no imprimible con el color del margen</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Mostrar enlace de texto</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Mostrar caracteres de control de texto</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Mostrar marcos</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Reglas relativas a la página</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Área de trabajo</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>I&amp;zquierda:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Espaciado entre páginas</translation>
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
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Ajustar tamaño de visionado</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Para ajustar el visionado, arrastre la regla inferior con el desplazador.</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visor</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Intérprete PostScript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Nombre del ejecutable:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Suavizar &amp;texto</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Suavizar &amp;gráficos</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Resolución:</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Herramienta de procesamiento de imágenes</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Nombre del &amp;ejecutable:</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Herramientas externas</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Preguntar siempre antes de reemplazar tipografías al cargar un documento</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Vista previa del estilo de párrafo actual visible al editar estilos</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Mostrar pantalla de inicio</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Usar siempre el Lorem Ipsum estándar</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Párrafos totales:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Miscelánea</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Extensiones</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Activar o desactivar  el visionado de marcos enlazados.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Mostrar caracteres no imprimisble como los marcadores de párrafo en los marcos de texto</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Des/activa el visionado de los marcos</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Seleccionar el idioma por defecto para Scribus. Dejar este campo en blanco para eligir en base a las variables del sistema. Puedes sobreescribir esta opción utilizando una opción en la línea de comandos al iniciar Scribus</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Elige la decoración de ventana y apariencia por defecto. Scribus hereda cualquier tema KDE o Qt disponible, si Qt está configurado para buscar extensiones KDE.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Tamaño de tipografía por defecto para menús y ventanas</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Tamaño de tipografía por defecto para las ventanas de herramientas</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medidas por defecto para la edición del documento</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Número de líneas que Scribus desplazará por cada movimiento de la rueda del ratón</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Número de documentos editados recientemente que se mostrarán en el menú Archivo</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Directorio de documentos por defecto</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Directorios de perfiles ICC por defecto. No se puede modificar con un documento abierto. Por defecto, Scribus buscará en los directorios del sistema bajo Mac OS X y Windows. En Linux y Unix, Scribus buscará en $home/.color/icc,/usr/share/color/icc y /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Directorio de scripts de Scripter por defecto</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Directorio adicional para plantillas de documento</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Tamaño de página por defecto, tamaño estándar o personalizado</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientación por defecto para las páginas del documento</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas del documento, editable si has elegido un tamaño de páginas personalizado</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas del documento, editable si has elegido un tamaño de páginas personalizado</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Cuando está activado, Scribus guarda una copia de seguridad del archivo con la extensión .bak cada cierto intervalo de tiempo</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Periodo de tiempo para el autoguardado</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Asignar la longitud del historial de acciones en pasos. Si se asigna 0, se almacenarán un número infinito de acciones.</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Color del papel</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Enmascarar el área fuera de los márgenes con el color de margen</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Asignar el nivel de zoom por defecto</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Coloca una regla en la pantalla y rastra el desplazador para ajustar el nivel de zoom para que Scribus muestre las páginas y objetos en ella al tamaño correcto</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation>Añadir la ruta del intérprete Ghostscript. En Windows, por favor, recuerde que necesita utilizar el programa llamado gswin32c.exe - NO gswin32.exe. Si no, podria bloqueo durante el inicio de Scribus.</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Suaviza el texto al renderizar EPS y PDF en la pantalla</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Suaviza los gráficos al renderizar EPS y PDF en la pantalla</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Ubicación en el sistema de archivos del editor de gráficos. Si usas gimp y tu distribución lo incluye, se recomienda utilizar &apos;gimp-remote&apos; pues permite editar las imágenes en una instancia de gimp en ejecución.</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Define la cantidad de espacio libre a la izquierda en el lienzo del documento disponible como un espacio de intercambio para la creación y modificación de objetos y arrastrarlos en la página activa</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Define la cantidad de espacio libre a la derecha en el lienzo del documento disponible como un espacio de intercambio para la creación y modificación de objetos y arrastrarlos en la página activa</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Define la cantidad de espacio libre en la parte superior del lienzo del documento disponible como un espacio de intercambio para la creación y modificación de objetos y arrastrarlos en la página activa</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Define la cantidad de espacio libre en la parte inferior del lienzo del documento disponible como un espacio de intercambio para la creación y modificación de objetos y arrastrarlos en la página activa</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Ubicar Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Ubicar el editor de imágenes</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation>Mostrar imágenes</translation>
    </message>
    <message>
        <source>Turns the display of images on or off</source>
        <translation>Des/activa el visionado de imágenes</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>Save...</source>
        <translation>Guardar...</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>Por &amp;defecto</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Guardar preferencias</translation>
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
        <translation>¿Migrar antigua configuración de Scribus?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus ha detectado archivos de preferencias de Scribus 1.2 existentes.
¿Quiere migrarlos a la nueva versión de Scribus?</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>No se pudo abrir el archivo de preferencias &quot;%1&quot; como escritura: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Error al escribir el archivo de preferencias &quot;%1&quot;: QIODevice código de estado %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Error al abrir el archivo de preferencias &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Error al leer el XML de preferencia de &quot;%1&quot;: %2 en línea %3, columna %4</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>Error al escribir las preferencias</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus no fue capaz de guardar las preferencias:&lt;br&gt;%1&lt;br&gt;Por favor, compribue los permisos del archivo y del directorio y el espacio disponible en disco.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Error al cargar las preferencias</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus no fue capaz de cargar las preferencias:&lt;br&gt;%1&lt;br&gt;Se cargará la configuración por defecto.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
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
        <translation>Número áureo</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Noveno</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Puedes selecciona una disposición de página predefinida aquí. &apos;Ninguno&apos; deja los márgenes tal cual, Gutenberg configura los márgenes de forma clásica, &apos;Magazine&apos; configura todos los márgenes al mismo valor. La encuadernación es el valor Derecha/Interior.</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Guardar c&amp;omo...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Ejecutar</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Ejecutar en &amp;consola</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>Guardar &amp;salida...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Consola Python de Scribus</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Consola de Script</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Escribe los comandos aquí. Una selección se procesará como script</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Salida del script</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Scripts de Python (*.py)</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Abrir archivo script de Python</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Guardar los comandos Python en archivo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Archivos de texto (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Guardar salida actual</translation>
    </message>
    <message>
        <source>This is a standard Python console with some 
known limitations. Please consult the Scribus 
Scripter documentation for futher information. </source>
        <translation>Esta es una consola Python estándar con algunas 
limitaciones conocidas. Por favor, consulte la docu- 
mentación de Scribus Script para más información. </translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>T&amp;ono:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Valor:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rojo:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Verde:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Az&amp;ul:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal al&amp;fa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Colores &amp;básicos</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Colores personali&amp;zados</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definir colores personalizados &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Añadir a colores personalizados</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Seleccionar color</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copiar o mover un archivo</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Leer: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Escribir: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nombre de archivo:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tipo de archivo:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Subir un directorio</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Buscar &amp;en:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Atrás</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Crear nueva carpeta</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Vista en lista</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Vista en detalle</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Vista previa de información de archivo</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Vista previa de contenidos de archivo</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lectura-escritura</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Sólo lectura</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Sólo escritura</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inaccesible</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Enlace simbólico a archivo</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Enlace simbólico a directorio</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Enlace simbólico a especial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Directorio</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Especial</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renombrar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>R&amp;ecargar</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Ordenar por &amp;nombre</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Ordenar por ta&amp;maño</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Or&amp;denar por fecha</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>Desordenad&amp;o</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Ordenar</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Mostrar arc&amp;hivos ocultos</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>el archivo</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>el directorio</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>el enlace simbólico</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Eliminar %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;¿Está seguro de querer eliminar %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nueva carpeta 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nueva carpeta</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nueva carpeta %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Buscar directorio</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Directorios</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Guardar</translation>
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
Archivo no encontrado.
Compruebe la ruta y el nombre dle archivo.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Todos los archivos (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Seleccionar un directorio</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Directorio:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>Tipogra&amp;fía</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Est&amp;ilo de tipografía</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efectos</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Tac&amp;hado</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>S&amp;ubrayado</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Color</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Muestra</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Índ&amp;ice</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Seleccionar tipografía</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Limpiar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rehacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Co&amp;rtar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Preparar</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Personalizar...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Acerca de Qt&lt;/h3&gt;&lt;p&gt;Este programa utilizar Qt versión %1.&lt;/p&gt;&lt;p&gt;Qt es un kit de herramientas en C++ para desarrollo de aplicaciones y IGU multiplataformas.&lt;/p&gt;&lt;p&gt;Qt provee de portabilidad con un único código fuente entre MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, y la mayoría de variantes comerciales de Unix.&lt;br&gt;Qt también está disponible para dispositivos empotrados.&lt;/p&gt;&lt;p&gt;Qt es un producto de Trolltech. Ver &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; para más información.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importando texto</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikáans</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Árabe</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanés</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Vasco</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Búlgaro</translation>
    </message>
    <message>
        <source>Breton</source>
        <translation>Bretón</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalán</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chino</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Croata</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Checo</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danés</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandés</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglés</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Inglés (británico)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation>Estonio</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemán</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Alemán (tradicional)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandés</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francés</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Gallego</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griego</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesio</translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>Islandés</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>Japonés</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Coreano</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxemburgués</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Noruego (bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Noruego (nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Noruego</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polaco</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugués</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugués (Brasil)</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruso</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Sueco</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Español</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Español (latino)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Eslovenio</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbio</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Tailandés</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turco</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ucraniano</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Galés</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Error de Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Error de Scribus debido a la señal  #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Error de Scribus debido a la siguiente excepción: %1</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Página maestra </translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
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
        <translation>No se pudo abrir el archivo de salida %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>El flujo de salida no es de escritura</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Error en la verificación de configuración: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>No se pudo abrir el archivo de entrada %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Imposible leer la configuración XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (línea %2 columna %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Imposible leer la configuración XML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>nodo inicial vacío</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; inválido</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>encontrado %1 &lt;%2&gt; nodos, se necesita 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>nodo &lt;%2&gt; vacío inesperado</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>el nodo &lt;%1&gt; no es un objeto</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>el objeto &lt;%1&gt; no tiene le atributo &apos;value&apos;</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>el valor del elemento &lt;%1&gt;  debe ser &apos;true&apos; o &apos;false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>el elemento &lt;lpiSettingsEntry&gt; no tiene el atributo &apos;name&apos;</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Exportando archivo PostScript</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Imprimiendo archivo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Negro</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Amarillo</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>La biblioteca freetype2 no está disponible</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>La tipografía %1 está dañada, no se incrustará</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>La tipografía %1 está dañada (flujo de lectura), no se incrustará</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>La tipografía %1 está dañada (FreeType2), descartando</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>La tipografía %1 está dañada (sin familia), descartando</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>La tipografía %1 tiene dañado el carácter %2 (código de carácter %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>La tipografía %1 está dañada y será descartada</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>La tipografía %1 no se pudo leer, no se incustrará</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Creando caché de tipografías</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>La tipografía %1 está dañada, descartando</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Error al carga tipografía %1 - tipo de tipografía desconocido</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Nueva tipografia encontrada, comprobando...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Encontrada tipografía modificada, comprobando...</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Tipografia %1 cargada desde %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Tipografía %1(%2) es un duplicado de %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Cargando tipografía %1 (encontrada usando fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Error al cargar tipografía - freetype2 no pudo encontrar el archivo de tipografía</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Leyendo caché de tipografías</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Escribiendo caché de tipografías actualizada</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Buscando tipografías</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>La tipografía %1 está dañada (FreeType), descartando</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>La tipografía %1  tiene dañado el carácter %2 (código de carácter %3), descartando</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished">extrayendo familia %1 de la tipografía %2 (desplazamiento=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished">cabecera memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabla &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished">tabla memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished">desplazamiento memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 podría estar dañado : etiquetas de resolución no disponibles</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation>%1 podría estar dañado : etiquetas de resolución no disponibles o dañadas</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Inicializando...</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Versión de desarrollo de Scribus</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Está ejecutando una versión de desarrollo de Scribus 1.3.x. El documento con el que está trabajando fue creado con Scribus 1.2.3 ó menor. El proceso de guardado hará que este archivo no se pueda volver a utilizar en Scribus 1.2.3 a menos que utilice Archivo-&gt;Guardar como. ¿Está seguro de querer continuar con esta operación?</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Está intentando importar más páginas de las que hay disponibles en el documento actual contando desde la página activa.&lt;/p&gt;Elige una de las siguientes opciones:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Crear&lt;/b&gt; páginas no disponibles&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importar&lt;/b&gt; páginas hasta la última página&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancelar&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>C&amp;rear</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Los cambios del documento no han sido guardados y ha seleccionado revertirlos. ¿Desea continuar?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
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
        <translation>Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centímetros (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cícero (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>El archivo existe</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Ya existe un archivo llamado &apos;%1&apos;.&lt;br/&gt;¿Desea reemplazarlo con el archivo que está guardando?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Reemplazar</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>página</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Generador de códigos de barra</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Rueda de color</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Vista previa de la tipografía</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Mi extensión</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nuevo desde plantilla</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Plantilla de documento: </translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Boletines</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Folletos</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catálogos</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Cuartillas</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Letreros</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Postales</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Membretes</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Sobres</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Tarjetas de presentación</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendarios</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiquetas</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menús</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programas</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formularios PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Presentaciones PDF</translation>
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
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Documentos de texto</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Dípticos</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Cajas de media</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Plantillas propias</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportar como imagen</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Guardar como imagen</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Error al escribir el/los archivo/s de salida.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>La exportación se realizó correctamente.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>El archivo existe. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>ya existe. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Sí a todo</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>Importación de PS/EPS</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Todos los formatos soportados (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Guardar como plantilla</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede obtener un color sin nombre.</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Color no encontrado.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede cambiar un color sin nombre.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>El color no se encontró en el documento.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>El color no se encontró en los colores por defecto.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede crear un color sin nombre.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede eliminar un color sin nombre.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede reemplazar un color sin nombre.</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder es mayor de lo permitido.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Error al abrir el documento.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Error al guardar el documento.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unidad fuera de rango. Utilice alguna de las constantes scribus.UNIT_*.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Color no encontrado- error de python</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>El argumento debe ser el nombre de un objeto de página, o una instancia PyCObject</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Propiedad no encontrada</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Hijo no encontrado</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>No se puede convertir el tipo &apos;%1&apos; obtenido.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Propiedad tipo &apos;%1&apos; no está soportada</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>No se puede convertir &apos;%1&apos; a propiedad tipo &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Lo tipos coincidieron, pero la configuración de la propiedad falló.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>El destino no es un marco de imagen.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>El objeto especificado no es un marco de imagen.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>No se pueden agrupar menos de dos objetos</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>No se pueden agrupar menos de dos objetos</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Se necesita una selección o lista de elementos como argumento para agrupar</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>No se puede escalar por 0%.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Tipografía no encontrada.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>No se puede renderizar una muestra vacía.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Imposible guardadr mapa de píxeles</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>No se puede tener un nombre de capa vacío.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Capa no encontrada.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>No se puede eliminar la última capa.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>No se puede crear una capa sin nombre.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Ya existe un objeto con el nombre requerido.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe contener, al menos, dos puntos (cuatro valores).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>La lista de punto debe contener un número par de valores.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe contener al menos tres puntos (seis valores).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>La lista de puntos debe contener al menos cuatro puntos (ocho valores).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>La lista de punto debe contener un múltiplo de seis valores.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objeto no encontrado.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Estilo no encontrado.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede aplicar un estilo a un marco no de texto.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Error al guardar EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Número de página fuera de rango.</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>El nombre de página maestra no concuerda con ninguna existente.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>el argumento no es una lista: debe ser una lista de valores en coma flotante.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>el argumento contiene valores no numéricos: debe ser una lista de valores en coma flotante.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>el argumento contiene valores no numéricos: debe ser una lista de valores en coma flotante.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Ancho de línea fuera de los límites, debe ser 0 &lt;= ancho_línea &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Saturación de línea fuera de rango, debe ser 0 &lt;= saturación &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Saturación de relleno fuera de rango, debe ser 0 &lt;= saturación &lt;= 100.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>El radio de esquina debe ser un número positivo.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Estilo de línea no encontrado.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño de tipografía de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener la tipografía de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño de texto de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el total de columnas de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el espacio de línea de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el espaciado de columna de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el texto de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar texto a un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede insertar texto en un marco no de texto.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Insertar índice fuera de los límites.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Alineación fuera de rango. Utilice alguna de las constantes scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar una alineación de texto a un marco no de texto.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Tamaño de tipografía fuera de límites - debe ser 1&lt;= tamaño &lt;=512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un tamaño de tipografía a un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar una tipografía a un marco no de texto.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Espaciado de línea fuera de límites, debe ser &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un espaciado de línea a un marco no de texto.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Espaciado de columna fuera de límites, debe ser positivo.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un espaciado de columna a un marco no de texto.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Total de columnas fuera de límites, debe ser &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un número de columnas a un marco no de texto.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Índice de selección fuera de límites</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede seleccionar texto en un marco no de texto</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede borrar texto de un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un relleno de texto en un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar un trazo del texto a un marco no de texto.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede asignar una saturación de texto a un marco no de texto.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Sólo se pueden enlazar marcos de texto.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>El marco destino debe estar vacío.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>El marco destino enlaza con otro marco.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>El marco destino está enlazado por otro marco.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Origen y destino son el mismo objeto.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede desenlazar un marco no de texto.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>El objeto no es un marco de texto enlazado, no se puede desenlazar.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>El objeto es el último marco en una serie, no se puede desenlazar. Desenlace el marco previo en su lugar.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>No se puede convertir un marco no de texto a contornos.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Sólo puede comprobarse el desbordamiento en los marcos de texto</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede asignar un marcador a un marco no de texto</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede obtener información de un marco no de texto</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>El nombre de archivo debe ser una cadena.</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>El nombre de archivo no debe ser una cadena vacía.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>No se puede eliminar configuración de tipo de imagen.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>La imagen debe ser una cadena.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>El atributo &apos;allTypes&apos; es de SÓLO-LECTURA</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Error al exportar imagen</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Ejecutar script...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Mostrar &amp;consola</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Acerca del script...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripts de Scribus</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Scripts &amp;recientes</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Acerca del script</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Scripter</translation>
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
        <translation>Módulo de interfaz Python de Scribus

Este módulo es la interfaz Python para Scribus. Provee funciones para
controlar Scribus y manipular objetos en el lienzo. Cada función está
documentada individualmente a continuación.

Unas cuantas cosas son comunes a casi toda la interfaz.

La mayoría de las funciones operan con marcos. Los marcos son identificados
por su nombre, una cadena - no son objetos Python reales. Muchas funciones
toman un parámetro opcional (que no sea palabra clave), un nombre de marco.
Muchas excepciones son también comunes en muchas funciones. No están
documentadas actualmente en el docstring de cada función.
- Muchas funciones lanzarán NoDocOpenError si intenta usarlas sin tener un
documento abierto con el que operar.
- Si no pasa un nombre de marco a una función que lo requiera, la función
utilizar el marco seleccionado actualmente, de haberlo, o lanzará
NoValidObjectError si no puede encontrar ninguno con el que operar.
- Muchas funciones lanzará WrongFrameTypeError si intenta usarlas en un
tipo de marco para el que no están diseñadas. Por ejemplo, asignar el color
de texto en un marco de gráficos no tiene sentido, y acabará lanzando esta
excepción.
- Los errores resultantes de las llamadas a la API de Python subyacente serán
pasadas sin alteración. Por lo tanto, la lista de excepciones lanzadas por cualquier
función disponible aquí y en su docstring es incompleta.

Se provee de detalles sobre las excepciones que cada función puede lanzar en la
documentación de la función, aunque al igual que la mayoría del código Python, esta
lista no es exhaustiva debido a las excepciones de las funciones llamadas.
</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuración personalizada (opcional): </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuración estándar: </translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Palabras cortas</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Palabras cortas está procesando. Por favor, espere...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Palabras cortas está procesando. Terminado.</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>Exportar a SVG</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg *.svgz);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>¿Realmente quiere sobreescribir el archivo:
%1 ?</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>Importar SVG</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Soporte antiguo formato .sla</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Importar OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Archivos de valores separados por coma</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>datos_CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>cabecera_CSV</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Documentos Word</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Archivos HTML</translation>
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
Enlaces externos</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Documentos de texto OpenDocument</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Documentos Palm PDB</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>datos_PDB</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Importar PDB</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>No se pudo abrir el archivo %1</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>El archivo no se reconoce como un documento PDB. Por favor, informe de ello si está seguro de que es un error.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Documentos OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtros de texto</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Archivos de texto</translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation>Inglés (americano)</translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation>Inglés (australiano)</translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation>Alemán (Suiza)</translation>
    </message>
    <message>
        <source>Chinese (Trad.)</source>
        <translation>Chino (tradicional)</translation>
    </message>
    <message>
        <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
        <translation type="unfinished">La tipografía %1 tiene daños en las medidas en el archivo %2, ignorando medidas</translation>
    </message>
    <message>
        <source>Valid metrics were found for font %1, using metrics in file %2</source>
        <translation type="unfinished">Se han encontrado medidas válidas en la tipografía %1, utilizando medidas en archivo %2</translation>
    </message>
    <message>
        <source>Insufficient memory for this image size.</source>
        <translation>Memoria insuficiente para el tamaño de esta imagen.</translation>
    </message>
    <message>
        <source>Fill opacity out of bounds, must be 0.0 &lt;= opacity &lt;= 1.0</source>
        <comment>python error</comment>
        <translation>Opacidad de relleno fuera de límites, debe ser 0.0 &lt;= opacidad &lt;= 1.0</translation>
    </message>
    <message>
        <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
        <comment>python error</comment>
        <translation>Transparencia fuera de los límites, debe ser 0 &lt;= transparencia &lt;= 1.</translation>
    </message>
    <message>
        <source>%1 has more than 8 bits per channel, Scribus will not preserve less significant bits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get text distances of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text distances out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text distances on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Kurdish</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Limpiar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rehacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Co&amp;rtar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menú del sistema</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Enrollar</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Desenrollar</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizar</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximizar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurar</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Mover</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimizar</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximizar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Siempre &amp;visible</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Restaurar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Enroll&amp;ar</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>D&amp;esenrollar</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Configuración del documento</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Anchura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Unidad:</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Aplicar configuración de tamaño a todas las páginas</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de margen</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autoguardado</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervalo:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Información del documento</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Visionado de página</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Mostrar &amp;el área no imprimible con el color del margen</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Mostrar enlace de texto</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Mostrar caracteres de control de texto</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Mostrar marcos</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Reglas relativas a la página</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Área de trabajo mínima</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>I&amp;zquierda:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Espaciado entre páginas</translation>
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
        <source>Display</source>
        <translation>Visor</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografía</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>División por sílabas</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Tipografías</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Verificación previa</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Exportar a PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Atributos de objetos del documento</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabla de contenido e índices</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Secciones</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Gestión de color</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Activar o desactivar el visionado de marcos de texto enlazados.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Mostrar caracteres no imprimisble como los marcadores de párrafo en los marcos de texto</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Des/activa el visionado de los marcos</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Color del papel</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Enmascarar el área fuera de los márgenes con el color de margen</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Aplicar los cambios de tamaño de página a todas las páginas existentes en el documento</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Ajustar colores</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation>Mostrar imágenes</translation>
    </message>
    <message>
        <source>Turns the display of images on or off</source>
        <translation>Des/activa el visionado de imágenes</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Scripts de Python (*.py);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Ejecutar como un script de extensión</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Gestor de estilos</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation>Columna 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation>C&amp;lonar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>Acep&amp;tar</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>A&amp;plicar</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>Ca&amp;ncelar</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>Líneas</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Configuraciones de estilo</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo actual</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Configuraciones de color de relleno</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color del relleno de texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color del relleno de texto</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Configuraciones del color de trazo</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color del contorno de texto</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Configuraciones de tipografía</translation>
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
        <translation>Tipografía del texto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de tipografía</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar anchura de caracteres</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Escalar altura de caracteres</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Configuraciones del carácter</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Interletrado manual</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Guardar página como &amp;SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Exporta archivos SVG</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Exporta la página actual en un archivo SVG.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importar &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importa archivos SVG</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importa la mayoría de archivos SVG en el documento
actual, convirtiendo los datos vectoriales en objetos Scribus.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>El archivo SVG contiene algunas características no soportadas</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Palabras cortas</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Insertar un espacio de no separación en:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>Marcos &amp;seleccionados</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Página activa</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>T&amp;odos los objetos</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Sólo se han procesado los marcos seleccionados.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Sólo se ha procesado la página actual.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Se han proceado todos los objetos del documento.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Configuración del usuario</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Configuración general del sistema</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Reiniciar</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Guardar configuración del usuario</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Recargar configuración general del sistema y eliminar la definida por el usuario</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Editar configuración personalizada. Si la guardas, será utilizada en vez de la general del sistema</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Palabras cortas</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Ya existe una configuración del usuario. ¿Realmente quieres sobreescribirla?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>No se puede escribir archivo %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Configuraciones del usuario guardadas</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Configuración general del sistema recargada</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>No se puede abrir archivo %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Guardar como &amp;plantilla...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Guardar documento como plantilla</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Guardar documento como plantilla. Un buen método para facilitar el trabajo inicial en documento con una apariencia constante</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Cargar/Guardar/Importar/Exportar</translation>
    </message>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Persistente</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Superior</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecho</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Inferior</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Izquierdo</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Permitir acoplar a...</translation>
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
        <translation>Orientación al flotar...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Imprimiendo...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>Colores del documento</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Ocurrió un error al abrir los perfiles ICC, la gestión de color no está activada.</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nueva capa</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>¿Realmente quiere limpiar todo el texto?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>No se puede eliminar objeto en uso</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>El objeto %1 está siendo editado por el editor interno. La operación de eliminado será cancelada</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializando extensiones</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Inicializando teclas rápidas</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Leyendo preferencias</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Inicializando el editor interno</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Leyendo perfiles ICC</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Inicializando división por silabas</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Leyendo álbum de recortes</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Configurando teclas rápidas</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Editar</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Buscando tipografías</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>No se han encontrado tipografías en el sistema.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Cerrando programa.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Error fatal</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Sistema de tipografías inicializado</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Abrir &amp;reciente</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportar</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Contenidos</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>Est&amp;ilo</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Color</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Saturación</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>Tipogra&amp;fía</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efectos</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objeto</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Configuración de vista previa</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivel</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Enviar a ca&amp;pa</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Opciones &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>C&amp;onvertir en</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>I&amp;nsertar</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Carácter</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Comillado</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Espacios &amp;y rupturas</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaduras</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Vista</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Ventanas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>Ay&amp;uda</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Alineación</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Listo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importando páginas...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar página/s</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importación terminada</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>No se encontró nada a importar</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>El archivo %1 no es un formato aceptado</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Cargando...</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Algunos perfiles ICC usados en el documento no están instalados:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> fue reemplazada por: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(convertido)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>No se puede escribir el archivo: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *scd.gz);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Guardando...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus ha detectado algunos errores. Considere utilizar la verificación previa para corregirlos</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorar</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Abortar</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Imprimiendo...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>¡Error en la impresión!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>No se puede cortar objeto en uso</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>El objeto %1 está siendo editando en el editor interno. La operación de cortado será cancelada</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Acerca de Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Manual de Scribus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Convertir página en página maestra</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nueva página maestra %1</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ningún estilo</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Número de copias: %1
Desplazamiento horizontal: %2
Desplazamiento vertical: %3</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Los siguientes programas no están disponibles:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: no puedes utilizar imágenes EPS o la vista previa de impresión</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: no puedes utilizar imágenes EPS o la vista previa de impresión PostScript</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript no está disponible: la vista previa de impresión Postscript no está disponible</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus detectó algunos errores.
Considere utilizar la verificación previa  para corregirlos.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>Archivos EPS (*.eps);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Se detectaron algunos errores.
Considere utilizar la verificación previa para corregirlos</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Página%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Algunos objetos están bloqueados.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>B&amp;loquear todos</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Desbloquear todos</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Información</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>¡El programa %1 ya se está ejecutando!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>¡El programa %1 no está disponible!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>El color seleccionado no existe en el set de color del documento. Por favor, introduzca un nombre para este nuevo color.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Color no encontrado</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>El nombre que ha seleccionado ya existe. Por favor, introduzca un nombre diferente para este nuevo color.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>Nive&amp;l</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Enviar a capa</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Configuración de &amp;vista previa</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Espaciado</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;dura</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Herramientas</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Posición X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Posición Y:</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>¿Realmente quiere reemplazar la imagen existente?</translation>
    </message>
    <message>
        <source>Your document was saved to a temporary file and could not be moved: 
%1</source>
        <translation>El documento fue guardado en un archivo temporal y no puede ser movido: 
%1</translation>
    </message>
    <message>
        <source>Scribus </source>
        <translation type="obsolete">Scribus </translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Arguento inválido: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>El archivo %1 no existe, abortando.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Uso: scribus [opción ...] [archivo]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Muestra la ayuda (exte mensaje) y sale del programa</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Usar xx como un atajo para el idioma, p. ej. &apos;en&apos; o &apos;de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Lista los idiomas de interfaz instalados actualmente</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Muestra información en la consola mientras las tipografías son cargadas</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>No mostrar la pantalla de inicio al arrancar</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Muestra la información de versión y sale del programa</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Usar orden de botones de diálogo de derecha a izquierda (p. ej. Cancelar/No/Sí en vez de Sí/No/Cancelar)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>nombre de archivo</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Usar nombre de archivo como ruta para las preferencias dadas por el usuario</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>Muestra una ventana de consola</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Los idiomas de interfaz de Scribus instalados son los siguientes:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Para sobreescribir el idioma por defecto elige:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx o scribus --lang xx, donde xx es el idioma elegido.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Versión de Scribus</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Página principal</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Documentación</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation></translation>
    </message>
    <message>
        <source>Show location ICC profile information on console while starting</source>
        <translation>Mostrar la información de ubicación del perfil ICC en la consola al iniciar</translation>
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
        <translation>Capa</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Copiar aquí</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Mover aquí</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Archivo: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>PPI originales: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>PPI actuales: </translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Espacio de color: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
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
        <translation>Escala de grises</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Texto enlazado</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Marco de texto</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Texto en un trazado</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Párrafos: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Activado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Desactivado</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;formación</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Configuración de vista previa</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Opciones &amp;PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Enviar a ca&amp;pa</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;vel</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Conve&amp;rtir en</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Contenidos</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Enlazar marcos de texto</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Está intentando enlazar a un marco con relleno, o consigo mismo.</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Página %1 a %2</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>No se puede convertir objeto en uso</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>El objeto %1 está siendo editado en el editor interno. La operación de conversión a contornos de este elemento será ignorada</translation>
    </message>
    <message>
        <source>Export: </source>
        <translation>Exportar: </translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Imagen</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>Documento:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>ha sido cambiado desde el último guardado.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Descartar</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Soporte empotrado de scripts de Python.</translation>
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
        <translation>Error en el script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Si está ejecutando un script oficial, por favor, informe sobre ello en &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Este mensaje está también en el portapapeles. Utilice Ctrl+V para pegarlo en el seguimiento de errores.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Ocurrió un error interno al intentar ejecutar el comando introducido. Se mostraron los detalles en stderr. </translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Examinar script</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Scripts de Python (*.py);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Documentación para:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> no contiene ninguna docstring!</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Error al configurar la extensión Python. Los detalles de error se mostraron en strderr. </translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Extensions</source>
        <translation>Extensiones</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Consola</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Cambiar...</translation>
    </message>
    <message>
        <source>Scripter Preferences</source>
        <translation>Preferencias de Scripter</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Activar scripts de extensión</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Script de inicio:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Errores:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Comentarios:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Palabras clave:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Señales:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Números:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Cadenas:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Textos de base:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Seleccionar color</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Ubicar script de inicio</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Mostrar vista previa de página</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Buscar/Reemplazar</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Buscar:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Estilo de párrafo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Tipografía</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de tipografía</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efectos de tipografía</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Color de relleno</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Saturación de relleno</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Color de trazo</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Sombra del trazo</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Izquierdo</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecho</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Justificado</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forzado</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Reemplazar por:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>Pala&amp;bra completa</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorar mayúsculas</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Bu&amp;scar</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Reemplazar</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Reemplaz&amp;ar todas</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Búsqueda terminada</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Búsqueda terminada, se encontraron %1 coincidencias</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Seleccionar campos</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Campos disponibles</translation>
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
        <translation>Campos seleccionados</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Otro...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
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
        <translation>Desplazamiento-X</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Desplazamiento-Y</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Pala&amp;bras cortas...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Palabras cortas</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Extensión especial para añadir espacios de no separación antes o después de las llamadas palabras cortas. Disponible en los siguientes idiomas: </translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ningún estilo</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Editar estilos...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Ningún estilo</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Editar estilos</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo estilo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos los documentos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Insertar</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Carácter</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Comillado</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Espacios &amp;y rupturas</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaduras</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Con&amp;figuraciones</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Editor interno</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Limpiar todo el texto</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Recargar el texto desde un marco</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Guardar en archivo...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Cargar desde archivo...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Guardar &amp;documento</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Act&amp;ualizar marco de texto y salir del programa</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Salir sin actualizar marco de t&amp;exto</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Seleccion&amp;ar todo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Co&amp;rtar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Bu&amp;scar/Reemplazar...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Editar estilos...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Vista previa de tipogra&amp;fías...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>Act&amp;ualizar marco de texto</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Espaciado</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Insertar carácter...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Fondo...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Mostrar tipografía...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Selección de texto inteligente</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Párrafo actual:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totales:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Párrafos: </translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Editor interno - %1</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>¿Quiere guardar los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>¿Realmente quiere perder todos los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>¿Realmente quiere limpiar todo el texto?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
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
        <translation>Desplazamiento</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Ancho de línea</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation>Están seleccionados más de un objeto</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Subrayar texto. Presiona el botón momentáneamente para configurar las opciones del ancho de línea y desplazamiento.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Subrayar sólo palabras. Presiona el botón momentáneamente para configurar las opciones del ancho de línea y desplazamiento.</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Mayúsculas</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Tachado. Presiona el botón momentáneamente para configurar las opciones del ancho de línea y desplazamiento.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation>Contorno. Presiona el botón momentáneamente para cambiar el ancho de trazo.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Texto sombreado. Presiona el botón momentáneamente para activar el desplazamiento del espaciado.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Opciones de importación de OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Sobreescribir estilos de párrafo</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Al activar esta opción, se sobreescribirán los estilos existentes en el documento Scribus actual</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionar estilos de párrafo</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionar estilos de párrafo por atributos. Producirá menos estilos de párrafo similares, reteniendo los atributos de estilo, incluso si los estilos del documento original tienen nombres diferentes.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Usar el nombre del documento como prefijo de los estilos de párrafo</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Anteponer el nombre del documento al nombre del estilo de párrafo en Scribus.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>No volver a preguntar</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Hacer estas opciones las opciones por defecto y no volver a preguntar al importar documentos OpenOffice.org 1.x.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabla de contenido e índices</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>El marco en el que la tabla de contenido se insertará</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Los números de página colocados:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Nombre del atributo del objeto:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>El atributo de objeto que se asignará a los marcos utilizados como base para la creación de entradas</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Coloque los números de página de las entradas al comienzo o al final de la línea, o en ninguna parte</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Listas entradas no imprimibles</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Incluir también marcos marcados para no impresión</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>El estilo de párrafo utilizado para las líneas de entrada</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Estilo de párrafo:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Marco destino:</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Tabla de contenido</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Al comienzo</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Al final</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>No mostrados</translation>
    </message>
    <message>
        <source>Table of Contents %1</source>
        <translation>Tabla de contenido %1</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabla de contenido e índices</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Tabla de contenido</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Añadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>El marco en el que se insertará la tabla de contenido</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Ubicación de números de página:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Nombre del atributo de objeto:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>El atributo de objeto que se asignará a los marcos utilizados como base para la creación de entradas</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Coloque los números de página de las entradas al comienzo o al final de la línea, o en ninguna parte</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Listar entradas no imprimibles</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Incluir también marcos marcados para no impresión</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>El estilo de párrafo utilizado para las líneas de entrada</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Estilo de párrafo:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Marco destino:</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ignorar todos los errores</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Comprobar automáticamente antes de imprimir o exportar</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Comprobar caracteres no disponibles</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Comprobar objetos fuera de la página</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Comprobar desbordamiento en los marcos de texto</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Comprobar utilización de transparencias</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Comprobar imágenes no disponibles</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Comprobar resolución imagen</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Menor resolución permitida</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Comprobar archivos PDF ubicados</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Comprobar anotaciones y campos PDF</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Añadir perfil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Eliminar perfil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Configuraciones comunes</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Ubicar en los documentos</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>En el fondo</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>En la superficie</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Ajustar</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Distancia de ajuste:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Radio de selección:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Mostrar guías</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Mostrar márgenes</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Mostrar cuadricula de página</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Cuadrícula mayor</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Espaciado:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Cuadrícula menor</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Mostrar cuadrícula de línea de base</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Configuraciones de línea de base</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Cuadrícula de línea de base:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Desplazamient&amp;o de línea de base:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Las guías no son visibles a través de los objetos en la página</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Las guías son visibles por encima de los objetos en la página</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Distancia entre las líneas de cuadrícula menores</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Distancia entre las líneas de cuadrícula mayores</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Distancia dentro de la cual un objeto se ajustará a las guías ubicadas</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Radio del área donde Scribus permitirá seleccionar un objeto</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Color de las líneas de cuadrícula menor</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Color de las líneas de cuadrícula mayor</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Color de las líneas de guías insertadas</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Color de las líneas de margen</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Color de la cuadrícula de línea de base</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Activa o desactiva la cuadrícula base</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Distancia entre las líneas de la cuadrícula de la línea de base</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Distancia de la parte superior de la página a la primera línea de base</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Activa o desactiva las líneas de cuadrícula</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Activa o desactiva las guías</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Activa o desactiva los márgenes</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides. After setting this you will need to restart Scribus to set this setting.</source>
        <translation>Distancia dentro de la cual un objeto se unirá a las guías creadas. Si modifica este valor deberá reiniciar Scribus para poder emplearlo.</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles.After setting this you will need to restart Scribus to set this setting.</source>
        <translation>Radio del área donde Scribus permitirá seleccionar un objeto. Si modifica este valor deberá reiniciar Scribus para poder emplearlo.</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Gestionar tabuladores</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Exportar rango</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Tod&amp;as las páginas</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Eli&amp;gir páginas</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Ajustar a márgenes de página</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Opciones de archivo</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Compat&amp;ibilidad:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Encuadernación:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Margen izquierdo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Margen derecho</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Generar minia&amp;turas</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Guardar marcos de texto en&amp;lazados como artículos PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Incluir marcadores</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Incluir c&amp;apas</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Resolución de gráficos EPS:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Com&amp;primir texto y gráficos vectoriales</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Método de compresión de imágenes</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automático</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>JPEG con pérdidas</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Zip sin pérdidas</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguna</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Méto&amp;do de compresión:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Máximo</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Alta</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Media</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Baja</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Mínima</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Calidad de compresión:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Re&amp;muestrear imágenes a:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;General</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Incrustar</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tipografías disponibles:</translation>
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
        <translation>Tipografías a incrustar:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Tipografías</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Activar efectos de &amp;presentación</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Mostrar vista pre&amp;via de página</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efectos</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Duración del visionado:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Duración del efec&amp;to:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>T&amp;ipo de efecto:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Marquesinas:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Desde la:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Dire&amp;cción:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> s</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Sin efecto</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Bandas</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Caja</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Disolver</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Brillo</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>División</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Cortina</translation>
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
        <translation>Interior</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Exterior</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Izquierda a derecha</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Arrba a abajo</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Abajo a arriba</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Derecha a izquierda</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Superior izquierda a inferior derecha</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Mostrar configuraciones</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Página simple</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Continua</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Doble página izquierda</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Doble página derecha</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Apariencia visual</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Usar configuración por defecto de visores</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Usar modo a pantalla completa</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Mostrar pestaña de marcadores</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Mostrar miniaturas</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Mostrar pestaña de capas</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Ocultar barra de herramientas de los visores</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Ocultar barra de menú de los visores</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Ajustar páginas a la ventana del visor</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Acciones especiales</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript a ejecutar cuando
el documento PDF se abra:</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Ningún script</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Visor</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Usar encriptación</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Contraseñas</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Usuario:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Pr&amp;opietario:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Configuraciones</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Permitir im&amp;presión del documento</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Permitir &amp;cambiar el documento</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Permitir cop&amp;iar texto y gráficos</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Permitir añadir &amp;anotaciones y campos</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>S&amp;eguridad</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Archivo de salida destinado para:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Pantalla / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Impresora</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Escala de grises</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convertir colores de tinta plana en colores de proceso</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Usar configuraciones de renderizado personalizadas</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Configuraciones de renderizado</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Fre&amp;cuencia:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>Án&amp;gulo:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Función de &amp;puntos:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Punto simple</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Colores sólidos:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Usar perfil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Perfil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Prueba de renderizado:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimetría relativa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimetría absoluta</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Imágenes:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>No utilizar perfiles ICC incrustados</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>C&amp;olor</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Prueba de salida PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>Cadena de &amp;información:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Perfil de salida:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Cuadro de recorte</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Activa los efectos de presentación al utilizar Adobe&amp;#174; Reader&amp;#174; y otros visores de PDF que los soporten a pantalla completa.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Muestra la vista previa de página para cada página listada arriba.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Duración del visionado de la página antes de que la presentación comience en la página seleccionada. Indicando 0 desactivará la transición de página automática.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Duración de la ejecución del efecto. Un tiempo menor acelerará el efecto, uno mayor lo ralentizará.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Tipo de efecto que se mostrará.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Dirección del efecto de las marquesinas para los efectos división y bandas.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Posición inicial para los efectos caja y división.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Dirección de los efectos brillo y cortina.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Aplicar el efecto seleccionado a todas las páginas.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportar todas las páginas a PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportar un rango de página a PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Insertar una lista de elementos separados por coma, donde un elemento puede ser * para todas las páginas, 1-5 para un rango de páginas o solo un número de página.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Determina la compatibilidad PDF. Por defecto es PDF 1.3 que provee de la mayor compatibilidad. Elige PDF 1.4 si el archivo utiliza características como transparencia o requiere encriptación de 128 bits. PDF 1.5 es necesario cuando deseas preservar objetos en capas separadas dentro del PDF.  PDF/X-3 es para exportar el PDF cuando quieres gestión de color RGB para impresiones comerciales y es seleccionable cuando has activado la gestión de color. Usar sólo cuando sea requerido por el impresor o en algunos casos al imprimir en una impresora láser digital de 4 colores.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Determina la encuadernación de las páginas en el PDF. A menos que sepa que necesita cambiarlo, mantenga la opción por defecto - Derecha.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Genera miniaturas de cada página en el PDF. Algunos visores las pueden utilizar para la navegación.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Generar artículos PDF, los cuales son útiles para la navegación de artículos enlazados en PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Las capas del documento son exportadas al PDF únicamente si PDF 1.5 está seleccionado.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Incrustar los marcadores creados en el documento. Son útiles para la navegación de largos documentos en PDF.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Exportar la resolución del texto y gráficos vectoriales. No afecta a la resolución de imágenes de mapas de bits como fotografías.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Activa la compresión sin pérdidas del texto y gráficos. A menos que tenga una razón, deje esta opción marcada. Esto reduce el tamaño del archivo PDF.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Re-muestrear las imágenes de mapas de bits al DPI seleccionado. Dejando esta opción desmarcada, renderizará las imágenes a su resolución nativa. Activando esta opción, aumentará el uso de memoria y ralentizará la exportación.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (puntos por pulgada) para exportar la imagen.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Activa las características de seguridad en el PDF exportado. Si seleccionó PDF 1.3, el PDF estará protegido por encriptación de 40 bits. Si seleccionó PDF 1.4, el PDF estará protegido por encriptación de 128 bits. Advertencia: la encriptación PDF no es tan fiable como la encriptación GPG o PGP y tiene algunas limitaciones.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Elige una contraseña maestra que active o desactive todas las características de seguridad en el PDF exportado</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Elige una contraseña para que los usuarios puedan leer el PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Permite la impresión del PDF. Si está desmarcado, se evita la impresión. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Permite la modificación del PDF. Si está desmarcado, se evita la modificación del PDF.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Permite copiar textos o gráficos del PDF. Si está desmarcado, los textos y gráficos no se pueden copiar.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Permite añadir anotaciones y campos al PDF. Si está desmarcado, se evita la edición de anotaciones y campos.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Modelo de color para la salida del PDF. Elige Pantalla/Web para PDF cuyo destino es el visionado por pantalla y la impresión en impresoras de tinta típicas. Elige Impresora si se va a imprimir en una verdadera impresora de 4 colores CMYK. Elige Escala de grises cuando quieras un PDF en escala de grises.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Esta es una configuración avanzada que no está activada por defecto. Sólo debería activarse a petición expresa del impresor y cuando te haya dado los detalles exactos requeridos. En cualquier otro caso, el PDF exportado podria no imprimirse debidamente y no ser verdaderamente portable entre sistemas.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Incrustar un perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Prueba de renderizado para colores sólidos</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Incrustar un perfil de color para las imágenes</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>No usar perfiles de color que están incrustados en las imágenes originales</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Perfil de color para las imágenes</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Prueba de renderizado para imágenes</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Perfil de salida para impresión. Si es posible, obten algunas directrices de tu impresor sobre la selección de perfil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Cadena obligatoria para PDF/X-3 o el PDF fallará en la conformidad PDF/X-3. Recomendamos utilizar el título del documento.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Distancia de la sangría a la parte superior de la página fisica</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Distancia de la sangría a la parte inferior de la página física</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Distancia de la sangría a la parte izquierda de la página física</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Distancia de la sangría a la parte derecha de la página física</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Reflejar página/s horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Reflejar página/s verticalmente</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Activa la conversión de colores de tinta plana a colores de composición. A menos que esté planeando imprimir colores de tinta plana en una impresora comercial, es probablemente mejor dejarlo activado.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>No mostrar objetos fuera de los márgenes en el archivo exportado</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Método de compresión a utilizar en las imágenes. Automático permite a Scribus elegir el mejor método. ZIP es una compresión sin pérdidas y bueno para imágenes con colores sólidos. JPEG es mejor para crear archivos PDF más pequeños con muchas fotografías (con la menor pérdida de calidad de imagen posible). Déjalo a automático a menos que necesites opciones especiales de compresión.</translation>
    </message>
    <message>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Niveles de calidad para métodos de compresión sin pérdidas: Mínimo (25%), Bajo (50%), Medio (75%), Alto (85%), Máximo (95%). Note que un nivel de calidad no determina directamente el tamaño de la imagen resultante - tambo el tamaño como la calidad varía de imagen en imagen a cualquier nivel de calidad dado. Incluso con máximo seleccionado, siempre hay alguna pérdida de calidad con jpeg.</translation>
    </message>
    <message>
        <source>&amp;Embed All</source>
        <translation>&amp;Incrustar todo</translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation>Tipografías a contorno:</translation>
    </message>
    <message>
        <source>Outline &amp;All</source>
        <translation>Todo &amp;a contornos</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Disposición del documento</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>Incrustar tipografías en el PDF. Incrustar las tipografías preservará la disposición y apariencia del documento. Algunas tipografías como Open Type sólo pueden incrustar un subconjunto, pues no se pueden incrustar en versiones de PDF anteriores a PDF 1.6. </translation>
    </message>
    <message>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation>Incrustar subconjunto de todas las tipografías en el PDF. Esto sólo incrusta los caracteres utilizados en vez de la tipografía completa en el PDF. Algunas tipografías como Open Type sólo pueden incrustar un subconjunto, pues no se pueden incrustar en versiones de PDF anteriores a PDF 1.6.</translation>
    </message>
    <message>
        <source>&amp;Apply Effect to all Pages</source>
        <translation>&amp;Aplicar efectos a todas las páginas</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Tipografía:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Color de texto:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Saturación:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Trazo del texto:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Color de relleno:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Color del trazo:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punto</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Guión</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Tabulador de relleno de carácter:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Ancho de tabulador:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Espaciado:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>El veloz murciélago hindú comía feliz cardillo y kiwi. La cigüeña tocaba el saxofón detrás del palenque de paja. 1234567890</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Color de &amp;línea:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Color de &amp;relleno:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>S&amp;aturación:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Estilo de línea:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>A&amp;ncho de línea:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Es&amp;tilo de línea:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Flechas:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Inicio:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Final:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>E&amp;scalado libre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Escalado &amp;horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Escalado &amp;vertical:</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Mantener &amp;ratio de aspecto</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Color de re&amp;lleno:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Usar trazado de recorte incrustado</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Vista previa en pantalla</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Vista previa a resolución completa</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Vista previa a resolución normal</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Vista previa a resolución baja</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mí&amp;nimo:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Má&amp;ximo:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Pasos:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Propiedades del marco de texto</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Propiedades del dibujo de formas</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Niveles de zoom por defecto</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Propiedades de dibujo de líneas</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Propiedades de dibujo de polígonos</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Tipografía para nuevos marcos de texto</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Tamaño de la tipografía para nuevos marcos de texto</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Color de la tipografía</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Número de columnas en un marco de texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Espaciado entre columnas de marco de texto</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Muestra de la tipografía</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Escalado horizontal de imágenes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Escalado vertical de imágenes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Mantener el mismo escalado vertical y horizontal</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Saturación del color de relleno</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Color de línea de las formas</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Saturación del color de las líneas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Color de relleno de las formas</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Estilo de línea de las formas</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Ancho de línea de las formas</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Zoom mínimo permitido</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Zoom máximo permitido</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Cambiar el zoom en cada operación de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Color de líneas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturación del color</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Estilo de líneas</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Ancho de líneas</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Personalizado:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Personalizado: </translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>&amp;Scale Image to Frame Size</source>
        <translation>E&amp;scalar imagen al tamaño de marco</translation>
    </message>
    <message>
        <source>Image Frame Properties</source>
        <translation>Propiedades del marco de imagen</translation>
    </message>
    <message>
        <source>Image frames allow images to scale to any size</source>
        <translation>Los marcos de imagen permiten escalar imágenes a cualquier tamaño</translation>
    </message>
    <message>
        <source>Images in image frames are scaled to the size of the frame</source>
        <translation>Las imágenes en los marcos de imágenes se escalan al tamaño del marco</translation>
    </message>
    <message>
        <source>Automatically scaled images keep their original proportions</source>
        <translation>Imágenes escaladas automáticamente mantienen sus proporciones originales</translation>
    </message>
    <message>
        <source>Fill color of image frames</source>
        <translation>Color de relleno de los marcos de imágenes</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Desplazamiento:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>E&amp;scala:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>D&amp;esplazamiento:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Es&amp;cala:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Subrayar</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Desplazamiento:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ancho de línea:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Tachado</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Esc&amp;alado:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Espaciado de &amp;línea automático</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Espaciado de línea:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Desplazamiento por encima de la línea de base de la tipografía en una línea</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Tamaño relativo del superíndice comparado con la tipografía normal</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Desplazamiento por debajo de la línea de base de la tipografía en una línea</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Tamaño relativo del subíndice comparado con la tipografía normal</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Tamaño relativo de las versalitas comparado con la tipografía normal</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Porcentaje de aumento sobre el tamaño de la tipografía para el espaciado de línea</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Desplazamiento por debajo de la línea de base de la tipografía normal expresado como un porcentaje de los descendientes de la tipografía</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Ancho de línea expresado como porcentaje del tamaño de la tipografía</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Desplazamiento por encima de la línea de base de la tipografía normal expresado como un porcentaje de los descendientes de la tipografía</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Izquierdo</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecho</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Coma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Posición:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punto</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Guión</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Carácter de relleno:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Eliminar todo</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Sangría de la primera línea del párrafo</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Sangría desde la izquierda del párrafo entero</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Eliminar todos los tabuladores</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Personalizado:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Personalizado: </translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Period</source>
        <translation>Punto</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Free Objects</source>
        <translation>Liberar objetos</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grupo </translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Página </translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Contorno</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objeto</translation>
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
        <translation>Desplazamiento</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Ancho de línea</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Añadir guía vertical</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Añadir guía horizontal</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Eliminar guía vertical</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Eliminar guía horizontal</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Mover guía vertical</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Mover guía horizontal</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Bloquear guías</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Desbloquear guías</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Mover</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Redimensionar</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Rotar</translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation>X1: %1, Y1: %2, %3(new line)X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation>A1: %1, L1: %2
A2: %3, L2: %4</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Cambiar desplazamiento de imagen</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Cambiar escala de imagen</translation>
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
        <translation>Selección</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grupo</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Selección/Grupo</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Crear</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
A: %3, L: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Alinear/Distribuir</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Objetos involucrados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Asignar color de relleno</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Color1: %1, Color2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Asignar saturación color de relleno</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Asignar color de línea</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Asignar saturación de color de línea</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Desbloquear</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Bloquear tamaño</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Desbloquear tamaño</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Activar impresión de objeto</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Desactivar impresión de objeto</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Desagrupar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>De %1
a %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Aplicar página maestra</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Pegar</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Asignar color de relleno de transparencia</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Asignar color de línea de transparencia</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Asignar estilo de línea</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Asignar estilo final de línea</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Asignar el estilo de unión de línea</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Asignar el ancho de línea</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Sin estilo</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Asignar estilo de línea personalizado</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>No utilizar estilo de línea personalizado</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Asignar flecha inicial</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Asignar flecha final</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Crear tabla</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Filas: %1, Columnas: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Asignar tipografía</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Asignar tamaño de tipografía</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Asignar ancho de tipografía</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Asignar altura de tipografía</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Asignar el color de relleno de tipografía</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Asignar el color de contorno de la tipografía</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Asignar saturación de relleno de color de tipografía</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Asignar saturación de color de trazo de tipografía</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Asginar cran</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Asignar espaciado de línea</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Asignar estilo de párrafo</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Asignar idioma</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Alinear texto</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Asignar efectos de tipografía</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Marco de imagen</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Marco de texto</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Curva Bézier</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polilínea</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Texto en un trazado</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Convertir en</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importar imagen SVG</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importar imagen EPS</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Importar imagen OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Área de trabajo</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>El texto fluye alrededor del marco</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>El texto fluye alrededor del cuadro circunscrito</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>El texto fluye alrededor de la línea de contorno</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>El texto no fluye</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Ningún cuadro circunscrito</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Sin línea de contorno</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Página %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Asignar escala de imagen</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Tamaño del marco</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Escalado libre</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Mantener ratio de aspecto</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Romper ratio de aspecto</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Editar línea de contorno</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Editar forma</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Reiniciar línea de contorno</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Añadir página</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Añadir páginas</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Eliminar página</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Eliminar páginas</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Añadir capa</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Eliminar capa</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Renombrar capa</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Subir capa</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Bajar capa</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Enviar a capa</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Activar impresión de capa</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Desactivar impresión de capa</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Cambiar nombre de la capa</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Obtener imagen</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Duplicados múltiples</translation>
    </message>
    <message>
        <source>Reset control point</source>
        <translation>Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>Modify image effects</source>
        <translation>Modificar efectos de imagen</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Estado inicial</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Historial de acciones</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Mostrar únicamente los objetos seleccionados</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Rehacer</translation>
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
        <translation>Intentando obtener el archivo de actualización de versión de Scribus</translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(No se enviará ningún dato del ordenador a una localización externa)</translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation>S ecabó el tiempo de espera para obtener el archivo de actualización.</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>Error al intentar obtener el archivo de actualización: %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>El archivo no se encontró en el servidor</translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>No se puede abrir el archivo de actualización: %1
Error:%2 en línea: %3, fila: %4</translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>Ocurrió un error al buscar actualizaciones para Scribus, por favor, compruebe la conexión a Internet.</translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>No hay ninguna actualización disponible para la versión de Scribus %1</translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>Hay disponibles una o más actualizaciones para la versión de Scribus (%1):</translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation>Esta lista puede contener versiones de desarrollo.</translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Por favor, visite www.scribus.net para más detalles.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Márgenes mínimos para tamaño de página %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Usar márgenes de impresora</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Seleccionar im&amp;presora:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Márgenes</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Derecho:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>I&amp;zquierdo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
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
        <translation>Insertar valor</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Introducir un valor y pulsar aceptar.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Introducir un valor y pulsar aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Enviar el valor al script</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propiedades...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>Herramientas PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de marcado</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Casilla combo</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Casilla de lista</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Insertar campos PDF</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Insertar anotaciones PDF</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige la importación a utilizar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automático</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importar sólo texto</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importar texto sin ningún formato</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importación:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige la importación a utilizar</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Recordar asociación</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Recordar la asociación extensión de archivo- importación y no volver a preguntar por la selección de la importanción de archivos de este tipo.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nuevo desde plantilla</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Elimina&amp;r</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Creado con</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Descargando plantillas</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Se pueden encontrar plantillas de documento en http://www.scribus.net/ en la sección Descargas.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Instalando plantillas</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extraer el paquete al directorio de plantillas  ~/.scribus/templates para el usuario actual o PREFIX/share/scribus/templates para todos los usuarios del sistema.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Preparando plantilla</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Asegúrate de que las imágenes y tipografías utilizada se pueden usar libremente. Si las tipografías no pueden compartirse, no las recolecte al guardar como plantilla.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>El creador de plantillas debería asegurarse también que la sección anterior instalando plantillas se aplica también a sus plantillas. Esto significa que un usuario debería ser capaz de descargar un paquete de plantillas y poder extraerlo en el directorio de plantillas y comenzar a utilizarlo.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Eliminando plantilla</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Al eliminar una plantilla del diálogo nuevo desde plantilla sólo eliminará la entrada del archivo template.xml, no borrará los archivos del documento. Sólo se muestra un menú emergente con eliminar si tiene acceso de escritura al archivo template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Traduciendo template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copiar un template.xml existente a un archivo llamado template.idioma_PAIS.xml (utilizar el mismo código de idioma presente en el archivo qm de tu idioma), por ejemplo template.es_ES.xml para el idioma español (de España) en template.xml. La copia debe ubicarse en el mismo directorio que el template.xml original para que Scribus pueda cargarlo.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Guardar como plantilla</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categoría</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamaño de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Correo electrónico</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Más detalles</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Menos detalles</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloide</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>personalizado</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Crear filtro</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpiar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Elegir el anterior filtro guardado</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Asignar un nombre a este filtro para guardado</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Asignar un nombre para guardar</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Des/activar esta línea de filtro</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Eliminar esta línea de filtro</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Añadir nueva fila de filtro</translation>
    </message>
    <message>
        <source>to</source>
        <translation>a</translation>
    </message>
    <message>
        <source>and</source>
        <translation>y</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>eliminar coincidencia</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>no eliminar coincidencia</translation>
    </message>
    <message>
        <source>words</source>
        <translation>palabras</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Reemplazar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>El valor de la derecha es una expresión regular</translation>
    </message>
    <message>
        <source>with</source>
        <translation>con</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>estilo de párrafo</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>todas las instancias de</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>todos los párrafos</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>párrafos que comiencen con</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>párrafos con menos de</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>párrafos cono más de</translation>
    </message>
</context>
</TS>
