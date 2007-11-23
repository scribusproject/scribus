<!DOCTYPE TS><TS>
<defaultcodec></defaultcodec>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished">
      getColorNames() -&gt; list

      傳回檔案中所有已定義顏色名稱的列表
      
      </translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

顯示 &quot;新建檔案&quot; 對話視窗, 如果使用者接受設定值, 則開啟新的檔案. 如果使用者案取消鍵,
檔案將不會新建. 如果新檔案已經新增, 傳回 &quot;真&quot;.
      </translation>
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
        <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

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

開新的 pageName 的主頁, 並打開它來編輯.
</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(pageName)

刪除已命名的主頁
</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(double)

在主視窗縮放文件, 縮放值如: 20.0, 100.0 等, 縮放並符合視窗大小已 -100 為記號(為主).
</translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Contributions from:</source>
        <translation>貢獻者: </translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>關於 (&amp;A)</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>作者 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>翻譯 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>線上資料 (&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>開發團隊:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>官方文件: </translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>其他文件: </translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>首頁</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>線上參考資料</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>回報錯誤及功能請求</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>郵遞列表</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>官方翻譯及翻譯者: </translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>先前翻譯貢獻者: </translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>關於 Scribus %1</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation>Windows 移植: </translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation>Mac OSX Aqua 移植: </translation>
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
        <translation>使用的 Ghostscript 版本 %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>沒有可用的 Ghostscript 版本 </translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus 版本 %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>編譯 ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. 
        The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>這個面板顯示Scribus版本, 編譯日期, 並且已經編譯到 Scribus 的支援的函式庫中,
        這裡 C-C-T-F 指的是 C=littlecms C=CUPS T=TIFF 支援 F=Fontconfig, 最後一個字母是渲染器(?) 
        C=cairo 或者 A=libart。不存在的支援函式庫以*號表示. 這個也註明瞭Scribus檢測到的Ghostscript版本號碼
        </translation>
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
        <source>Tango Project Icons:</source>
        <translation>Tango Project Icons:</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>OS/2&amp;#174;/eComStation&amp;#8482; 移植:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished">這個面板顯示版本, 編譯日期, 和支援 Scribus 的 library。C-C-T-F 代表 C=littlecms C=CUPS T=TIFF 支援 F=Fontconfig 支援。</translation>
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
    <message>
        <source>Filename:</source>
        <translation>檔案名:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>版本:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>啓用:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>發佈日期:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>描述:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>作者:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>版權:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>授權:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: 關於插件</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation>檔案名:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>版本:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>啓用:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>發佈日期:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>版權:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>作者:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>描述:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>授權:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;Image Effects</source>
        <translation>圖像效果 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>跳位字元 (&amp;T)...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>打開 (&amp;O)...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>另存新檔 (&amp;A)...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>恢復到已儲存 (&amp;V)</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>準備輸出 (&amp;U)...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>匯入文字...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>追加文字 (&amp;T)...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation> 匯入影像...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>儲存文字 (&amp;T)...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>儲存頁面爲 EPS (&amp;E)...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>儲存爲 PDF (&amp;D)...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>檔案設定 (&amp;S)...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>列印 (&amp;P)...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>退出 (&amp;Q)</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>復原 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>重做 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>物件動作模式 (&amp;I)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪下 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;L 清除內容</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>全選 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>取消全選 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>尋找 / 取代 (&amp;S)...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>顔色 (&amp;O)...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>段落樣式 (&amp;P)...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>線條樣式 (&amp;L)...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>主頁 (&amp;M)...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>偏好設定 (&amp;R)...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>其他 (&amp;O)...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>左 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>中間 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>右 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>區塊 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>強制 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>%1 % (&amp;%)</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>普通 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>底線 (&amp;U)</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>增加底線 (&amp;W)</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>刪除線 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>全部大寫 (&amp;A)</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>小寫 (&amp;C)</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>上標 (&amp;P)</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>下標 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;O 檔案結構樹</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>複製 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>多重複製 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>群組 (&amp;G)</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>取消群組 (&amp;U)</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>鎖定 (&amp;L)</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>鎖定尺寸 (&amp;Z)</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>降低到底部 (&amp;B)</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>升高到頂部 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>降低 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>升高 (&amp;R)</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>傳送到寄存本 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>特性 (&amp;A)...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>顯示圖像 (&amp;M)</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>爲 PDF 書籤 (&amp;B)</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>爲 PDF 注釋 (&amp;N)</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>注釋屬性 (&amp;R)</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>文字域屬性 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>編輯形狀 (&amp;E)...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>將文字附到路徑上 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>從路徑分離文字 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>合併多邊形 (&amp;C)</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>分離多邊形 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>貝茲曲線 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>圖片框 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;O 輪廓</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>多邊形 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>文字框 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>符號 (&amp;G)...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>範例文字</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>插入 (&amp;I)...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>匯入 (&amp;P)...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>刪除 (&amp;D)...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>複製 (&amp;C)...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>移動 (&amp;M)...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>套用主頁 (&amp;A)...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>管理參考線 (&amp;G)...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>符合視窗 (&amp;F)</translation>
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
        <translation>縮圖 (&amp;T)</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>顯示頁面邊距 (&amp;M)</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>顯示框體 (&amp;F)</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>顯示圖片 (&amp;I)</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>顯示網格 (&amp;G)</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>顯示參考線 (&amp;U)</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>顯示底線網格 (&amp;B)</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>顯示文字鏈 (&amp;T)</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>吸附到網格 (&amp;A)</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>吸附到參考線 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>屬性 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>寄存本 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>圖層 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>書籤 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>測量 (&amp;M)</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>歷史動作 (&amp;H)</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>印前檢驗器 (&amp;V)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>工具 (&amp;T)</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF 工具</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>選擇物件</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>表格 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>形狀 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>線條 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>手繪線 (&amp;F)</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>旋轉物件</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>放大或縮小</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>放大</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>縮小</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>編輯框內容</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>串聯文字框</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>解除串聯文字框</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>取色器 (&amp;E)</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>複製物件屬性</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>用故事編輯器編輯文字</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>插入文字框</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>插入圖片框</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>插入表格</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>插入形狀</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>插入多邊形</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>插入線</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>插入貝茲曲線</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>插入手繪線</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>管理圖片 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>用連字元連字 (&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>產生目錄索引 (&amp;G)</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>關於 Scribus (&amp;A)</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>關於 &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>提示 (&amp;P)</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus 使用手冊 (&amp;M)...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>智慧連字元 (&amp;H)</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>不間斷空格 (&amp;S)</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>頁碼 (&amp;N)</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>版權</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>注冊商標</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>商標</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>項目符號</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Em 破折號</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En 破折號</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>圖形破折號</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>引用破折號</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>上標點</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>雙倍直線</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>單面左邊</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>單面右邊</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>雙面左邊</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>雙面右邊</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>單面反轉</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>雙面反轉</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>左單引號</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>右單引號</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>左雙引號</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>右雙引號</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>下單逗號</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>下雙逗號</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>中日韓左單</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>中日韓右單</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>中日韓雙左</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>中日韓雙右</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>切換色板</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>陰影 (&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>低解析度 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>正常解析度 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>完全解析度 (&amp;F)</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>編輯圖片...</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>刷新圖片 (&amp;U)</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>調整框體適應圖片大小</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>圖片延伸屬性</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>顯示控制字元</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>對齊和分佈 (&amp;A)</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>編輯文字...</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>新建線條</translation>
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
        <translation>不間斷破折號</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>切換參考線</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>安排頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>取消連字元</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>管理頁面屬性...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>尺規相對應於頁面</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>預覽列印 (&amp;W)</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>轉換爲主頁...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>層疊 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>平鋪 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>關於插件 (&amp;A)</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>更多資訊...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>允許列印 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>水平翻轉 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>垂直翻轉 (&amp;F)</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>顯示尺規</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>輪廓 (&amp;O)</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation type="unfinished"></translation>
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
        <translation>插入智慧型連字元</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>插入不間斷破折號</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>插入不間斷空格</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>插入頁碼</translation>
    </message>
    <message>
        <source>ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ft</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>st</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;tyles...</source>
        <translation>樣式 (&amp;T)...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>輪廓 (&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>輪廓 (&amp;O)</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>絕對貼上 (&amp;A)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;400%</source>
        <translation>&amp;400%</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>對齊和分佈</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>對齊</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>對應於 (&amp;R):</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>先選擇</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>後選擇</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>頁</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>邊距</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>選擇</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>物件的右邊和錨點的左邊對齊</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>物件的左邊和錨點的右邊對齊</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>底部對齊</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>右邊對齊</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>物件頂部和錨點底部對齊</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>以垂直軸居中</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>左邊對齊</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>以水平軸居中</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>物件底部和錨點頂部對齊</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>頂部對齊</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>分佈</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>左邊等距分佈</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>底部等距分佈</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>中間水平等距分佈</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>讓物件垂直間距相等</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>中間垂直等距分佈</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>頂部等距分佈</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>右邊等距分佈</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>參考線</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>已選參考線 (&amp;S):</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>讓物件水平間距相等</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>讓物件水平間距和指定值相等</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>讓物件垂直間距和指定值相等</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>間距 (&amp;D):</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>按指定的間距分佈物件</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>沒有選定</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>某些物件被鎖定</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>全部解除鎖定 (&amp;U)</translation>
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
        <translation>文字靠左對齊</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>文字靠右對齊</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>文字居中對齊</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>文字兩端對齊</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>文字強制兩端對齊</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>域屬性</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>類型:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>按鈕</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>文本域</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>勾選框</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>多選框</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>列舉框</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>屬性</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名稱:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>工具提示:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>邊</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>顔色:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>寬度:</translation>
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
        <translation>寬</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>樣式:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>實</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>點</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>底線</translation>
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
        <translation>唯讀</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>必需</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>不要輸出值</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>可見度:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>可見</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>隱藏</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>不可列印</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>不可檢視</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>外觀</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>按鈕下陷文本</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>滑鼠經過文本</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>圖示</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>使用圖示</translation>
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
        <translation>滑鼠經過</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>圖示位置...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>高亮</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>反轉</translation>
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
        <translation>密碼</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>限制於</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>字元</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>不要捲動</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>不要拼寫檢查</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>勾選樣式:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>勾選</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>十字</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>寶石</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>圓形</translation>
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
        <translation>預設已勾選</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>可編輯</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>轉到</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>提交表單</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>清空表單</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>匯入資料</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>事件:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>滑鼠上移</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>滑鼠下移</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>滑鼠進入</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>滑鼠退出</translation>
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
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>編輯...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>傳送到 URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>以 HTML 來傳送資料</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>匯入資料:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>目的地</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>到檔案:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>更改...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>頁:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-座標:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-座標:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>動作</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>域格式化爲:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>純</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>數位</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>百分數</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>日期</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>時間</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定義</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>數位格式</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>小數位:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>使用貨幣符號</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>預設貨幣符號</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>格式</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>百分數格式</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>日期格式</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>時間格式</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>自定 Script</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>格式:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>按鍵:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>格式</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>爲驗校值</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>值必須大於或等於:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>而且小於或等於:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>自定驗證 Script:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>驗證</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>沒有計算值</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>值是</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>求和</translation>
    </message>
    <message>
        <source>product</source>
        <translation>相乘</translation>
    </message>
    <message>
        <source>average</source>
        <translation>平均</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>最小值</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>最大值</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>下面域:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>選擇...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>自定計算 Script:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>計算</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>在此輸入以逗號隔開的域列表</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>你最起碼需要用於普通的圖示來使用按鈕的圖示</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>範例:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>選擇更改</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>用於 PDF 1.3 的字體:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>選項被PDF 1.3所忽略</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF 檔案 (*.pdf);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>圖片 (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>無</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>注釋屬性</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文字</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>鏈結</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>外部鏈結</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>外部網頁鏈結</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>目的地</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF 檔案 (*.pdf);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>類型 (&amp;T):</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>更改 (&amp;H)...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>頁面 (&amp;P):</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>X-座標 (&amp;X)</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Y-座標 (&amp;Y):</translation>
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
        <translation>套用主頁</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>主頁 (&amp;M):</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>套用到</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>目前頁面 (&amp;P)</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>偶數頁面 (&amp;E)</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>奇數頁面 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>全部頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>在範圍內 (&amp;W)</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>to</source>
        <translation>到</translation>
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
        <translation>在以下的範圍內套用所選的模板到偶數頁, 奇數頁或者全部頁面</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>在以下的範圍內套用所選的主頁到偶數頁, 奇數頁或者全部頁面</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>條碼產生器 (&amp;B)...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribus 的純 Postscript 條碼輸出前端</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>打開檔案時發生錯誤: %1</translation>
    </message>
    <message>
        <source>Error creating preview</source>
        <translation>產生預覽錯誤</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 或 13 位</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 位</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 或 12 位</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 或 8 位</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 位</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 位</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>條碼未完成</translation>
    </message>
    <message>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>條碼產生器</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>條碼</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>類型 (&amp;T):</translation>
    </message>
    <message>
        <source>C&amp;ode:</source>
        <translation>編碼 (&amp;O):</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>選擇一個可用的條碼類型</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>代表編碼的數位。參見下面的求助資訊</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>重置條碼範例</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>在條碼中包含文字 (&amp;I) </translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>如果勾選, 在條碼中也會有數位</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>注意留白 (&amp;G)</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>畫箭頭確保編碼旁的空位 </translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>顔色</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>背景 (&amp;B)</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>背景顔色 - 在條碼下面</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>線條 (&amp;L)</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>條碼中線條的顔色</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>文字 (&amp;T)</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>文字和數字的顔色</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>這裡顯示提示和求助</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>預覽效果。72dpi 範例。</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>條碼 (&amp;D):</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>包含檢查碼 (&amp;N)</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>產生並包含檢查碼在條碼中</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation type="unfinished"></translation>
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
        <translation>刪除</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>物件</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>新物件</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>重命名</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>載入 (&amp;L)...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>另存新檔 (&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>小 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>中 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>大 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>預覽 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>名稱 (&amp;N):</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>名稱 &quot;%1&quot;非唯一
請另選。</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>選擇寄存本目錄</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>選擇一個目錄</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Scrapbook (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>選擇要匯入的寄存本檔案</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation>匯入寄存本檔案 (&amp;I)...</translation>
    </message>
    <message>
        <source>Main</source>
        <translation>主要</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>書籤</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>移動書籤</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>插入書籤</translation>
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
        <translation>書籤</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>圖示位置</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>版面:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>僅標題</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>僅圖示</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>圖示在標題上面</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>標題在圖示上面</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>標題在圖示右邊</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>標題在圖示左邊</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>標題覆蓋圖示</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>縮放:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>總是</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>當圖示太大時</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>從不</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>如何縮放:</translation>
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
        <translation>圖示</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
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
        <translation>當圖示太大</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>系統配置文件</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>渲染意圖</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>感官</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相對比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>飽和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>絕對比色</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>預設頁面單色的顔色配置文件</translation>
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
        <translation>黑點補償是增加照片對比的方法。
如果你的檔案中有照片, 建議啟用此選項。</translation>
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
        <translation>在螢幕上顯示可能不能正確列印的顔色的方法。
這需要非常精確的配置文件而且僅用於警示之用。</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>啟動色彩管理 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>單色 (&amp;S):</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>監視器 (&amp;M):</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>印表機 (&amp;R):</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>監視器 (&amp;O):</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>印表機 (&amp;I):</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>在監視器上模擬印表機 (&amp;U)</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>標出超出範圍的顔色 (&amp;G)</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>使用黑點補償 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>RGB 圖片 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>CMYK 圖片 (&amp;C):</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>匯入的 CMYK 圖片的預設顔色配置檔案</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>匯入的 RGB 圖片的預設顔色配置檔案</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>編輯顔色</translation>
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
        <translation>網頁安全 RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>新</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>舊</translation>
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
        <translation>動態顔色條</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>靜態顔色條</translation>
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
        <translation>HSV-顔色表</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>名稱 (&amp;N):</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>顔色類型 (&amp;M)</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>是專色</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>是記錄顔色</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>你不能生成一個叫 &quot;%1&quot;的顔色。
這個是給透明色預留的名字</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>顔色名稱不唯一</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>如果你長按滑鼠右鍵，你能看到一個微縮圖。插入鍵插入一個符號到選擇的下面，刪除鍵刪除最後插入的符號。</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>選擇字元:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>字體:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>字元類:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>插入 (&amp;I)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>在文字的游標處插入字元</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>刪除目前選擇的。</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>關閉對話框並返回到文字編輯。</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>完全字元集</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>基本拉丁</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>拉丁-1 附錄</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>拉丁擴展-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>拉丁擴展-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>一般標點符號</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>上標和下標</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>貨幣符號</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>類似字母符號</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>數位形式</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>箭頭</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>數學運算符</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>方盒繪圖</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>區塊元素</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>幾何圖形</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>各種符號</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>裝飾符</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>小形狀變體</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>連字</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>特別符號</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>希臘字元</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>擴展希臘字元</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>斯拉夫字元</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>斯拉夫字元附錄</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>阿拉伯字元</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>阿拉伯字元擴展 A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>阿拉伯字元擴展 B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>希伯來字元</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>關閉此對話框並返回編輯文字</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>符號遺失</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>文字溢出</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>物件不在頁面上</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>圖片遺失</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>圖片的 DPI 值低於 %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>物件有透明度</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>物件是一個 PDF 注釋或域</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>物件是個嵌入的 PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>沒有發現問題</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>頁面 </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>自由物件</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>發現問題</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>印前檢驗器</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>物件</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>問題</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>目前的配置文件:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>忽略錯誤 (&amp;I)</translation>
    </message>
    <message>
        <source>Annotation uses a non TrueType font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>選擇樣式</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>可用樣式</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>選擇一個目錄</translation>
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
        <translation>不能收集所有文件來輸出文件:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>不能收集文件: 
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>顔色</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>複製 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>清除未用的 (&amp;R)</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>顔色組合</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>目前顔色組合:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>儲存顔色組合 (&amp;S)</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>選擇要載入的顔色組合</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>儲存目前的顔色組合</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished">從當前檔案顔色組合清除未用顔色</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>從現有的檔案中匯入顔色組合到目前的組合</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>在目前的顏色組合中建立一個新的顏色</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>編輯目前選取的顏色</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>複製目前選取的顏色</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>刪除目前選取的顏色</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>將目前的顏色組合設定為預設的顏色組合</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>名稱 (&amp;N):</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>選擇一個名稱</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>檔案 (*.sla *.sla.gz *.scd *.scd.gz);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>檔案 (*.sla *.scd);;全部文件 (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>複製 %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>新顔色</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>單色的</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>相似的</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>補充的</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>拆分補充的</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>三合一的</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>四分的（雙補充的）</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>基本顔色</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>單色亮</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>單色暗</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>第一個相似的</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>第二個相似的</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>第一個拆分</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>第二個拆分</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>第三個拆分</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>第四個拆分</translation>
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
        <translation>顔色轉盤</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>顔色</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
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
        <translation>選擇方式:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>角度 (0 - 90 度):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>混合顔色 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>取代顏色 (&amp;R)</translation>
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
        <translation>錯誤: </translation>
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
        <translation>混合顔色 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>取代顏色 (&amp;R)</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>退出 (&amp;X)</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>顔色 (&amp;O)</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>新顔色</translation>
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
        <translation>你所選顔色組合的顔色</translation>
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
        <translation>顔色 %1 被追加。</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>顏色組成 (&amp;C)...</translation>
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
        <translation>顔色轉盤 (&amp;C)...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>顏色設定幫助</translation>
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
        <translation>套用 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>取消 (&amp;C)</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>確定 (&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>無</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>自定</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>單頁</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>雙邊</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3折</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4折</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>左頁</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>中間</translation>
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
        <source>Right Page</source>
        <translation>右頁</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
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
    <message>
        <source>Monday</source>
        <translation>星期一</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>星期二</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>星期三</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>星期四</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>星期五</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>星期六</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>星期日</translation>
    </message>
    <message>
        <source>January</source>
        <translation>一月</translation>
    </message>
    <message>
        <source>February</source>
        <translation>二月</translation>
    </message>
    <message>
        <source>March</source>
        <translation>三月</translation>
    </message>
    <message>
        <source>April</source>
        <translation>四月</translation>
    </message>
    <message>
        <source>May</source>
        <translation>五月</translation>
    </message>
    <message>
        <source>June</source>
        <translation>六月</translation>
    </message>
    <message>
        <source>July</source>
        <translation>七月</translation>
    </message>
    <message>
        <source>August</source>
        <translation>八月</translation>
    </message>
    <message>
        <source>September</source>
        <translation>九月</translation>
    </message>
    <message>
        <source>October</source>
        <translation>十月</translation>
    </message>
    <message>
        <source>November</source>
        <translation>十一月</translation>
    </message>
    <message>
        <source>December</source>
        <translation>十二月</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation>None</translation>
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
        <translation>CSV 匯入器選項</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation type="unfinished">Field delimiter:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation type="unfinished">Value delimiter:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>第一列是標題</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>印表機選項</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>選項</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>數值</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation type="unfinished">頁面套</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>全部頁面</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>只要偶數頁</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>只要奇數頁</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>鏡像</translation>
    </message>
    <message>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>頁面方向</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Up 列印</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>每張紙的頁面</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>每張紙的頁面</translation>
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
        <translation>編碼:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>移動到你的檔案目錄。
這可以在偏好設定裡設定。
</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>壓縮檔案 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>包括字體 (&amp;I)</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>設置顔色組成</translation>
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
        <translation>設定 RGB (&amp;R)</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>設定 CMYK (&amp;M)</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>設定 HSV (&amp;H)</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>被使用者取消</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>刪除顔色</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>刪除顔色:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>取代上:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>刪除頁</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>到:</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>刪除來自:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>刪除樣式</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>刪除樣式:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>取代為:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>沒有樣式</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>匯入失敗</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>匯入 Word 檔案失敗
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>檔案資訊</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>標題 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>作者 (&amp;A):</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>關鍵字 (&amp;K):</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>描述 (&amp;P):</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>出版者 (&amp;U):</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>貢獻者 (&amp;C):</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>日期 (&amp;E):</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>類型 (&amp;Y):</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>格式 (&amp;O):</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>身份標記 (&amp;F):</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>來源 (&amp;S):</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>語言 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>關係 (&amp;R):</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>範圍 (&amp;V):</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>權利 (&amp;G):</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>更多資訊 (&amp;I)</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>負責制作該檔案的一個人或組織</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>負責爲該檔案內容作出貢獻的一個人或組織</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>一個和檔案存在時的某事件相關聯的日期，以ISO 8601標準表示，格式是年－月－日</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>檔案內容的性質或流派，例如類別，功能或流派等等</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>一個在特定的上下文裏對檔案的清晰參考，例如ISBN或URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>一個關於當前檔案怎麽起源的參考，例如ISBN或URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>一個關於相關檔案的參考，興許使用一個正式的身份標記，例如ISBN或者URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>檔案內容的區域或者範圍，可能的話包括地點，時間和管轄範圍</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>關於在檔案裏或對檔案擁有的權力，例如版權，專利或者商標</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>檔案 (&amp;T)</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>製作此檔案內容的主要人或組織。這個專案可以被嵌入到Scribus檔案以及PDF檔案中的元資料中備考。</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>賦予檔案的名稱。這個專案可以被嵌入到 Scribus 檔案以及 PDF 檔案中的元資料中備考</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>檔案內容的說明。這個專案是檔案的簡單描述或者摘要。這個在輸出PDF檔案時將被嵌入</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>檔案內容的主題。這個專案是你想嵌入到PDF檔案中的關鍵字，以便幫助尋找和檢索PDF檔案</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>檔案的物理或數碼的證明。媒體類型或者尺寸將值得關注。RFC2045，RFC2046在此也有用</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>書寫檔案內容所用的語言，一般上是可能帶橫杠尾碼和一個ISO-3166國家代碼（如en-GB，fr-CH）的ISO-639語言編碼</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>增加一個頁碼區域到檔案。這個新區域將被增加到目前選中的區域後面。</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>刪除目前所選擇的區域。</translation>
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
        <source>Name: Optional name for section eg. Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
        <translation>名稱：區域的可選名稱，例如：Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>檔案區域</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>From</source>
        <translation>來自</translation>
    </message>
    <message>
        <source>To</source>
        <translation>到</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>樣式</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>顯示</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>相關於</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>父級于</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>子級於</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>文字框</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>圖片框</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>布林</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>整數</translation>
    </message>
    <message>
        <source>String</source>
        <translation>字串</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>實數</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>無</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>無</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>檔案物件特性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>數值</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>參數</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>關係</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>有關係於</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>自動增加到</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
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
        <translation>設置印表機</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>儲存爲</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>青</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>洋紅</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黃</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>黑</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>插入一個以逗號隔開的記號列表,
可以 * 來代表所有頁, 1-5代表頁面的範圍,
或者單獨一個頁碼。
</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>列印到</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>選項 (&amp;O)...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>檔案 (&amp;F):</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>更改 (&amp;H)...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>另外的印表機命令 (&amp;L)</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>命令 (&amp;M):</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>範圍</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>全部列印 (&amp;A)</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>列印目前頁面 (&amp;G)</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>列印範圍 (&amp;R)</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>列印份數 (&amp;U):</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>列印 (&amp;P)</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>普通列印</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>分色列印</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>儘量用彩色列印</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>灰度列印</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript 級別 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript 級別 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript 級別 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>頁面</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>頁面水平鏡像</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>頁面垂直鏡像</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>設定介質尺寸</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>顔色</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>應用底色清除</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>把 Spot Colours 轉成 Process Colours</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>套用 ICC 配置檔案</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>進階選項</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>預覽...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>設定 PostScript 級別。
設定成級別 1或 2 可能產生巨大檔案</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript 檔案 (*.ps);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>使用其他列印管理器來使用額外的列印選項, 例如 kprinter 或者 gtklp</translation>
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
    <message>
        <source>Clip to Page Margins</source>
        <translation>裁切到頁邊</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Do not show objects outside the margins on the printed page or exported file</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>匯入文件:
%1
失敗!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>致命錯誤</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>錯誤</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation>匯入 PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>分析 PostScript:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>產生物件中</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>轉換圖片 %! 失敗!</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>編輯樣式</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>字元</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
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
        <translation>所選文字或物件的字體</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字體大小</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>文本填充顔色</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>文本輪廓顔色</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished">Determines the overall height, in line numbers, of the Drop Caps</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>段落上方的距離</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>段落下方的距離</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>跳位字元和縮進</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>名稱 (&amp;N):</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>行 (&amp;L):</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>距離</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>固定行距</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>自動行距</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>和基線網格對齊</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>首字下沈</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>到文字的距離:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>預覽段落樣式</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>確定下沈字和文字的間隔</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>切換這個段落樣式的文字範例</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>這個樣式的名稱非唯一</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>取消字元的基準線</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>自動</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>編輯器</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>打開 (&amp;O)...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>另存新檔 (&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>儲存並退出 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>退出但不儲存 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>復原 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>重做 (&amp;R)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪下 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>取得功能變數名稱 (&amp;G)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;全部檔案 (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>圖片效果</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>選項:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>顔色:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>陰影:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>亮度:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>對比:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>半徑:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>數值:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>海報效果化:</translation>
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
        <translation>顔色化</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>對比</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>反轉</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>海報效果化</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>銳化</translation>
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
        <translation>使用中效果</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
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
        <translation>全部頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>更改輸出目錄</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>輸出目錄是儲存你的圖片的地方。
輸出檔案的名稱將會是 &apos;檔案名-頁碼.文件類型&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>僅匯出目前頁面</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>可用匯出格式</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>選擇一個匯出目錄</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>更改 (&amp;H)...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>匯出到目錄 (&amp;E):</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>圖片類型 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>品質 (&amp;Q):</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>匯出成圖片</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>解析度 (&amp;R):</translation>
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
        <translation>範圍</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>目前頁面 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>範圍 (&amp;R)</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>匯出一個頁面範圍</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>插入一個以逗號隔開的記號，這裏記號可以是*以代表所有頁，1-5代表頁範圍或者一個單獨的頁碼。
</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>輸出所有頁</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>圖片的解析度。
如果圖片用於螢幕顯示, 用 72 dpi</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>圖片的品質, 100% 爲最好, 1% 爲最低</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>圖片尺寸。100% 爲沒有更改, 200% 是兩倍大, 以此類推。</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>圖像擴展屬性</translation>
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
        <translation>色調</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>飽和度</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>顔色</translation>
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
        <translation>螢幕</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>溶解</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>重疊</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>強光</translation>
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
        <translation>顔色模糊</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>顔色焦灼</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>混合模式:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>透明度:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>圖層</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>不用任何路徑</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>路徑</translation>
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
        <translation>尺寸:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>標題:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>無標題</translation>
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
        <translation>Scribus 檔案</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>解析度:</translation>
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
        <translation>灰度</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>顔色模式:</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>這個檔案使用的一些字體被替代爲:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> 已被替代成: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>可用字體</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>字體替代</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>其他路徑</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>字體名稱</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>替代</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>選擇一個目錄</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>可用字體 (&amp;A)</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>字體替代 (&amp;S)</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>其他路徑 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>更改 (&amp;H)...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>增加 (&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>移除 (&amp;R)</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>字體名稱</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>使用字體</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>嵌入在:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>子集</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>字體文件的路徑</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation>字體搜索路徑只能在偏好設定中設定，而且要在沒有打開任何檔案的情況下。關閉所有檔案，然後用編輯－設置來更改字體搜索路徑。</translation>
    </message>
    <message>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>字體名稱</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation>文件</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation>類型</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>子集</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation>Access</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation>字體尺寸:</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation>字體預覽</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation>快速尋找: </translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>尋找 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>追加選中的字體到樣式，字體功能表</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>離開預覽</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation>在此輸入字以便快速尋找字體名稱。例如&apos;粗體&apos;顯示所帶有粗體的名稱。尋找大小寫有別</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>開始尋找</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>所選字體的尺寸</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>用戶</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>系統</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>追加 (&amp;A)</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>字體預覽</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>快速尋找 (&amp;Q):</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>尋找 (&amp;S)</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation></translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>字體名稱</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>子集</translation>
    </message>
    <message>
        <source>Access</source>
        <translation type="unfinished">Access</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>字體大小 (&amp;F):</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文字</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>要顯示的文字樣本</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation></translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>重置文字</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>追加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
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
        <translation>字體預覽 (&amp;F)...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>字體預覽視窗</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>排列, 尋找和瀏覽可用的字體。</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>字體替代</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>原始字體</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>替代字體</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>讓這些替代永久有效</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>這個檔案包括一些你的系統沒有安裝的字體，請爲它們選擇合適的替代字體。按取消將停止載入檔案。</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>取消這些字體替代並停止載入檔案。</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>啓用這個就告訴 Scribus 永久性地在以後的版面使用替代字體來代替不存在的字體。這個可以在編輯－偏好設定－字體中被改變或逆轉。</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>如果你選擇確認然後儲存, 這些替代將永久保留到檔案中。</translation>
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
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>這裡你可以增加, 更改或者刪除顔色節點。</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>在這裡增加, 更改或者刪除顔色節點</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>管理參考線</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>水平參考線</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>垂直參考線</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Y-座標 (&amp;Y):</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>刪除 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>X-座標 (&amp;X):</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>增加 (&amp;D)</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>刪除 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>鎖定參考線 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>行 (&amp;R):</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>列 (&amp;O):</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>參考:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>頁面 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>頁面邊距 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>選擇 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>行和列－自動參考線</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>行距 (&amp;G)</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>列距 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>更新 (&amp;U)</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>在檔案設定參考線。參考線管理器還是打開狀態，但是更改是延續的</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>套用到所有頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>參考線</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>單位</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>預覽</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>已經有空白的(0.0)參考線</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>對不起, 沒有使用手冊可用! 請造訪: http://docs.scribus.net 來獲得最新檔案。
如果要下載, 請造訪: www.scribus.net 。</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>內容</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>鏈結</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus 線上求助</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>內容 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>尋找 (&amp;S)</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>尋找</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>尋找條件:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>尋找 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>全部刪除 (&amp;L)</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>書籤 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>列印 (&amp;P)...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>退出 (&amp;X)</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>尋找大小寫有別</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>新書籤</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>新書籤的標題:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>尋找 (&amp;F)...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>尋找下一個 (&amp;N)</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>尋找上一個 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>增加書籤 (&amp;A)</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>全部刪除 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>書籤 (&amp;B)</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>相關</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>可能連字元</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>接受</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>跳過</translation>
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
        <translation>需要用連字元的最小的詞的長度。</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>連續連字元的最大允許數目。
0 表示不限制。</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>語言 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>最小字 (&amp;S):</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>連字元提示 (&amp;H)</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>輸入時自動增加連字元到文字 (&amp;D)</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished">A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>啓用在輸入時自動用連字元。</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>允許連續連字元 (&amp;A):</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>圖片資訊</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>一般資訊</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>日期 / 時間:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>已經嵌入配置檔案:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>配置檔案名稱:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>已經嵌入路徑:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>有圖層:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF 資訊</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>作者:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>評論:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>用戶評論:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>相機型號:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>相機製造商:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>描述:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>版權:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>掃描器型號:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>掃描器製造商:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>匯入 EPS / PS (&amp;E)...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>匯入 EPS 檔案</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>匯入大部分 EPS 檔案到目前文件,
把它們的向量資料轉換爲 Scribus 物件。</translation>
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
        <translation>插入頁面</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>頁面之前</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>頁面之後</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>最後</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>頁面</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>插入 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>主頁 (&amp;M):</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面尺寸</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>頁面方向 (&amp;N):</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>高度 (&amp;H):</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>物件和它們的頁面一起移動</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>主頁</translation>
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
        <translation>行數:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>列數:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>新 Script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>編輯 JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>編輯 (&amp;E)...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>增加 (&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>新 Script (&amp;N):</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>否 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>是 (&amp;Y)</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>你真的要刪除這個 Script 嗎?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>增加一個新 Script, 預設了一個同名稱的函數。如果你想把這個 Script 作爲 &quot;打開動作&quot; Script 來用, 確保沒有更改函數的名稱。</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>動作</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>目前鍵位</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>爲這個動作選擇一個鍵位</translation>
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
        <source>&amp;No Key</source>
        <translation>沒有鍵 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>使用者自定鍵 (&amp;U)</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>設定鍵 (&amp;K)</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>可載入快捷鍵組合</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>載入 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>匯入 (&amp;I)...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>匯出 (&amp;E)...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>重置 (&amp;R)</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>可載入的快捷鍵組合</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>載入選擇的快捷鍵組合</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>匯入一個快捷鍵組合到目前設置中</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>匯出目前快捷鍵到一個可被匯入的檔案中</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>重新載入 Scribus 預設的快捷鍵</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>鍵位組合 XML 檔案 (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>這個鍵位組合已經被使用</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation>Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation>Meta+</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>圖層</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>刪除圖層</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>你確定要一併刪除此圖層上的所有物件嗎?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>增加圖層</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>刪除圖層</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>升高圖層</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>降低圖層</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>編輯線條樣式</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copy of %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>新樣式</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>檔案 (*.sla *.sla.gz *.scd *.scd.gz);;所有檔案 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>檔案 (*.sla *.scd);;所有檔案 (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>複製 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>否 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>是 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>你真的要刪除此樣式嗎?</translation>
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
        <translation>線條寬度:</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>平頭</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>方頭</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>圓頭</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation type="unfinished">斜角連接</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation type="unfinished">圓角連接</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>所有檔案 (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>選擇範例文字</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>作者:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>獲得更多:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML 檔案:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>範例文字</translation>
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
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Margin Guides</source>
        <translation>頁面邊緣參考線</translation>
    </message>
    <message>
        <source>Manage Page Properties</source>
        <translation>管理頁面屬性</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面尺寸</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>頁面方向 (&amp;N):</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>高度 (&amp;H):</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>把物件和頁面一起移動</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>類型:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>底部 (&amp;B):</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>頂部 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>右 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>左 (&amp;L):</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>頂部邊距線到頁邊的距離</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>底部邊距線到頁邊的距離</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>裡面 (&amp;I):</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>外面 (&amp;U):</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>預設版面:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>應用頁邊距設定到所有頁面</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>應用頁邊距設定到檔案中現存的所有頁面</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>左邊頁邊邊距到頁邊的距離。如果對面頁被選擇, 這個距離可以用來獲得正確的裝訂邊距</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>右邊頁邊邊距到頁邊的距離。如果對面頁被選擇, 這個距離可以用來獲得正確的裝訂邊距</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>印表機頁面邊距...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>從可用印表機匯入所選頁面的頁面邊距。</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>編輯主頁</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>你確認要刪除這個主頁嗎?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>否 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>是 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>名稱 (&amp;N):</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>新建主頁</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copy of %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名稱:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>新建主頁</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>複製 #%1 的 </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>複製目前選擇的主頁</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>刪除目前選擇的主頁</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>增加一個新的主頁</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>從另外一個檔案匯入主頁</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>新建主頁 %1</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>無法重新命名主頁</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>重新命名主頁</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>新名稱:</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>多重複製</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>複製數量 (&amp;N):</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>水平位移 (&amp;H):</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>垂直位移 (&amp;V):</translation>
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
        <translation>DX:</translation>
    </message>
    <message>
        <source>DY:</source>
        <translation>DY:</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>角度:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>長度:</translation>
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
        <translation>打開</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>檔案 (*.sla *.sla.gz *.scd *.scd.gz);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>檔案 (*.sla *.scd);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>匯入頁面</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> 來自 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>建立頁面</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> 來自 %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>匯入主頁</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>來自檔案 (&amp;F):</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>更改 (&amp;C)...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>匯入頁面 (&amp;I):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>匯入主頁 (&amp;I)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>頁面之前</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>頁面之後</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>最後</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>插入一個以逗號隔開的標記, 這裏一個記號可以是* 來表示全部頁面, 1-5表示頁面範圍, 或者輸入單個頁碼。</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>丟失字體</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>字體 %1 沒有安裝。</translation>
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
        <translation>移動頁面</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>複製頁面</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>移動頁面:</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>移動頁面</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>頁面之前</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>頁面之後</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>最後</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>到:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>複製數量:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>屬性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
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
        <translation>基點</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>圖層</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>形狀：</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>文字距離</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>顯示曲線</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>開始位移:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>到曲線的距離:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>  %</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>輸入配置檔案:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation type="unfinished">渲染意圖:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>知覺</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相對比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>飽和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>絕對比色</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>左點</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>尾點</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>尖角連接</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>斜角連接</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>圓角連接</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>平頭</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>方頭</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>圓頭</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>沒有樣式</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字體大小</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>行距</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>所選物件名稱</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>當前基點的水平位置</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>當前基點的垂直位置</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>寬度</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>高度</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>在當前基點旋轉物件</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>表示尺寸和旋轉角度的點</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>選擇左上爲基點</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>選擇右上爲基點</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>選擇左下爲基點</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>選擇右下爲基點</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>選擇中間爲基點</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>水平翻轉</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>垂直翻轉</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>上升一級</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>下降一級</translation>
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
        <translation>鎖定或解鎖物件</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation type="unfinished">鎖定或者解鎖物件的尺寸</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>啓用或者禁用物件的列印</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="unfinished">選中的文字或者物件的字體</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">字元寬度縮放</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>文字輪廓顔色的飽和度</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>文字填充色的飽和度</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">當前段落的樣式</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation type="unfinished">更改左或尾點的設置</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>線條樣式</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>線條粗細</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>線條類型</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>線條端類型</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation type="unfinished">Line style of current object</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation type="unfinished">選擇框體的形狀...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation type="unfinished">編輯框體的形狀...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation type="unfinished">設置圓角幅度</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation type="unfinished">文字框中的列數</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation type="unfinished">列間距</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>文字和框體頂部的距離</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>文字和框體底部的距離</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>文字和框體左邊的距離</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>文字和框體右邊的距離</translation>
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
        <translation>Make text in lower frames flow around the object shape</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished">Switches between Gap or Column width</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>列寬</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>形狀 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>文字 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>圖片 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>線條 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>顔色 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>X-座標 (&amp;X):</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Y-座標 (&amp;Y):</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>高度 (&amp;H):</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>旋轉 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>編輯形狀 (&amp;E)...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>圓角 (&amp;O):
</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">&amp;M 列：</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">間距 (&amp;G):</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>頂部 (&amp;P):</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>底部 (&amp;B):</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>左 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>右 (&amp;R):</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;A 跳位字元...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>&amp;F 字體環繞框</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>&amp;B 使用限制方框</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;U 使用輪廓線條</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>樣式 (&amp;Y):</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>語言 (&amp;G):</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>自由縮放 (&amp;F)</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-縮放 (&amp;A):</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-縮放 (&amp;E):</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation type="unfinished">&amp;T 縮放到框體大小</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation type="unfinished">&amp;R 按比例</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished">&amp;B 基點：</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>線條類型 (&amp;Y):</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>線條寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>邊緣 (&amp;G):</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="unfinished">&amp;E 結尾：</translation>
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
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Use a surrounding box instead of the frame&apos;s shape for text flow</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Use a second line originally based on the frame&apos;s shape for text flow</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="unfinished">框裏連字元語言</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation type="unfinished">右到左書寫</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished">手工追蹤</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation type="unfinished">箭頭：</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation type="unfinished">箭尾：</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished">字元底線位移</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished">字元高度縮放</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished">固定行距</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished">自動行距</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">與底線網格對齊</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>實際 X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>實際 Y-DPI:</translation>
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
    <message>
        <source>Fill Rule</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>非零</translation>
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
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
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
    <message>
        <source>Auto</source>
        <translation>自動</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable or disable exporting of the object</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation type="unfinished">編輯樣式</translation>
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
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">實線</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">虛線</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">點線</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">虛點線</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">虛點點線</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished">名稱 &quot;%1&quot; 不唯一。&lt;br/&gt;請選擇另外一個</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>進度</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>總進度:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>取消 (&amp;C)</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>我的插件 (&amp;P)</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - 我的插件</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>插件可作用!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>新建檔案</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面大小</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>頁面邊緣參考線</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>檔案的頁面大小, 標準尺寸或者是自定尺寸</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>檔案頁面的方向</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>檔案的頁面寬度, 當你選擇自定頁面尺寸時, 可以修改它</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>檔案的頁面高度, 當你選擇自定頁面尺寸時, 可以修改它</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>檔案的起始頁碼</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>編輯檔案時預設使用的尺寸單位</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>當新頁面增加時自動建立文字框</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>自動建立的列的列距</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>自動建立的文字框裡的列數</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>頁面方向 (&amp;N):</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>高度 (&amp;H):</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>起始頁碼 (&amp;I):</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>預設單位 (&amp;D):</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>自動文字框 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>間距 (&amp;G):</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>列 (&amp;M):</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>不再顯示此對話框</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>檔案起始頁數</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>頁面數 (&amp;U):</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>新建檔案 (&amp;N)</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>打開現有檔案 (&amp;E)</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>打開最近的檔案 (&amp;D)</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>從模板新建 (&amp;F)...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>以預設的版面載入檔案</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>從一個別人或自己創建的模板中開創檔案。（例如，如果你的檔案有固定的樣式）</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>節點</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>移動節點</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>移動控制點</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>增加節點</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>刪除節點</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>重置控制點</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>重置這個節點</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>絕對座標 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>X-座標 (&amp;X):</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Y-座標 (&amp;Y):</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>編輯輪廓線 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>重置輪廓線 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>結束編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>獨立移動控制點</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>對稱移動控制點</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>打開一個多邊形或剪下貝茲 (Bezier) 曲線</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>關閉這個貝茲 (Bezier) 曲線</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>水平鏡像路徑</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>垂直鏡像路徑</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished">向左水平切變路徑</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished">向上垂直切變路徑</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished">向下垂直切變路徑</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="unfinished">逆時針旋轉路徑</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation type="unfinished">順時針旋轉路徑</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>按所示減少路徑尺寸 %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished">按所示增加路徑尺寸 %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation type="unfinished">旋轉角度</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% 來增加或減少</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished">啟動輪廓線條編輯模式</translation>
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
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>這個檔案似乎不是 OpenOffice Draw 檔案。</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>匯入OpenOffice.org Draw (&amp;O)...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>匯入 OpenOffice.org Draw 檔案</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished">Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished">用檔案名稱作爲段落樣式的字首</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>下次不要再詢問</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished">啓用這個將覆蓋當前Scribus中的現存樣式</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished">合併段落樣式</translation>
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
        <translation>覆蓋段落樣式</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>OpenDocument 匯入器選項</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus 檔案</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x 檔案</translation>
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
        <translation>線條寬度</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>儲存爲 PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>輸出到檔案 (&amp;U):</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>更改 (&amp;N)...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>逐頁輸出單個檔案 (&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF 檔案 (*.pdf);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>儲存 PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>輸出主頁:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>輸出頁面:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>輸出目前頁面的物件:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>列印預覽</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>顯示檔案中的透明度和透明物件。需要 Ghostscript 7.07 或更高版本</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>不用RGB顔色而以CMYK墨水來類比列印預覽</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>啓用 / 停用 C (青) 墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>啓用 / 停用 M (洋紅) 墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>啓用 / 停用 Y (黃) 墨水板</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>啓用 / 停用 K (黑) 墨水板</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>&amp;T 反鋸齒文本</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>&amp;T 反鋸齒圖像</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>顯示透明度 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>顯示 CMYK (&amp;D)</translation>
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
        <translation>清除底色 (&amp;U)</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>分離名稱</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>青</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>洋紅</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黃</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>黑</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>縮放:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>列印...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>在閱讀器中提供更加悅目的文本顯示，代價是預覽稍微緩慢。這只影響Type 1字體</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>在預覽中提供了對TrueType字體，OpenType字體，EPS，PDF和向量圖更加悅目的顯示，代價是預覽速度輕微的緩慢。</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>A way of switching off some of the grey shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the grey. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>調整頁面的伸縮比例</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>正處理主頁中：</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>匯出頁面：</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>圖片</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文字</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>線條</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>多邊形</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>線套</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>路徑文字</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Copy of</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>相對於</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>是...的父級</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>是...的子級</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>頁面物件特性</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>數值</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>參數</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>關係</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>關係於</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>確定 (&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>取消 (&amp;C)</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>頁面排版</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>第一頁是:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>文件樣式</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>雙面</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>中右</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>把頁面或者主頁拖拉到垃圾箱裏來刪除它們</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>這是你所有的主頁。要建立新頁, 請把主頁拖拉到下面的頁面視圖</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>管理頁面</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>可用主頁:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>檔案頁:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 的 %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 的 %1</translation>
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
        <translation type="unfinished">中等</translation>
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
        <translation>結果</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>搜尋結果: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>預覽</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>選擇</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Goto</source>
        <translation>跳到</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>遺失</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>尋找</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>路徑</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>頁面</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>列印</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>狀態</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>取消尋找</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>管理圖片</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - 圖像尋找</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>尋找失敗: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>找不到叫做 &quot;%1&quot; 的圖片。</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>選擇一個尋找的基礎目錄</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>另存為圖片 (&amp;I)...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>匯出成圖片</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>把選中的頁面匯出爲點陣圖。</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>找不到插件</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>未知的錯誤</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">不能找到標記 (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>插件: 正在載入 %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation type="unfinished">init failed</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>未知的插件類型</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>插件 %1 已經載入</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation type="unfinished">插件： %1 不能載入： %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>插件管理器</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>插件</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation type="unfinished">如何運行</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">類型</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>載入嗎?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation type="unfinished">插件ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">文件</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished">你必須重新啓動程式來應用更改。</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>多邊形屬性</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>角數 (&amp;E):</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>旋轉 (&amp;R):</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>套用因式 (&amp;F)</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>因式 (&amp;F):</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>多邊形的角數</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>多邊形的旋轉角度</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>套用凹凸大小來改變多邊形的形狀</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">多邊形樣本</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished">一個負數值將會使多邊形變爲凹形（或者星形），一個正數值將使它成爲凸形。</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>偏好設定</translation>
    </message>
    <message>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>參考線</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>排版</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>寄存本</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>顯示</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>圖形介面</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>路徑</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面大小</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>頁面邊緣參考線</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>自動儲存</translation>
    </message>
    <message>
        <source>min</source>
        <translation>分鐘</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>其他選項</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>預覽</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>小</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>中</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>選擇一個目錄</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>外部工具</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>圖片處理工具</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>列印</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>功能表和視窗的預設字體大小</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>檔案編輯預設的長度單位</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>滑鼠滾輪每次滾動移動的行數</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>在檔案功能表裡顯示最近編輯檔案的數目</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>預設的檔案目錄</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>預設 Scripter scripts 目錄</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>預設頁面尺寸, 標準或是自定的尺寸</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>預設檔案頁面方向</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>檔案的頁面寬度, 當你選擇自定頁面尺寸時, 可以修改它</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>檔案的頁面高度, 當你選擇自定頁面尺寸時, 可以修改它</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>自動儲存的時間間隔</translation>
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
        <translation>Do not show objects outside the margins on the printed page or exported file</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Save the scrapbook contents everytime after a change</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>佈景主題 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>滾輪跳行 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>最近打開的檔案 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>檔案 (&amp;D):</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>更改 (&amp;C)...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>ICC配置文件 (&amp;I):</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>更改 (&amp;H)...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>更改 (&amp;A)...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>方向 (&amp;N):</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>高度 (&amp;H):</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>底部 (&amp;B):</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>頂部 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>右 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>左 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>間隔 (&amp;I):</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>&amp;V 更改時儲存內容</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>大</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>用頁面邊緣顔色標出非列印區域 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>調整顯示尺寸 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished">&amp;N 可執行文件的名稱：</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished">&amp;T 反鋸齒文本</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished">&amp;G 反鋸齒圖形</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished">&amp;E 可執行文件的名字：</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>&amp;M 按頁邊距線裁剪</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>&amp;U 應用底色清除</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>更改 (&amp;N)...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>語言 (&amp;L):</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>文件模板 (&amp;E):</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>單位:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>復原/重做</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>儲存的歷史動作長度</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished">連字元</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>字體</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>色彩管理</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF 輸出</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished">檔案物件特性</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">內容和索引</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>鍵盤快捷鍵</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">頁面顯示</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>顔色:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">顯示圖像</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">顯示文本鏈</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">顯示框體</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">草稿區</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>其他</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>插件</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">開閉框體顯示</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">開閉圖片顯示</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">檔案模板的額外目錄</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Ghostscript解析器在文件系統裏的位置</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished">載入檔案是總是詢問是否取代字體</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">顯示文本控制符</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">當編輯樣式時顯示當前段落樣式的預覽</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">要調節顯示，拖拉尺規下面的滑動按鈕</translation>
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
        <translation type="unfinished">印前檢驗器</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>解析度:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">範例文本</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">總是使用標準的範例文本 </translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>段落數:</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">尺規相對於頁面</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished">頁面之間的間距</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>水平:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>垂直:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>啓動時顯示對話框</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>定位 Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished">定位圖像編輯器</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished">PostScript解析器</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">開閉連鎖框體顯示。</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">選擇 Scribus 預設使用的語言。空白則預設使用系統環境設定。你還可以在命令行運行Scribus時給出選項。</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>字體大小 (功能表) (&amp;F):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>字體大小 (工具視窗) (&amp;P):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">工具視窗的預設尺寸大小</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>當啟用時, Scribus 會自動在每個時間間隔以 .bak 為副檔名儲存一份檔案的備份</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>設定儲存的歷史動作長度。設定為 0 則表示不限制儲存長度上限。</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished">File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>預設 (&amp;D)</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>儲存...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>儲存偏好設定</translation>
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
        <translation type="unfinished">遷移舊的Scribus設置</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished">Scribus檢測到現存的Scribus 1.2的設置文件。
你想要遷移他們到新的Scribus版本中嗎？</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>單頁</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation>雙頁</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>左頁</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>右頁</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3折</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>中間</translation>
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
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation type="unfinished">不能打開偏好設定文件 &quot;%1&quot; 來寫入 %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation type="unfinished">寫入 &quot;%1&quot; 失敗： QIODevice 狀態代碼： %2</translation>
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
        <translation>錯誤寫入偏好設定</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation type="unfinished">Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>載入設定時出錯</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus 無法載入偏好設定: &lt;br&gt;%1&lt;br&gt; 將載入預設設置。</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>雜誌</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>斐波納契</translation>
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
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>打開 (&amp;O)...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>退出 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>執行 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>儲存輸出 (&amp;S)...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Script 控制臺</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation type="unfinished">在此寫你的命令。一個選擇作爲腳本運行</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>你的 Script 輸出</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python Scripts (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>將 Python 命令儲存於檔案中</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>文字檔案 (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished">儲存當前輸出</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>另存新檔 (&amp;A)...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>以控制臺模式執行 (&amp;C)</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus Python 控制臺</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished">這個起源於標準的Python控制臺，所以它有一些局限性，特別是在空白空格的時候。更多資訊請參考Scribus手冊。</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>打開 Python Script 檔案</translation>
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
        <translation>紅 (&amp;R):</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>綠 (&amp;G):</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>藍 (&amp;U):</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lpha 通道:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>基本顔色 (&amp;B)</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>自定顔色 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>設定自定顔色 (&amp;D)&gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>增加到自定顔色 (&amp;A)</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>選擇顔色</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>複製或移動檔案</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>讀取: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>寫入: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>檔案名 (&amp;N):</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>檔案類型 (&amp;T):</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>上一層目錄</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>全部檔案 (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>大小</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>類型</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>日期</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>特性</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>尋找 (&amp;L): </translation>
    </message>
    <message>
        <source>Back</source>
        <translation>退回</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>建立新目錄</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>清單</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>詳細資料</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>預覽檔案資訊</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>預覽檔案內容</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>可讀寫</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>唯讀</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>只可寫入</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation type="unfinished">不可達</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>指向檔案的鏈結</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>指向目錄的鏈結</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>指向特別物件的鏈結</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>目錄</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>特別物件</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>打開 (&amp;O)</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>重新命名 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>重新載入 (&amp;E)</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>按名稱排序 (&amp;N)</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>按大小排序 (&amp;S)</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>按日期排序 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>不排序 (&amp;U)</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>排序</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>顯示隱藏檔案 (&amp;H)</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>目錄</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>鏈結</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>刪除 %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;你真的要刪除 %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>是 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>否 (&amp;N)</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>新檔案夾 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>新建檔案夾</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>新檔案夾 %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>尋找檔案夾</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>目錄</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>儲存</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>錯誤</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
檔案未找到。
請檢查路徑和檔案名。</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>全部檔案 (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>選擇一個目錄</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>目錄:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>字體 (&amp;F)</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>字體樣式 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>尺寸 (&amp;S)</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>特效</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>刪除線 (&amp;K)</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>底線 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>顔色 (&amp;C)</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>範例</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Scr&amp;ipt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>套用</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>選擇字體</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>全選</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>復原 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>重做 (&amp;R)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪下 (&amp;t )</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
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
        <translation>自定...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>起始中...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>你真的要覆蓋檔案:
%1 ?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG 圖片 (*.svg *.svgz);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG 圖片 (*.svg);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>是</translation>
    </message>
    <message>
        <source>No</source>
        <translation>否</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>另存爲圖片</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">寫入輸出文件錯誤。</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>輸出成功。</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>檔案已存在, 要覆蓋嗎?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>已經存在, 要覆蓋嗎?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="unfinished">全部是</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>所有支援的格式 (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>全部檔案 (*)</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>報紙</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>小冊子</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>目錄</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>傳單</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>標誌</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>卡片</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation type="unfinished">信頭</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>信封</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>名片</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>日曆</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>廣告</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>標簽</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>功能表</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>程式</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF 表單</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF 報告</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>雜誌</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>海報</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>公告</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>文字檔案</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>折頁</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation type="unfinished">自己模板</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>匯入文字</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>全部支援的格式</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML 檔案</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>文字檔案</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>以逗號分開數值的文件</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV 資料</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV 標頭</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>字體 %1 損壞, 將忽略之</translation>
    </message>
    <message>
        <source>External Links</source>
        <translation>外部鏈結</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation type="unfinished">文本篩檢程式</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished">媒質</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>阿爾巴尼亞語</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation type="unfinished">巴斯克語</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="unfinished">保加利亞語</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>巴西語</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="unfinished">加態隆語</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>中文 (簡體中文)</translation>
    </message>
    <message>
        <source>Traditional Chinese</source>
        <translation>中文(繁體中文)</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>捷克語</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>丹麥語</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>荷蘭語</translation>
    </message>
    <message>
        <source>English</source>
        <translation>英語</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>英式英語</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>世界語</translation>
    </message>
    <message>
        <source>German</source>
        <translation>德語</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>芬蘭語</translation>
    </message>
    <message>
        <source>French</source>
        <translation>法語</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>加利西亞語</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>希臘語</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>匈牙利語</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>印尼語</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>義大利語</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>韓語</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>立陶宛語</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>挪威語 (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>挪威語 (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>挪威語</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>波蘭語</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>俄語</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>瑞典語</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>西班牙語</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>西班牙語 (拉丁)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>斯洛伐克語</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>斯洛文尼亞語</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation type="unfinished">塞爾維亞語</translation>
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
        <translation>目標不是一個圖片框。</translation>
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
        <translation>OpenOffice.org Draw (*.sxd);;All Files (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer 檔案</translation>
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
        <translation>阿富汗語</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>土耳其語</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>烏克蘭語</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>威爾士語</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>檔案名必須是個字串。</translation>
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
        <translation>字型不存在。</translation>
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
        <translation>圖層不存在。</translation>
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
        <translation type="unfinished">克羅地亞語</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>葡萄牙語</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>葡萄牙語 (BR)</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>頁面</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>主頁</translation>
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
        <translation type="unfinished">Comm10E</translation>
    </message>
    <message>
        <source>DLE</source>
        <translation type="unfinished">DLE</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished">輸出流不可寫</translation>
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
        <translation>Font %1 (found using fontconfig) is broken, discarding it</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribus 開發中版本</translation>
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
        <translation type="unfinished"> p</translation>
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
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source>cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>點 (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>毫米 (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>英寸 (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">十二點活字 (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>釐米 (cm)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>檔案存在</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>取代 (&amp;R)</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>檔案模板: </translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>無法打開檔案。</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python ??</comment>
        <translation>Failed to save document.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>參數必須是頁面物件名稱，或者是 PyCObject 實例</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>屬性不存在</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation type="unfinished">沒找到子級</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished">無法轉換結果類型 &apos;%1&apos;.</translation>
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
        <translation>找不到物件。</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>找不到樣式。</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Cannot set style on a non-text frame.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>儲存至 EPS 失敗。</translation>
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
        <translation>線條寬度超出範圍, 必須在 0 &lt;= line_width &lt;= 12。</translation>
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
        <translation>找不到線條樣式。</translation>
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
        <translation>執行 Script (&amp;E)...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>顯示控制臺 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>關於 Script (&amp;A)...</translation>
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
        <translation>最近的 Script (&amp;R)</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>關於 Script</translation>
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
        <translation>Freetype2 庫不可用 </translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>字體 %1 損壞, 將不嵌入</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished">字體 %1 破損 （查看流），將不嵌入</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>字體 %1 損壞 (FreeType2), 將忽略它</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>字體 %1 損壞 (no Face), 將忽略它</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>字體 %1 有破損字元 %2 (字元碼 %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>字體 %1 損壞而且將被忽略</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>無法讀取字體 %1 , 將不嵌入</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>讀取字體 %1 失敗 - 無法辨識的字體型態</translation>
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
        <translation>Scribus 崩潰</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus崩潰了，因由信號  #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>確定 (&amp;O)</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished">不能群組少於 2 個物件</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation type="unfinished">群組需要物件選擇範圍或者參數列表</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished">不能群組少於 2 個物件</translation>
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
        <translation type="unfinished">Cicero (c)</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>檔案名不應該是一個空白字串。</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>頁</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copy #%1 of</translation>
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
        <translation>洋紅</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>黃</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>顔色轉盤</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>字體預覽</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>我的插件</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>從模板新建</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>匯出成圖片</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS / EPS 匯入器</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>另存爲模板</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Scripter</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>匯出 SVG</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>匯入 SVG</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Draw 匯入器</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus崩潰了, 因由以下異常: %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>建立字體快取</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>發現新字體, 驗證中...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>發現更改過的字體, 驗證中...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>讀取字體快取</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>寫入已更新字體快取</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>搜尋字體</translation>
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
        <translation>&lt;br&gt;名為 &apos;%1&apos; 的檔案已存在&lt;br/&gt;你想要以你正在儲存的檔案取代它嗎?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished">firstPageOrder is bigger than allowed.</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>舊 .sla 格式支援</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>德語 (傳統)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>輸出 PostScript 檔案</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>列印檔案</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished">&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>建立 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>泰語</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>條碼產生器</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>儲存檔案失敗。</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;所有檔案(*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word 檔案</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB 檔案</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB 資料</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>PDB 匯入</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>無法打開檔案 %1</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>盧森堡語</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation type="unfinished">阿拉伯字元</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>日語</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>冰島語</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Breton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation>美式英語</translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation>澳式英語</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation>德語 (瑞士)</translation>
    </message>
    <message>
        <source>Chinese (Trad.)</source>
        <translation>中文 (正體中文)</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
外部連結
</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>清除</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>全選</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>復原 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>重做 (&amp;R)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪下 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation type="unfinished">系統功能表</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">陰暗</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation type="unfinished">取消陰暗</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>正常化</translation>
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
        <translation>關閉</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>復原 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>移動 (&amp;M)</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>尺寸 (&amp;S)</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>&amp;N 最小化</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>&amp;X 最大化</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>至於頂部 (&amp;T)</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>最小化</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>往下恢復</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation type="unfinished">&amp;A 陰暗</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
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
        <translation type="unfinished">檔案設定</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>頁面邊緣參考線</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>頂部 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>左 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>底部 (&amp;B):</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>右 (&amp;R):</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面尺寸</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;N 頁面方向</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;H 高度：</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>單位 (&amp;U):</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>自動儲存</translation>
    </message>
    <message>
        <source>min</source>
        <translation>分鐘</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>間隔 (&amp;I):</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">檔案</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>參考線</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>頁面顯示</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>顔色:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>&amp;U 以頁邊色顯示不可列印區域</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>顯示圖片</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">顯示文本連鎖</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>顯示框體</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>顯示</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>排版</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>連字元</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>字體</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF 輸出</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>檔案物件特性</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">內容和索引</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>色彩管理</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">開閉框體顯示</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">開閉圖片顯示</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>紙張顔色</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>把頁邊距外的區域用頁邊距顔色蒙上</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">檔案資訊</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">顯示文本控制符</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">顯示文本框裏的非列印字元如段落標記</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>印前檢驗器</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">尺規相對應於頁面</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished">最小草稿區</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>頁面之間的間距</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>水平:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>垂直:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>調整顔色</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished">啓用/關閉文本框連鎖顯示</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>應用尺寸設定到所有頁面</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished">區域</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">應用頁面尺寸改變到檔案中現有全部頁面</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripts (*.py);; 全部檔案 (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>以延伸 Script 來執行</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>樣式管理者</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation></translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation></translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名稱:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>確定 (&amp;K)</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation></translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>套用 (&amp;P)</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation></translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>取消 (&amp;N)</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>屬性</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>線條</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>目前段落的樣式</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>樣式設定</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>文本填充色</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>文本填充色的飽和度</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>填充色設置</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>文本輪廓的顔色</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>文本輪廓的顔色飽和度</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>輪廓顔色設置</translation>
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
        <translation>選中文字的字體</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字體大小</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>字元寬度縮放</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>字體設定</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>字元高度縮放</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>字元設定</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>手動追蹤</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>儲存頁面爲 SVG (&amp;S)...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>匯出 SVG 檔案</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>把目前頁面匯出成一個 SVG 檔案。</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>匯入 SVG (&amp;S)...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>匯入 SVG 檔案</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>匯入大部分 SVG 檔案到目前文件,
把它們的向量資料轉換爲 Scribus 物件。</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>可縮放向量圖 (SVG)</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG 檔案包含了一些不被支援的功能</translation>
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
        <translation>應用不間斷的空格於:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>所選框體 (&amp;S)</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Active &amp;page</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>所有物件 (&amp;A)</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>只處理了所選的框體。</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Only actual page processed.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>已處理檔案中所有的物件。</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>使用者設定</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>系統範圍設定</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>儲存 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>重置 (&amp;R)</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>儲存用戶設定</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>重新載入系統範圍的設定並清除使用者定義的設定</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>編輯自定的設置。如果你儲存了它, 它將覆蓋系統範圍的設置</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Short Words</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>使用者設定已經存在。你真的要覆蓋它嗎?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>不能寫入檔案 %1 。</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>使用者設定已儲存</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>已重新載入系統範圍的設定</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>不能打開文件 %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>儲存爲模板 (&amp;T)...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>把檔案儲存爲模板</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>把檔案儲存爲模板。輕鬆創建具有類似外觀的文件的一個好方法。</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>持續</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>動作</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>載入/儲存/匯入/匯出</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right</source>
        <translation>右</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>底部</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>左</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation type="unfinished"></translation>
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
        <source>Floating Orientation...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>列印中...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copy #%1 of</translation>
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
        <translation>檔案顔色</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>新圖層</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>你真的想清除你所有的文字嗎?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>不能刪除正在使用的物件</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>物件 %1 正被故事編輯器使用中。刪除操作將被取消</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>啓動插件</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>啓動鍵盤快捷鍵</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>讀取偏好設定</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>啓動故事編輯器</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>讀取 ICC 配置檔案</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>啓動連字元器</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>讀取寄存本</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>設定快捷鍵</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>編輯</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>尋找字體</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>系統中沒有找到字體。</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>現在退出。</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>致命錯誤</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>字體系統已被啓動</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>打開最近的檔案 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>匯出 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>樣式 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>顔色 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>尺寸 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>陰暗 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>字體 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>效果 (&amp;E)</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>物件 (&amp;I)</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>預覽設定</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">圖層</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>傳送到圖層 (&amp;Y)</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>PDF 選項 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>形狀 (&amp;S)</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>轉換爲 (&amp;O)</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>插入 (&amp;N)</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>字元</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>引號</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>空間</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>頁面 (&amp;P)</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>檢視 (&amp;V)</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>附加命令 (&amp;X)</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>視窗 (&amp;W)</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>說明 (&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>對齊 (&amp;A)</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>就緒</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>匯入頁面...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>匯入頁面</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>匯入結束</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>沒有找到任何可匯入的東西</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>檔案 %1 的格式不被接受</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>載入...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>這個檔案使用的一些 ICC 配置檔案沒有安裝:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> 被取代爲: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(被轉換)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>所有支援的格式</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>所有檔案 (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>不能寫入檔案:
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>檔案 (*.sla *.sla.gz *.scd *scd.gz);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>檔案 (*.sla *.scd);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>儲存中...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus 偵測到一些錯誤。考慮使用印前檢驗器來修正它們</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>忽略 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>退出 (&amp;A)</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>列印中...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>列印失敗!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>不能剪下正在使用的物件</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>物件 %1 目前正被故事編輯器編輯中, 剪下操作將被取消</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>關於 Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus 使用手冊</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>文字檔案 (*.txt);;全部檔案(*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>普通</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名稱:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>把頁面轉換爲主頁</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>新建主頁</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>尺寸 (&amp;S):</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>尺寸</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>陰暗 (&amp;S):</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>陰暗</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>沒有樣式</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>以下段落遺失:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: 你不能使用 ESP 圖片或者列印預覽</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus 偵測到一些錯誤。
考慮使用印前檢驗器  來修正它們。</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS 檔案 (*.eps);;所有檔案 (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>偵測到一些錯誤。
考慮使用印前檢驗器來糾正它們</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-頁面%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>某些物件被鎖定</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>全部鎖定 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>全部解除鎖定 (&amp;U)</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>資訊</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>程式 %1 已經在執行!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>程式 %1 遺失!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>所選的顔色在檔案的顔色組合中不存在。請爲這個新顔色輸入一個名字。</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>顔色不存在</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>你爲新顔色選擇的名承已經存在。請輸入一個新名稱。</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="unfinished">圖層 (&amp;L)</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>傳送到圖層</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>預覽設定 (&amp;W)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>工具 (&amp;T)</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-座標:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-座標:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: 你不能使用 ESP 圖片或者 PostScript 列印預覽</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript 遺失: Postscript 列印預覽不可用</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>連字</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>新主頁 %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>複製數目: %1
水平位移: %2
垂直位移: %3</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>你真的想取代現存的圖片嗎?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>內容</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>連字 (&amp;T)</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>無效參數: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>檔案 %1 不存在, 中止。</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>用法: scribus [選項...] [檔案]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>選項:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>列印求助 (這個資訊) 然後退出</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>使用 xx 作爲一個語言的快捷鍵, 例如 &apos;en&apos; 或者 &apos;de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>列出目前已經安裝的介面語言</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>在控制臺顯示字體何時被載入的資訊</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>在啓動時不顯示啓動視窗</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>輸出版本資訊然後退出</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>使用從右到左的對話框按鈕順序 (例如: 取消 / 否 / 是, 而不是 是 / 否 / 取消）</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>檔案名</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation type="unfinished">用文件名作爲用戶給出的偏好設定路徑</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Scribus 已經安裝的介面語言如下:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>覆蓋預設語言選擇:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="unfinished">scribus -l xx 或者 scribus --lang xx，這裏xx是語言選擇</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus 版本</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, 開放原始碼的桌面出版軟體</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>首頁</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>問題</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>顯示控制臺視窗</translation>
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
        <translation>圖層</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>複製到這裏</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>移動到這裏</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>圖片</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>檔案: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>原始 PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>實際 PPI:  </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>串聯的文字</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>文字框</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>路徑上的文字</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>段落: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>字: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>字元: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>列印：</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>已啓用</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>已禁用</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>資訊 (&amp;F)</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>預覽設定</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF 選項 </translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>傳送到圖層 (&amp;Y)</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>圖層 (&amp;V)</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>轉換爲 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>不能刪除正在使用的專案</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>專案 %1 正被故事編輯器使用中。刪除操作將被取消。</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>串聯文字框</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>你正在嘗試鏈結到一個已填充框，或者本框</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>你真的要清除所有的文本嗎？</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>不能轉換正在使用的物件</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>物件 %1 目前正在被故事編輯器編輯。把目前物件轉換爲曲線的操作將被跳過</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>頁面 %1 到 %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>顔色模式: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
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
        <translation>灰度</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>內容</translation>
    </message>
    <message>
        <source>Export: </source>
        <translation>匯出: </translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>檔案:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>自上次儲存已被更改過。</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>放棄 (&amp;D)</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>嵌入的 Python scripting 支援。</translation>
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
        <translation>Script 錯誤</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>如果你正在執行一個官方的 script, 請在 &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; 報告。</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>這個資訊同時存在于剪下班。使用Ctrl+V來貼上到臭蟲追蹤器裏面</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>檢驗 Script</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>在嘗試你所輸入的命令是出現一個內部錯誤。詳細資訊被列印到標準輸出了。</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>設定 Python 插件失敗。具體錯誤資訊已被列印到標準輸出。</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Python Scripts (*.py);;All 檔案 (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Scripter 偏好設定</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>啓用延伸 Script</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>延伸</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>控制臺</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>啓動運行 Script:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>錯誤:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>說明:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>關鍵字:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>簽名:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>數位:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>字串:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>基本文字:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>選擇顔色</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>更改...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>顯示頁面預覽</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>尋找/取代</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>尋找：</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文本</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>段落樣式</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>字體</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>字體尺寸</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>字體效果</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>填充顔色</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>填充陰影</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>邊顔色</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>邊陰暗</translation>
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
        <translation>區塊</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>強制</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>取代為:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>尋找結束</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>整個字 (&amp;W)</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>忽略大小寫 (&amp;I)</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>尋找 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>取代 (&amp;R)</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>全部取代 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>關閉 (&amp;C)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>尋找結束, 找到 %1 個符合</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>選擇域</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>可用域</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>選中域</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
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
        <translation>其他...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>陰暗</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;S 陰暗：</translation>
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
        <translation>X-移位</translation>
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
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>沒有樣式</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>編輯樣式...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>沒有樣式</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>編輯樣式</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copy of %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>新樣式</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>檔案 (*.sla *.sla.gz *.scd *.scd.gz);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>檔案 (*.sla *.scd);;全部檔案 (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>複件 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>匯入 (&amp;I)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>故事編輯器</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>目前段落:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>字: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>字元: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>總計: </translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>段落: </translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>文字檔案 (*.txt);;全部檔案(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>另存新檔</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>你要儲存變更嗎?</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>新建 (&amp;N)</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>從框體重新載入文字 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>儲存到檔案 (&amp;S)...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>從檔案載入 (&amp;L)...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>儲存檔案 (&amp;D)</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>更新文字框並退出 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>不更新文字框就退出 (&amp;E)</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>剪下 (&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>複製 (&amp;C)</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>貼上 (&amp;P)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>清除 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>更新文字框 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>檔案 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>編輯 (&amp;E)</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>全選 (&amp;A)</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>編輯樣式 (&amp;E)...</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>從文件載入文本</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>把文本另存到文件</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>更新文本框並退出</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>退出而不更新文本框</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>從框重新載入文本</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>更新文本框</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>尋找 / 取代 (&amp;S)...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>字體預覽 (&amp;F)...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>背景 (&amp;B)...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>顯示字體 (&amp;D)...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>設定 (&amp;S)</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>尋找/替代</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>智慧文字選擇 (&amp;S)</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>插入符號 (&amp;I)...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>清除全部文字</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>故事編輯器 - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>你真的想遺失所有更改嗎?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>你真的想清除你所有的文字嗎?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>插入 (&amp;I)</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>字元</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>引號</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>連字</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>空間</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>移位</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>線寬</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation type="unfinished">多於一個 item 被選擇</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>底線</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>小寫</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>下標</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>上標</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>刪除線</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>全部大寫</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>輪廓</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>陰影</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation>只文字底線</translation>
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
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>把檔案名稱作爲段落樣式的一個字首</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>下次不要再詢問</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Writer 匯入器選項</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>啓用這個將覆寫當前Scribus檔案現有的樣式</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>合併段落樣式</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished">按特性合併段落樣式。這將減少類似段落樣式的數量，將保持段落樣式的特性，即使原始檔案的樣式的命名不同。</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished">把檔案名稱追加到Scribus裏的段落樣式的名稱。</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation type="unfinished">把這些設定作爲預設值而且不要在每次匯入一個 OpenOffice.org 1.x 文件時又詢問</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>覆寫段落樣式</translation>
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
        <translation>無</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>內容索引列表</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>內容列表</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>物件屬性名稱:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>目標框:</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>在開始處</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>在結尾處</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>不顯示</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>內容列表將被放入的框體</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>放置的頁碼:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>將被創建條目作爲基礎的框體裏的物件屬性</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished">Place page numbers of the entries at the beginning or the end of the line, or not at all</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>列出不可列印的條目</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>也包括被設爲不可列印的框體</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>被條目行使用的段落樣式</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>段落樣式:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>&amp;X 索引</translation>
    </message>
    <message>
        <source>Table of Contents %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished">內容和索引</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished">內容</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>增加 (&amp;A)</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>刪除 (&amp;D)</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished">內容將被放置入的框體</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished">安置的頁碼：</translation>
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
        <translation>Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>忽略全部的錯誤</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>在列印或匯出前自動檢查</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>檢查遺失的字形</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>檢查不在頁面上的物件</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>檢查文字框是否有溢出</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>檢查使用的透明度</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>檢查遺失的圖片</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>檢查圖片的解析度</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>允許的最低解析度</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>檢查放入的 PDF 檔案</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished">檢測PDF注釋和域</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>增加配置檔案</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>移除配置檔案</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>一般設定</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>檔案中的位置</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>在背景</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>在前景</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>吸附</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>吸附距離:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>抓取半徑:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>顯示參考線</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>顔色:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>顯示頁面邊距</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>顯示頁面網格</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>主網格</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>間距:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>次網格</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>顯示基線網格</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>基線設定</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>基線網格 (&amp;G):</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>基線位移 (&amp;O):</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>頁面次網格中線的距離</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>頁面主網格中線的距離</translation>
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
        <translation>次網格的顏色</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>主網格的顏色</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>參考線的顏色</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>頁面邊距的顏色</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>基線網格的顏色</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>顯示或不顯示基線網格</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>顯示或不顯示頁面網格</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>顯示或不顯示參考線</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>顯示或不顯示頁面邊距</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>參考線在頁面物件之下</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>參考線在頁面物件之上</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>基線網格中線的距離</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>頁面頂端到第一條基線的距離</translation>
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
        <translation>輸出範圍</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>全部頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>選擇頁面 (&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>旋轉 (&amp;R):</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>檔案選項</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>相容性 (&amp;Y):</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>裝訂 (&amp;B):</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>左邊</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>右邊</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>產生縮圖 (&amp;T)</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>把連鎖的文字框另存爲 PDF 物件 (&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>包括書籤 (&amp;I)</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>EPS 圖的解析度 (&amp;R):</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>壓縮文字和向量圖 (&amp;P)</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>圖像設置</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>自動</translation>
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
        <translation>無</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>最大</translation>
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
        <translation>最小</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>一般 (&amp;G)</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;E 嵌入全部字體</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>&amp;S 子集所有字體</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>嵌入</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>可用字體:</translation>
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
        <translation>要嵌入的字體:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>子集的字體：</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>字體 (&amp;F)</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>啓用報告效果 (&amp;P)</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>頁面</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>顯示頁面預覽 (&amp;V)</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>特效</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>顯示持續時間 (&amp;D):</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>特效持續時間 (&amp;T):</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>特效類型 (&amp;Y):</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>移動的線條 (&amp;M):</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>來自 (&amp;R):</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>方向 (&amp;I):</translation>
    </message>
    <message>
        <source> sec</source>
        <translation>秒</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>沒有特效</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">百葉窗</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>盒子</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">溶解</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">閃爍</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">劈開</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">擦拭</translation>
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
        <translation>裡面</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>外面</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>從左到右</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>從頂部到底部</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>從底部到頂部</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>從右到左</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>從左上到右下</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>套用特效到所有頁面 (&amp;A)</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>附加 (&amp;X)</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>使用加密 (&amp;U)</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>密碼</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>使用者 (&amp;U):</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>所有者 (&amp;O):</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>允許列印檔案 (&amp;P)</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>允許更改檔案 (&amp;C)</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>允許複製文字和圖片 (&amp;Y)</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>允許添加注釋和表單域 (&amp;A)</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>安全 (&amp;E)</translation>
    </message>
    <message>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>輸出目的 (&amp;I):</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>螢幕 / 網頁</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>印表機</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>灰度</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>使用自定渲染設定 (&amp;U)</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>渲染設置</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>頻率 (&amp;Q):</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>角度 (&amp;A):</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">S&amp;pot Function:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>單點</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>線條</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>圓</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>橢圓</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>單色</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>使用ICC配置文件</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>配置文件:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>渲染意圖:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>感官</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>相對比色</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>飽和度</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>絕對比色</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>圖片:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>不使用嵌入 ICC 配置檔案</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>顔色 (&amp;O)</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 輸出意圖</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>資訊字串 (&amp;I):</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>輸入配置檔案 (&amp;P):</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>裁剪方框</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>顯示以上列出的每頁的頁面預覽。</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>在選中的頁裏開始報告前頁面顯示時間長度。</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>顯示特效類型。</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">劈開或百葉窗特效動態線條的方向。</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">方盒和劈開特效的開始位置。</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Direction of the glitter or wipe effects.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>套用所選特效到所有頁面。</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>匯出所有頁面為 PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>匯出一個範圍的頁面爲 PDF</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Generate PDF Articles, which is useful for navigating linked articles in a PDF.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>匯出圖片的 DPI (每英寸的點數)。</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>選擇一個使用者可以讀取你的 PDF 文件的密碼。</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>允許列印 PDF。如果不勾選, 則禁止列印。 </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>允許修改 PDF。如果不勾選, 則禁止修改 PDF。</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>爲單色嵌入顔色配置檔案</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>單色的顔色配置檔案</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>單色的渲染意圖</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>爲圖片嵌入一個顔色配置檔案</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>不使用來源圖片中嵌入的顔色配置檔案</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>圖片的顔色配置檔案</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>圖片的渲染意圖</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">用於列印的輸出配置文件。如果允許，從你的列印者那裏獲取關於配置文件選擇的指導。</translation>
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
        <translation>水平鏡像頁面</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>垂直鏡像頁面</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Convert Spot Colours to Process Colours</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>壓縮品質 (&amp;Q):</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want colour managed RGB for commercial printing and is selectable when you have activated colour management. Use only when advised by your printer or in some cases printing to a 4 colour digital colour laser printer.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>壓縮圖片的方法。選擇自動則 Scribus 會選擇最好的方法。ZIP 是無損壓縮, 適用於有 solid colours 的圖片。JPEG 對於要製作有很多照片的 PDF 是個好方法 (有可能會減低圖片品質)。如果你不需要選擇特殊的壓縮方式, 則選自動。</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>允許從 PDF 中複製文字或圖片。如果不勾選, 則文字和圖片不可複製。</translation>
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
        <translation>包括圖層 (&amp;Y)</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>壓縮方式 (&amp;D):</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>重新採樣圖片為 (&amp;M):</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>嵌入字體到PDF。嵌入字體將保持你的檔案的版面外觀。</translation>
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
        <translation>決定 PDF 檔案頁面裝訂的地方。除非你知道你需要修改它, 否則保持預設選項, 也就是左邊。</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>在 PDF 裡產生每個頁面的縮圖。有些 PDF 檢視器可使用這些縮圖來導覽。</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>嵌入你在檔案中建立的書籤。這對於瀏覽長的 PDF 檔案很有幫助。</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>文字和向量圖的匯出解析度。這不會影響 bitmap 圖片 (例如照片) 的解析度。</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>對文字和圖片啟用失真壓縮。除非你有理由, 否則請勾選此選項。這會減少 PDF 檔案的大小。</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished">Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>選擇一個所有者的密碼。這個密碼可以對輸出的 PDF 啟用或停用所有的安全功能</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Colour model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 colour CMYK printer.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">這個進階設定, 預設是不啟用的。This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>顯示設定</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>頁面排版</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>單頁</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>連續的</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>雙頁左</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>雙頁右</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation type="unfinished">視覺外觀</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished">使用閱讀器預設</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>使用全螢幕模式</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>顯示書籤欄</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>顯示縮圖</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>顯示圖層欄</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished">隱藏檢視器工具欄</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished">隱藏檢視器功能表欄</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished">適應檢視器視窗</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation type="unfinished">特別動作</translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation>檔案被打開時執行的Javascript</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>沒有 Script</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>檢視器</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>失真 - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>無失真 - Zip</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>圖片壓縮方法</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>PDF 檔案開啟時,
所要執行的 Javascript:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished">決定 PDF 的相容性。預設的 PDF 1.3 有著最廣的相容性。若你需要透明度或 128 bit 的加密功能, 則選擇 PDF 1.4 。若你想要保存物件在不同的圖層中, 則選擇 PDF 1.5。  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>檔案中的圖層只有在選擇 PDF 1.5 時才會被匯出到 PDF。</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>在匯出的檔案中不要顯示超出頁邊的物件</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>失真壓縮的品質: 最小 (25%), 低 (50%), 中 (75%), 高 (85%), 最大 (95%)。需注意的是品質和壓縮後的圖片檔案大小沒有直接關係。不同的圖片在相同的品質設定之下壓所出的檔案大小和品質的損失都不同。即使選擇最大品質, 使用 JPEG 壓縮時仍然會有些品質損失。</translation>
    </message>
    <message>
        <source>&amp;Embed All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outline &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>文件樣式</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>嵌入字體到 PDF。嵌入字體將保持你的檔案的版面外觀。有些字體, 例如 Open Type, 只能是個子集, 所以在 PDF 1.6 版之前的版本是無法嵌入的。 </translation>
    </message>
    <message>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>字體:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>尺寸:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>填充顔色:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>輪廓顔色：</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">&amp;M 列：</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation> 間距 (&amp;G):</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>線條顔色 (&amp;L):</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;S 陰暗：</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>填充顔色 (&amp;F):</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">&amp;H 陰暗：</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>線條樣式:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>線條寬度 (&amp;W):</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>線條樣式 (&amp;T):</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>箭頭:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>開始:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>結束:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>自由縮放 (&amp;F)</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>水平縮放 (&amp;H):</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>垂直縮放 (&amp;V):</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>按框體大小縮放圖像 (&amp;S)</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>保持比例 (&amp;R)</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>填充顔色 (&amp;L):</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>使用嵌入剪下路徑</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>螢幕預覽</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>完全解析度預覽</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>普通解析度預覽</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>低解析度預覽</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>最小 (&amp;N):</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>最大 (&amp;X):</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>步長 (&amp;S):</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>文字框屬性</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>圖像框屬性</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>形狀繪製屬性</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>預設放大級別</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>線條繪製屬性</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>多邊形繪製屬性</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>新建文字框的字體</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>新建文字框的字體大小</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>字體顔色</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">文字框中的列數</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">文字框中列間隔</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>你的字體範例</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>圖像框允許圖像伸縮到任意尺寸</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>水平伸縮圖像</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>垂直伸縮圖像</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>儲存水平和垂直伸縮相同</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>圖像框中的圖像被伸縮到圖像框的尺寸</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>自動伸縮圖像到原始的比例</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>給圖像框填充顔色</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>填充顔色的飽和度</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>形狀的線條顔色</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>線條顔色的飽和度</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>給形狀填充顔色</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>形狀的線條樣式</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>形狀的線條寬度</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>允許最小縮放率</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>允許最大縮放率</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>每次縮放操作的縮放比例變化</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>線條的顔色</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>顔色飽和度</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>線條樣式</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>線條寬度</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>跳位字元填充字元:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>跳位字元寬度:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>自定:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>自定: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>文字顔色:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>陰暗:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>文字筆劃:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>點</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>中杠</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>底線</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定義</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>下標</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>移位 (&amp;D):</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>縮放 (&amp;S):</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>上標</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>移位 (&amp;I):</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>縮放 (&amp;S):</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>小寫</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>縮放 (&amp;A):</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>底線</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>移位:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>自動</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>線條寬度:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>刪除線</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>自動線條間隔 (&amp;L)</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>線條間隔:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>在一條線上字體基線的位移</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">和普通字體對比上標的相對尺寸</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">在一條線上普通字體的底線下的位移</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">和普通字體對比下標的相對尺寸</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">和普通字體相比小寫字體的相對尺寸</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>在字體大小的基礎上爲行距所增加的百分比</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished">以字體後繼百分比表示的普通字體底線下面的位移</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished">以字體大小的百分比表示的行寬</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished">以字體後繼百分比表示的普通字體底線上面的位移</translation>
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
        <translation>句號</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>逗號</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>中間</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>全部刪除</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>段落首行縮進</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>整個段落從左縮進</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>刪除所有跳位字元</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>位置 (&amp;P):</translation>
    </message>
    <message>
        <source>None</source>
        <translation>無</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>填充字元:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>自定:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>自定: </translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>點</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>連字元</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>底線</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>自定</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>無</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>結構樹</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>元素</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>群組</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>自由物件</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>頁</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>移位</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>線條寬度</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>增加垂直參考線</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>增加水平參考線</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>移除垂直參考線</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>移除水平參考線</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>移動垂直參考線</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>移動水平參考線</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>鎖定參考線</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>解除鎖定參考線</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>移動</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>縮放</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>旋轉</translation>
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
        <translation>選擇</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>群組</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>選擇 / 群組</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>產生</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation type="unfinished">X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>對齊 / 排列</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>參加物件</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>設置填充顔色</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>顔色1: %1, 顔色2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>設置填充顔色陰暗</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>設施線條顔色</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>設置線條顔色陰暗</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>水平翻轉</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>垂直翻轉</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>鎖定</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>解鎖</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>鎖定尺寸</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>解鎖尺寸</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>解群組</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>刪除</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>重命名</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation type="unfinished">從 %1
to %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>套用主頁</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>貼上</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>剪下</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>設置填充顔色透明度</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>設置線條顔色透明度</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>設置線條樣式</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation type="unfinished">設置線條尾端的樣式</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation type="unfinished">設置線條接點的樣式</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation type="unfinished">設置線寬</translation>
    </message>
    <message>
        <source>No style</source>
        <translation type="unfinished">沒有樣式</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation type="unfinished">設置自定義線條樣式</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation type="unfinished">不用自定義線條樣式</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation type="unfinished">設置前端箭頭</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation type="unfinished">設置尾端箭頭</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>建立表格</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation type="unfinished">行： %1, 列： %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation type="unfinished">設置字體</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation type="unfinished">設置字體大小</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation type="unfinished">設置字體寬度</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>設置字體填充色</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>設置字體輪廓色</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>設置字體填充色陰暗</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>設置字體輪廓色陰暗</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation type="unfinished">設置字距調整</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation type="unfinished">設置行距</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation type="unfinished">設置段落樣式</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>設置語言</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>對齊文字</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>設置字體效果</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>圖像框</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>文字框</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>多邊形</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>貝茲曲線</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>多線</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>轉換成</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>匯入 SVG 圖像</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>匯入 EPS 圖像</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished">草稿區</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation type="unfinished">文本環繞框體</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation type="unfinished">文本環繞方盒</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation type="unfinished">文本環繞輪廓線</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>沒有文本流</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>無限制方框</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation type="unfinished">沒有輪廓線</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>頁面 %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished">設置圖像縮放</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation type="unfinished">框大小</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>自由縮放</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>保持比例</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation type="unfinished">打破比例</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation type="unfinished">編輯輪廓線</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation type="unfinished">編輯形狀</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation type="unfinished">重置輪廓線</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>增加頁面</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>增加頁面</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>刪除頁面</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>刪除頁面</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>增加圖層</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>刪除圖層</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>重新命名圖層</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>升高圖層</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>降低圖層</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>發送到圖層</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>允許列印圖層</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>取消列印圖層</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>更改圖層名稱</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>取得圖片</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>匯入 OpenOffice.org Draw 圖片</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>設置字體高</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>路徑裏的文字</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>允許物件列印</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>取消物件列印</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>多重複製</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X: %1, Y: %2
X: %4, Y: %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset control point</source>
        <translation>重置控制點</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>重置控制點</translation>
    </message>
    <message>
        <source>Modify image effects</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>起始狀態</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>動作歷史</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>只顯示選中物件</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>復原 (&amp;U)</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>重做 (&amp;R)</translation>
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
        <translation>取得更新檔逾時。</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>取得更新檔時發生錯誤: %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>在伺服器中找不到檔案</translation>
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
        <source>This list may contain development versions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>細節請瀏覽 www.scribus.net.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation type="unfinished">頁面大小爲 %1 的最小頁邊距</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>使用印表機頁面邊距</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>選擇印表機 (&amp;P):</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>邊距</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>右:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>頂部 (&amp;T):</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>底部 (&amp;B):</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>左 (&amp;L):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>確定 (&amp;O)</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>取消 (&amp;C)</translation>
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
        <translation>插入數值</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>輸入一個數值然後按確定</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation type="unfinished">輸入一個數值然後按確定</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>傳送一個數值給 Script</translation>
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
        <translation>屬性...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>按鈕</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>文字域</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>勾選框</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>多選框</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>列舉框</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>文字</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>鏈結</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF 工具</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="unfinished">插入PDF域</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>插入 PDF 注釋</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>選擇要使用的匯入器</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>自動 </translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>匯入文字但不帶格式</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>匯入器:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>編碼:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>只匯入文字</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>打開</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>選擇要用的匯入器</translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>記住關係</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished">記住文件尾碼－匯入器綁定而且下一次爲此文件類型自動選擇匯入器。</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>從模板新建</translation>
    </message>
    <message>
        <source>All</source>
        <translation>全部</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面尺寸</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>顔色</translation>
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
        <translation>製作工具</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>作者</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>移除 (&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>打開 (&amp;O)</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>下載模板</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>安裝模板中</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>把包解壓縮到當前用戶的模板目錄 ~/.scribus/templates中，或者解壓縮到PREFIX/share/scribus/templates目錄中，以便系統中所有的用戶都可以使用。</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>正在準備一個模板</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>正在刪除一個模板</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>正在翻譯 template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>檔案模板可在 http://www.scribus.net/ 的下載區取得。</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>確定你使用的圖片及字體可以自由取得。若字體無法分享, 儲存模板時不要將其放入。</translation>
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
        <translation>儲存爲模板</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名稱</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>類別</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>頁面尺寸</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>顔色</translation>
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
        <translation>電子郵件</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>更多細節</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確認</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>更少細節</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>信紙</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>小報</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>橫式</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>直式</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>自定義</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>生成篩檢程式</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;L 清除</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;D 刪除</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>選擇一個以前儲存好的篩檢程式</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>給這個篩檢程式起個名稱以便儲存</translation>
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
        <translation>禁用或啓用這個過濾行</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>清除這個過濾行</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>現在添加一個新過濾行</translation>
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
        <translation>取代</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>應用</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>左邊的數值是一個正則運算式</translation>
    </message>
    <message>
        <source>with</source>
        <translation>用</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>段落樣式</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>所有情況</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>所有段落</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>段落開始於</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>段落不多於</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>段落不少於</translation>
    </message>
</context>
</TS>
