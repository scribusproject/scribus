<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;name&quot;])-&gt;float

Returnerar teckengrad för textramen &quot;name&quot;. Om det finns text
markerad i ramen returneras värdet för det första tecknet i
markeringen.
Om inget &quot;namn&quot; angetts returneras värdet från aktuell komponent.
</translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames()-&gt;list

Returnerar en lista med namnen på alla de i dokumentet definierade färgerna.
Om inget dokument är öppet returneras en lista med färger från standarddokumentet.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog()-&gt;bool

Visar dialogrutan &quot;Nytt dokument&quot;. Skapar ett nytt dokument om användaren
accepterar valda inställningar. Om användaren trycker Avbryt skapas inget dokument.
Returnerar värdet &quot;true&quot; om ett nytt dokument skapades.
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;])-&gt;string

Returnerar namnet på fyllningsfärgen för objektet &quot;name&quot;.
Om &quot;name&quot; inte angetts används aktuell komponent.
</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy[, &quot;name&quot;])

Flyttar ett objekt enligt dx och dy relativt nuvarande position. Avstånden uttrycks
i det aktuella dokumentets storleksmått (se konstanten ENHET).
Om &quot;name&quot; inte angetts appliceras flytten på aktuellt objekt.
Om objektet &quot;name&quot; tillhör en gruup flyttas hela gruppen.
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Stänger av omritningen av sidan om bool=False, annars är omritning aktiverad.
Denna ändring kvarstår även sedan skriptet avslutats, så se till att
setRedraw(True) anropas i en finally:clause i början av ditt skript.
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
        <translation>createRect(x, y, width, height, [&quot;name&quot;])-&gt;string

Skapar en ny rektangel på sidan och returnerar namnet. Koordinaterna anges i
aktuellt dokuments storleksmått (se konstanten ENHET). &quot;name&quot; måste vara en
unik identifierare av objektet eftersom det används som referens till objektet. Om
&quot;name&quot; inte anges kommer Scribus att skapa ett automatiskt.

Kan resa ett NameExistsError om du utryckligen anger ett namn som redan används.
</translation>
    </message>
    <message>
        <source>newPage(where [,&quot;template&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;template&quot; specifies the name of the
template page for the new page.

May raise IndexError if the page number is out of range
</source>
        <translation type="obsolete">newPage(where [,&quot;template&quot;])

Skapar en ny sida. Om &quot;where&quot;= -1 läggs den nya sidan till i slutet av 
dokumentet, annars infogas den före &quot;where&quot;. Sidnummer börjar på 1 och räknas uppåt
oberoende av vilket sidnummer som anges på förstasidan i ditt dokument.
Den valfria parametern &quot;template&quot; anger namnet på den mall som
ska användas på den nya sidan.

Kan resa IndexError om sidnumret inte anges med ett passande värde
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Anger den tonade fyllning till objektet &quot;name&quot; till &quot;type&quot;. Färgbeskrivningarna
är samma som för setFillColor() och setFillShade(). Se även tillgängliga typer för
konstanterna (FILL_&lt;type&gt;).
</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Skriver &quot;string&quot; i Scribus meddelanderad (statusraden). Texten måste
vara UTF8-kodad eller en &apos;Unicode&apos;-sträng (rekommenderas).
</translation>
    </message>
    <message>
        <source>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -&gt; bool

Creates a new document and returns true if successful. The parameters have the
following meaning:

    size = A tuple (width, height) describing the size of the document. You can
    use predefined constants named PAPER_&lt;paper_type&gt; e.g. PAPER_A4 etc.

    margins = A tuple (left, right, top, bottom) describing the document
    margins

    orientation = the page orientation - constants PORTRAIT, LANDSCAPE

    firstPageNumber = is the number of the first page in the document used for
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
        <translation type="obsolete">newDoc(size, margins, orientation, firstPageNumber,
                  unit, facingPages, firstSideLeft) -&gt; bool

Skapar ett nytt dokument och returnerar &quot;true&quot; om det lyckas. Parametrarna har
följande betydelser:

size = En tupel (bredd, höjd) som beskriver dokumentstorlek. Du kan
använda fördefinierade konstanter såsom PAPER_&lt;paper_type&gt;, t. ex. PAPER_A4 etc.

margins = En tupel (vänster, höger, topp, botten) som beskriver dokumentets
marginaler

 orientation = Sidorientering - konstanter PORTRAIT (porträtt), LANDSCAPE (landskap)

firstPageNumber = Numret på dokumentets förstasida som andänds
för sidnumrering. Oftast används 1 är det ofta användbart med högre
nummer om du skapar dokument i flera delar.

unit: Anger vilket storleksmått som dokumentet använder. Använd en
fördefinierad konstant till detta: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICAS, UNIT_POINTS.

 facingPages = Anger om dokumentet skall arbeta med uppslag.
Använd följande konstanter: FACINGPAGES, NOFACINGPAGES

 firstSideLeft = Anger om förstasidan skall vara en vänster- eller högersida. 
Möjliga värden: FIRSTPAGELEFT, FIRSTPAGERIGHT

Värdena för bredd, höjd och marginaler uttrycks i det givna dokumentets storleksmått.
Konstanterna PAPER_* uttrycks i punkter. Om ditt dokument inte använder punktmått
måste du ta hänsyn till detta.

exempel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
               FACINGPAGES, FIRSTPAGERIGHT
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
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
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
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
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
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
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
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Om Scribus%1%2</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Om</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Bidrag från:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Portning till Windows:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>Skapa&amp;re:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Tyska:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Franska:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italienska:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Ungerska:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrainska:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bulgariska:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galiciska:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turkiska:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Litauiska:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polska:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Tjekiska:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovakiska:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Danska:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norska:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Walesiska:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Ryska:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brasilianska:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finska:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovenska:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baskiska:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;�ersättningar</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>O&amp;nline</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Detta fönster visar versionsnummer, byggdatum och
 kompilerade stödbibliotek för Scribus. 
Beteckningen C-C-T betyder stöd för C=CUPS, C=littlecms och T=Tiff. 
Avsaknad av stöd är markerat med *</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Utvecklargrupp:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Officiell dokumentation:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>�rig dokumentation:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Engelska (British):</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Svenska:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hemsida</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Online referenshandbok</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Felrapporter och önskemål om nya funktioner</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Utsändningslista</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Officiella översättningar och översättare:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Koreanska:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Serbiska:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Spanska:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Tidigare översättare:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katalanska:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Om Scribus %1</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>November</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nej</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Stäng</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">�amp;ppna...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Spara &amp;som...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">�er&amp;gå till senast sparad</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">Samla ihop för utskrift...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Hämta text...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">Lägg &amp;till text...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Spara &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Spara sida som &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">Spara som P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">Dokumentin&amp;ställningar...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Skriv ut...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">&amp;Avsluta</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">�&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">Gör &amp;om</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">Rensa innehå&amp;ll</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">M&amp;arkera allt</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Sök/Ersätt...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">Fä&amp;rger...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">St&amp;yckemallar...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">&amp;Linjemallar...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">Inställninga&amp;r...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Annan...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">&amp;Vänster</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">&amp;Centrerat</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">&amp;Marginajustera</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished">&amp;Hård marginaljustering</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished">&amp;Understrykning</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="unfinished">�amp;versikt</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="unfinished">&amp;Tabulatorer...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">&amp;Duplicera flera gånger</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">&amp;Gruppera</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished">Lås &amp;upp grupp</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">Flytta &amp;upp en nivå</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">Skicka till &amp;klippbok</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">B&amp;ild synlig</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="unfinished">� ett PDF-&amp;bokmärke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">� en PDF-a&amp;nteckning</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">&amp;Egenskaper för anteckning</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">Fält e&amp;genskaper</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">R&amp;edigera form...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">S&amp;ammanfoga text och kurva</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">Lös&amp;gör text från kurva</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">Sammanf&amp;oga polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">Dela upp &amp;polygoner</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">&amp;Bezierkurva</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished">&amp;Konturer</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">&amp;Textram</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="unfinished">&amp;Infoga...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Flytta...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">Hantera stö&amp;dlinjer...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Minia&amp;tyrer</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Visa &amp;marginaler</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Visa &amp;ramar</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">Visa b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Visa stö&amp;drutor</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Visa stö&amp;dlinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">Visa &amp;baslinjerna</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished">&amp;Lås till rutmönster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished">Lås till stödlin&amp;jerna</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished">Egenska&amp;per</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">&amp;Klippbok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">&amp;Lager</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Bokmärken</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="unfinished">Ver&amp;ktyg</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">P&amp;DF verktyg</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Rotera enhet</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">Zooma in eller ut</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="unfinished">Redigera raminnehåll</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">Redigera text...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Länka textramar</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Avbryt länk mellan textramar</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="unfinished">Redigera text med Texthanteraren</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Infoga textram</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Infoga tabell</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="unfinished">Infoga frihandslinjer</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">Bild&amp;hantering</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">A&amp;vstava text</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="unfinished">&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">Om &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">Verktygsti&amp;ps</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">Scribus &amp;handbok</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="unfinished">Skriv ut förhands&amp;visning</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="unfinished">&amp;Kaskad</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished">&amp;Sida vid sida</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Avancerat</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Spegla sida(sidor) &amp;horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Spegla sida(sidor) &amp;vertikalt</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Applicera &amp;UCR (Underfärgsborttagning)</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Applicera &amp;ICC profiler</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">Postscript Nivå &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">Postscript Nivå &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">Postscript Nivå &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Skapar Postscript Nivå 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Skapar endast Postscript Nivå 2.
OBS! Detta kan skapa stora filer.</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Skapar endast Postscript Nivå 1.
OBS! Detta kan skapa stora filer.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En metod för att ta bort en del av de gråtoner som kan uppstå (new line)
vid blandning av cyan, gult och magenta genom att ersätta med svart.(new line)
UCR påverkar de delar av bilden som är neutrala och/eller har mörka toner(new line)
i närheten av grått. Användande av UCR kan förbättra tryckkvalitetn för en del bilder(new line)
och för bästa resultat krävs testning och experiment från fall till fall.(new line)
UCR förhindrar risken för övermättnad med CMG färger</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Fördela/Justera</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Justera</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horisontellt</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Vänsterkant</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Centrerat</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Högerkant</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Mellan:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">�amp;ndra inte</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">&amp;Justera</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">&amp;Förflyttning</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Fördela &amp;jämnt</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikalt</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">�erkant</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Nederkant</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">Me&amp;llan</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">�amp;ndra inte</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">&amp;Justera</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">&amp;Förflyttning</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Fördela &amp;jämnt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">A&amp;nvänd</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Justera</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Vänsterjustera text</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Högerjustera text</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Centrera text</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Marginaljustering</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Hård marginaljustering</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Fältegenskaper</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knapp:</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfält:</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kryssruta:</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Radioknapp:</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Namn:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Verktygstips:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Teckensnitt att använda med PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ram</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Färg:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredd:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tunn</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Bred
</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Heldragen</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Streckad</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understruken</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Fasad</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Infälld</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Annan</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Endast läsning</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Nödvändigt</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Exportera inte värdet</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Synlighet:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Synlig</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Dold</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ingen utskrift</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Visas inte</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Utseende</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text när knapp trycks ner</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text som visas vid Mus-över</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikoner</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Använd ikoner</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Ta bort</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Tryckt</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Mus-över</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikonplacering...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Markera</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertera</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Konturerad</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Tryck</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Flerradig</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Lösenord</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Begränsning</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Tecken</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Rulla (scrolla) inte</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Kör inte Rättstavning</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Kontrollera stil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Kontrollera</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kors</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Cirkel</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stjärna</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Kvadrat</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Startvärde markerat</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Redigerbar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Alternativ</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gå till</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Skicka formulär</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>�erställ</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importera data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Händelse:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mus upp</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mus ner</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mus aktiverar</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mus avslutar</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>I fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Ej i fokus</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Redigera...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Skicka till URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Skicka data som HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importera data från:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Mottagare</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Till fil:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>�dra...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Sida:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>�gärd</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Fält är formatterat som:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nummer</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Procent</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Tid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Nummerformat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimaler:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Använd valutasymbol</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Lägg till valutasymbol först</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formattera</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Procentformat</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datumformat</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Tidformat</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Anpassade skript</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Snabbkommando:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Värdet är ej bekräftat</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Värdet måste vara större än eller lika med:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>och mindre än eller lika med:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Anpassat valideringsskript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Bekräfta</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Värde har ej beräknats</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Värdet är</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>summa</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>medelvärde</translation>
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
        <translation>av följande fält:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Välj...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Anpassat beräkningsskript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Beräkna</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Markering beaktas ej för PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Ange en kommaseparerad fältförteckning här</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Du måste minst ange en ikon för Normal om du vill använda knappikoner</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Exempel:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>�dring av markering</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alla filer (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Egenskaper Anteckningar</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Länk</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Extern länk</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Extern webblänk</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Mottagare</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-dokument (*.pdf);;Alla filer (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Sida:</translation>
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
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">&amp;Alla sidor</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished">till</translation>
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
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Använd mall</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Mall:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Använd på &amp;nuvarande sida</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Använd på alla &amp;jämna sidor</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Använd på alla &amp;ojämna sidor</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Använd från &amp;sida:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Till:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Klippbok,</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Ladda...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Spara &amp;som...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Liten</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Medium</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>S&amp;tor</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>För&amp;handsvisning</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Klippbok (*.scs);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Byt namn</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Ta bort</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Namnet &quot;%1&quot; är inte unikt.(new line)
Vänligen välj ett annat.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Namn:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Ny artikel</translation>
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
        <source>Bookmarks</source>
        <translation>Bokmärken</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Flytta bokmärken</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Lägg till bokmärke</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bokmärken</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Ikonplacering</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Layout:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Endast text</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Endast ikon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Text under ikon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Text ovanför ikon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Text till höger om ikon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Text till vänster om ikon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Text ovanpå ikon</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skala:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>alltid</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>När ikonen är för liten</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>När ikonen är för stor</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Aldrig</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skala hur:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportionellt</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Icke-proportionellt</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>�erställ</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Inställningar för färghantering</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktivera färghantering</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Systemprofiler</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Bilder:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Fyllda färger:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>S&amp;krivare:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>�ergivningsavsikt</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptuell</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolormetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mättnad</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolormetri</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>S&amp;krivare:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulera skrivare på skärmen</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Markera färger utanför &amp;färgområdet</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Använd &amp;svartpunktskompensation</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Standard färgprofil vid import av bilder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Standard färgprofil för tonplattor på sidan</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Färgprofil som du skapat eller erhållit från leverantör.(new line)
Profilen måste vara anpassad till din monitor och inte en allmän profil, dvs. sRGB.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Färgprofil till skrivaren som du erhållit från leverantör.(new line)
Profilen måste vara anpassad till din skrivare och inte en allmän profil, dvs. sRGB.</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standardvärde för återgivningen på din monitor. Om du inte vet varför du bör ändra
bör du välja Relative Colorimetric eller Perceptual.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standardvärde för återgivningen på din skrivare. Om du inte vet varför du bör ändra
bör du välja Relative Colorimetric eller Perceptual.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Tillåt &apos;soft proofing&apos; för visning på skärm hur dina färger kommer att tryckas,(new line)
baserat på din valda skrivarprofil.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metod för att visa på skärmen de färger som inte kommer att tryckas korrekt.(new line)
Detta kräver noggranna färgprofiler och ska endast ses som varningar.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Svartpunktskompensation är en metod för att förbättra kontrasten i foton.(new line)
Du rekommenderas att använda detta om ditt dokument innehåller foton.</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Redigera färg</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Namn:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Färg&amp;modell</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMGS</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>Webbsäker RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Gammal</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-färgkarta</translation>
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
        <translation>G:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation>S:</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Dynamiska färgstaplar</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statiska färgstaplar</translation>
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
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Namnet på färgen är inte unikt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Du kan inte skapa en färg med namnet &quot;%1&quot;.
Namnet är reserverat för en transparent färg</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation type="unfinished">Välj tecken:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Teckensnitt:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Infoga</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Stäng</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished">Infoga tecken vid markören i texten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Ta bort aktuell(a) markering(ar).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished">Stäng denna dialogruta och återvänd till textredigering.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specials</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Grekiska</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CheckerPrefsList</name>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Välj en katalog</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varning</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
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
        <source>Normal Vision</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>K</source>
        <translation type="unfinished"></translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
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
        <source>Merging colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished"></translation>
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
        <source>Cr&amp;eate color...</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">Fä&amp;rg</translation>
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
        <source>New Color</source>
        <translation type="unfinished">Ny färg</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
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
    <message>
        <source>Color %1 exists already!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">A&amp;nvänd</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Spara</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varning</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Nyans:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Täckning:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Horisontell toning</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Vertikal toning</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonal toning</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Korsvis, diagonal toning</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radiell toning</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Anpassad linjär toning</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Anpassad radiell toning</translation>
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
        <translation>pt</translation>
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
        <source>Edit Line Color Properties</source>
        <translation>Redigera linjens färgegenskaper</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Redigera fyllningsfärgens egenskaper</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Färgmättnad:</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Normal eller tonad fyllning</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Ange graden av transparens för vald färg</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Färg på valt objekt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Alternativ för CSV-import</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Fältavgränsare:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Värdeavgränsare:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Första raden är rubrik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Skrivaralternativ</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Sidan inställd</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alla sidor</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Endast jämna sidor</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Endast udda sidor</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Spegla</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientering</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Utskrift av  N-sidor på arket</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Sida per ark</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Sidor per ark</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Alternativ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Värde</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Denna dialogruta visar olika utskriftsalternativ för CUPS.(new line)
De exakta parametrar som är tillgängliga beror på din skrivardrivrutin.(new line)
Du kan kontrollera stöd för CUPS genom att välja Hjälp &gt; Om.(new line)
Kontrollera koden C-C-T. Dessa motsvarar stöd för C=CUPS, C=littlecms och T=Tiff.(new line)
Eventuellt saknat stöd markeras med *.</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimera fil</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Inkludera teckensnitt</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodning:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Förflyttar till din dokumentmapp.
Du kan ange dokumentmapp i Inställningar.</translation>
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
        <translation type="unfinished">CMGS</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">S:</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Ta bort färg</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Ta bort färg.</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Ersätt med:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Ta bort sidor</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Ta bort från:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>till:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Saknat teckensnitt</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Teckensnitt %1 är inte installerat.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Använd</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">istället</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformation</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Författare:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Nyckelord:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Beskri&amp;vning:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>U&amp;tgivare:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Bidragare:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Dat&amp;um:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;yp:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;fierare:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Källa:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>S&amp;pråk:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relation:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Täc&amp;kning:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Rätti&amp;gheter:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Ytterligare &amp;information</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Den person eller organisation som är primärt ansvarig för tillkomsten av dokumentets innehåll.(new line)
Fältet kan bäddas in i Scribusdokumentet som referens, såväl som i metadata för PDF.</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Ett namn för dokumentet.(new line)
Fältet kan bäddas in i Scribusdokumentet som referens, såväl som i metadata för PDF.</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Kan ge en snabb inblick i vad dokumentet innehåller.(new line)
Ge en kort beskrivning eller sammanfattning av dokumentinnehåll. Inbäddas i metadata för PDF vid export</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Beskriver ämnet för dokumentets innehåll.(new line)
Används för att bädda in nyckelord i PDF, för att underlätta sökning och indexering av PDF-filer.</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>En person eller organisation som är ansvarig för att göra dokumentet tillgängligt</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>En person eller organisation ansvarig för att bidraga till dokumentinnehåll</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Ett datum kopplat tilll en tidpunkt i dokumentets livscykel i formatet YYYY-MM-DD, enligt ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Typ av innehåll eller genre i dokumentet, t ex kategori, funktion, genre, avdelning etc</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Dokumentets fysiska eller digitala form . Mediatyp och storlek kan vara bra att ange.(new line)
RFC2045, RFC2046 för MIME-typer är också lämpliga att ange.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>En entydig referens till dokumentet i ett givet sammanhang, t ex ISBN eller URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>En referens till dokument som aktuellt dokument baseras på, t ex ISBN eller URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Det språk som dokumentet är skrivet i, vanligvis angivet med ISO639 kod(new line)
alternativt med en förkortning och en ISO-3166 landskod tillagt i slutet, t ex en-GB, fr-CH.</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>En referens till ett relaterat dokument, oftast med en formell identifierare som ISBN eller URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Innehållets giltighetsområde, oftast med angivande av plats, tidpunkt och ansvarsområde angett.</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Information rörande upphovsrätt avseende aktuellt dokument, t ex copyright, patent, varumärke m m.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>Active</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Stil</translation>
    </message>
    <message>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Integer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>String</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Värde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Ra&amp;dera</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Skrivarinställningar</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Vald skrivare</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Alternativ</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fil:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternativt skrivarkommando</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Använd en annan skrivarhanterare, t ex kprinter eller gtklp,(new line)
för att utnyttja ytterligare skrivaralternativ.</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Ko&amp;mmando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Avsnitt</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>S&amp;kriv ut allt</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Skriv ut aktuell si&amp;da</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Skriv &amp;ut avsnitt</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Infoga en kommaseparerad lista med symboler där(new line)
en symbol kan vara * för alla sidor, 1-5 för ett avsnitt(new line)
av sidor eller en enskild sida.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>An&amp;tal kopior:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Alternativ</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">Skriv ut &amp;Normal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Skriv ut s&amp;eparationer</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Allt</translation>
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
        <translation>Gult</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Svart</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Skriv ut i färg om tillgängligt</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Skriv ut i gråskala</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">A&amp;vancerat...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Skriv ut</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscriptfiler (*.ps);; Alla filer (*)</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="unfinished">Avancerat</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
        <translation>Import av fil:(new line)
%1(new line)
misslyckades!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Allvarligt fel</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Redigera stilmall</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Namn:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tecken</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Teckensnitt:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">Stor&amp;lek:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Effekt:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">&amp;Justering:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">An&amp;fang</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linjer:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">F&amp;yllnadsfärg:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Streckfärg:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Vertikala mellanrum</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Justera mot &amp;baslinjer</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Radav&amp;stånd</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">O&amp;vanför:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">Nedan&amp;för:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer och indrag</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Namn på styckemall</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Teckensnitt från vald text eller objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Teckengrad</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textens fyllnadsfärg</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textens konturfärg</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Skapar en förstorad första bokstav i stycket.(new line)
Används för att styra läsaren till rätt stycke efter en ingress.</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Bestämmer anfangens totala höjd, i antal rader.</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Justera texten efter baslinjerna</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Utrymme ovanför stycke</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Utrymme nedanför stycke</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Radavstånd</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Namnet på stilmallen är inte unikt</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Avstånd</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>�amp;ppna...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Spara &amp;som...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Spara och avsluta</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Avsluta utan att spara</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>�&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Gör &amp;om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opiera</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Klistr&amp;a in</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Hämta fältnamn</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Javascripts (*.js);;Alla filer (*)</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Färg:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Nyans:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="unfinished">Invertera</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&gt;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;&lt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Effects in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Välj en katalog att exportera till</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportera som bild(er)</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportera till katalog:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Alternativ</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Bild&amp;typ:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Upplösning:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Avsnitt</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Aktuell sida</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alla sidor</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Avsnitt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportera en grupp sidor</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Infoga en kommaseparerad lista med symboler där(new line)
en symbol kan vara * för alla sidor, 1-5 för ett avsnitt(new line)
av sidor eller en enskild sida.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportera alla sidor</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportera endast aktuell sida</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Bildens upplösning(new line)
Använd 72 dpi för bilder avsedda för visning på skärmen</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Bildkvalitet - 100% är bäst, 1% är sämst kvalitet</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Tillgängliga exportformat</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Utmatningskatalog - den katalog där du sparar dina bilder(new line)
Namnet på den exporterade filen blir &apos;dokumentnamn-sidnummer.filtyp&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>�dra utmatningskatalog</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Bildstorlekar. 100% för ursprungsstorlek, 200% för dubbla storleken etc.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Mättnad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Dissolve</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Täckning:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Bakgrund</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="unfinished">Lager</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Sökvägar</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Storlek:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titel:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Ingen titel</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Författare:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Okänd</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribusdokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMGS</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Färger</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Ta bort oanvända</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Färggrupper</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktuell färggrupp:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Spara färggrupp</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Välj färggrupp att ladda</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Spara aktuell färggrupp</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Ta bort oanvända färger fån aktuellt dokuments färggrupp</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Lägg till färger till aktuell färggrupp från ett existerande dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Skapa en ny färg i färggruppen</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Redigera vald färg</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Gör en kopia av vald färg</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Ta bort vald färg</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Gör aktuell färggrupp till standardfärggrupp</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Namn:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Välj namn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopia av %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ny färg</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importera</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Globala inställningar för teckensnitt</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Tillgängliga teckensnitt</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Teckensnittsersättningar</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Ytterligare sökvägar</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Använd teckensnitt</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Inbäddad i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Delmängd</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Sökväg till teckensnittsfil</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Tillgängliga teckensnitt</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Ersättning</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Teckensnitt&amp;sersättningar</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Lägg till...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Ytterligare &amp;sökvägar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Välj en katalog</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished">Använd teckensnitt</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished">Inbäddad i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Delmängd</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished">Sökväg till teckensnittsfil</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Lägg till det valda teckensnittet till menyerna Stil och Teckensnitt</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Avbryt Förhandsvisning</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Delmängd</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Lägg till det valda teckensnittet till menyerna Stil och Teckensnitt</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Avbryt Förhandsvisning</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished">Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">Sö&amp;k</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="unfinished">&amp;Stäng</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation type="unfinished"></translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Här kan du justera var färgerna börjar och slutar.
Du kan också lägga till ytterligare färger i toningen.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Hantera stödlinjer</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Horisontella stödlinjer</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>T&amp;a bort</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Vertikala stödlinjer</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Lägg till...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>T&amp;a bort</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Lås stödlinjer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Sida</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Tyvärr finns ingen manual tillgänglig! Vänligen besök http://docs.scribus.net
för uppdaterade dokument och www.scribus.net om du vill ladda hem manualen.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innehåll</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Länk</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus Online-hjälp</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">Sö&amp;k</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Skriv ut...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Find</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Bokmärken</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Möjlig avstavning</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Acceptera</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Hoppa över</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Inställning för avstavning</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>S&amp;pråk:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Korta&amp;ste ord</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Längden för det kortaste ord som ska avstavas.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximalt antal avstavningar i följd(new line)
Värdet 0 innebär obegränsat med avstavningar.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Förslag till avstavningar</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Avstava texten automatisk vi&amp;d inskrivning</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>En dialogruta visar de möjliga avstavningarna för ett ord när du använder alternativet Extra, Avstava text.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Startar automatisk avstavning av texten vid inskrivning.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Avstavningar i följd tillåts:</translation>
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
        <translation type="unfinished">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nej</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Importera &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
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
        <translation>Infoga sida</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Infoga</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Sida/sidor</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>framför sida</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>efter sida</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>i slutet</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Infoga</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Mall (Vänstersida):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Mall:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">&amp;Mall (Högersida):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Infoga</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Sidstorlek</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Storlek:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Anpassad</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landskap</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Höjd:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Infoga tabell</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Antal rader:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Antal kolumner:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Redigera Javascripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nytt script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nytt script</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Vill du verkligen ta bort detta skript?</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Hantera snabbkommandon</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Aktivitet</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktuell tangent</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Välj tangent för denna aktivitet</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen tangent</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Användardefinierad tangent</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Välj &amp;tangent</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
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
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Den här tangentkombination används redan</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Lager</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Lägg till nytt lager</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Ta bort lager</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Höj lager</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Sänk lager</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nytt lager</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Vill du också ta bort alla objetkt i detta lager?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Redigera linjeutseende</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopia av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Vill du verkligen ta bort denna stilmall?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importera</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Alla filer (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author:</source>
        <translation type="unfinished">Författare:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation type="unfinished"></translation>
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
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Beskrivning</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importera</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Sidstorlek</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Storlek:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Anpassad</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landskap</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Höjd:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="unfinished">Typ:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Marginallinjer</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Avstånd mellan övre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Avstånd mellan nedre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">&amp;Utanför:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Namn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopia av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Namn:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopia #%1 av</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
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
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicera flera gånger</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>A&amp;ntal kopior:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontell förflyttning:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikal förflyttning:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Avstånd</translation>
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
        <translation>Vinkel:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Längd:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Skriptfel</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Om du kör ett officiellt skript, vänligen rapportera till &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Detta meddelande finns också på ditt klippbord. Använd Ctrl+V för att klistra in i felhanteraren.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Visa &amp;konsolen</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Göm &amp;konsolen</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Importera mall</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importera sida/sidor</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Från dokument:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">�dra...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importera sida/sidor:</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Infoga en kommaseparerad lista med symboler där(new line)
en symbol kan vara * för alla sidor, 1-5 för ett avsnitt(new line)
av sidor eller en enskild sida.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>från 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Skapa sida/sidor</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">framför sida</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">efter sida</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">i slutet</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importera</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>från %1</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importera</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Saknat teckensnitt</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">Teckensnitt %1 är inte installerat.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Använd</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">istället</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Flytta sidor</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiera sida</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Flytta sida/sidor:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">till:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">framför sida</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">efter sida</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">i slutet</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>After Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At End</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="unfinished">Till:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Bild</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Färger</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometri</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
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
        <translation>&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höjd:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Baspunkt:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivå</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Form:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>R&amp;edigera form...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>R&amp;unda(new line)
hörn:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Textavstånd</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Kolu&amp;mner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mellanrum:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>To&amp;pp:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vänster:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>Höge&amp;r:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abulatorer...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Egenskaper för text längs kurvlinje</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Visa kurvlinje</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Start offset:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Avstånd från kurvlinje:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>&amp;Flöda text runt ramen</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Använd &amp;avgränsande ram</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Använd kont&amp;urlinje</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Teckengrad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Nyans:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Anpassat avstånd</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Kerning:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Radav&amp;stånd:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;il:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>S&amp;pråk:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Anpassa storleken</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-sk&amp;ala:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-ska&amp;la:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Skala &amp;till ramstorlek</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roportionellt</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Inmatningsprofil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>�ergivningsavsikt:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptuell</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolormetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mättnad</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolormetri</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Vänster punkt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Slutpunkt</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Baspunkt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Linjet&amp;yp:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linje&amp;bredd:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spetsigt hörn</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Fasat hörn</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rundat hörn</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Ka&amp;nter:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Platt avslutning</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Fyrkantig avslutning</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rundad avslutning</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Avslutning:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stilmall</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Cellkanter</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linje överst</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linje till vänster</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linje till höger</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linje nederst</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Namn på valt objekt</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horisontell position för vald baspunkt</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikal position för vald baspunkt</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Bredd</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Höjd</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotation av objekt vid vald baspunkt</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punkt från vilken måttangivelse eller rotationsvinkel beräknas</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Välj överst till vänster som baspunkt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Välj överst till höger som baspunkt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Välj nederst till vänster som baspunkt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Välj nederst till höger som baspunkt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Välj mitten som baspunkt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Vänd horisontellt</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Vänd vertikalt</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Flytta upp en nivå</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Flytta ner en nivå</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Flytta till överst</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Flytta till nederst</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Markerar vilken nivå objektet befinner sig, 0 markerar nederst</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Lås eller lås upp objektet</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Lås eller lås upp objektets storlek</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Tillåt/tillåt ej utskrift av objektet</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Låt texter i ramar på lägre nivåer flöda runt objektets form</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Använd en avgränsande ram istället för objektets form för textflöde</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Använd en extra linje baserat på objektets form för textflöde</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Teckensnitt för vald text eller objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Teckengrad</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Storleksförändra teckenbredd</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Färg på textkontur</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Fyllnadsfärg på text</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Mättnad på färg på textkontur</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Mättnad på fyllnadsfärg för text</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuell kerning</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Radavstånd</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stil på valt stycke</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Avstavningsspråk för ramen</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>�dra inställningar för vänsterpunkt eller slutpunkt</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Linjemönster</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linjetjocklek</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Fogtyper för linjer</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Typ av linjeavslut</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Linjestil på valt objekt</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Välj form på ram...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Redigera form på ram...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Ange hörnradie</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Antal spalter i textramen</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Växla mellan spaltmellanrum och spaltbredd</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Spaltmellanrum</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Avstånd till text från ramens överkant</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Avstånd till text från ramens nederkant</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Avstånd till text från ramens vänsterkant</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Avstånd till text från ramens högerkant</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Redigera ramens tabulatorinställningar...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Tillåt att bildens storlek är en annan än ramens</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horisontell offset för bilden innanför ramen</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikal offset för bilden innanför ramen</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Storleksförändra bilden horisontellt</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Storleksförändra bilden vertikalt</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Behåll proportioner i X- och Y-led</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Behåll proportionerna</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Anpassa bilden till ramstorleken</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Använd bildens proportioner istället för ramens</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Bildens ursprungsprofil</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Syfte för bildåtergivning</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation>&amp;X1:</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation>&amp;X2:</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation>&amp;Y1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Spaltbredd</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Namnet &quot;%1&quot; är inte unikt.(new line)
Vänligen välj ett annat.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Skrivning från höger till vänster</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
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
        <translation>Redigera stilmall</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Platt avslutning</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Fyrkantig avslutning</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rundad avslutning</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spetsigt hörn</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Fasat hörn</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rundat hörn</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linjebredd:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Heldragen linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Streckad linje</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Punktlinje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Streck-punkt linje</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Streck-punkt-punkt linje</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Namnet &quot;%1&quot; är inte unikt.(new line)
Vänligen välj ett annat.</translation>
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
    <name>MusterPages</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Redigera mallar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Vill du verkligen ta bort denna mall?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Namn:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Ny mall</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopia av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Namn:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopia #%1 av</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Redigera mallar</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Namn:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Ny mall</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopia av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Namn:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopia #%1 av</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Vill du verkligen ta bort denna mall?</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nytt dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidstorlek</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Anpassad</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höjd:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Uppslag</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Vänst&amp;er sida först</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marginallinjer</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">Väns&amp;ter:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">�amp;verst:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Nederst:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Alternativ</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Första s&amp;idans nummer:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Stan&amp;dardmåttenhet:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Punkter (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Tum (tum)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pica (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Auto&amp;matiska textramar</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Stödlinjer för spalter</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>S&amp;paltmellanrum:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Spa&amp;lter:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Sidstorlek för dokumentet, antingen en standardstorlek eller en anpassad</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Sidorientering i dokumentet</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentsidornas bredd, redigerbara om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentsidornas höjd, redigerbara om du valt anpassad dokumentstorlek</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Tillåt enkelsidig eller dubbelsidig layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Gör dokumentets förstasida till en vänstersida</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan övre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan nedre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan vänster marginallinje och papperskanten.(new line)
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan höger marginallinje och papperskanten.(new line)
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Förstasidans nummer i dokumentet</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standardmåttenhet för redigering av dokumentet</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Skapa automatiskt nya textramar när nya sidor läggs till dokumentet</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Antal spalter i automatiskt skapade textramar</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Spaltmellarum för automatisk skapade spalter</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Innanför:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Utanför:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Ledger</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alla filer (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">�pna</translation>
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
        <translation type="unfinished">Ny &amp;från mall...</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Vänster sida</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Höger sida</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Noder</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absoluta koordinater</translation>
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
        <translation>Redigera &amp;konturlinje</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>�e&amp;rställ konturlinje</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Avsluta r&amp;edigering</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Flytta noder</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Flytta kontrollpunkter</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Lägg till noder</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Ta bort noder</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Flytta kontrollpunkter oberoende av varandra</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Flytta kontrollpunkter symmetriskt</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>�erställ kontrollpunkter</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>�erställ denna kontrollpunkt</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>�pnar en polygon eller skär upp en Bezierkurva</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Stäng Bezierkurvan</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Spegla kurvbanan horisontellt</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Spegla kurvbanan vertikalt</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Luta kurvbanan horisontellt åt höger</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Luta kurvbanan horisontellt åt vänster</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Luta kurvbanan vertikalt uppåt</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Luta kurvbanan vertikalt nedåt</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotera kurvbanan moturs</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotera kurvbanan medturs</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Minska storleken på kurvbanan med visad %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>�a storleken på kurvbanan med visad %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotationsvinkel</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% att öka eller minska med</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivera redigering av konturlinje</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>�erställ konturlinje till ursprunglig ramform</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">När denna ruta kryssas används koordinater relativt sidan,
annars används koordinater relativt objektet.</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished">Använd dokumentnamn som prefix för styckemallar</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished">Fråga inte igen</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished">Scribusdokument</translation>
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
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation type="obsolete">Skriptkonsol</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Skapa PDF fil</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Sk&amp;icka till fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Skriv ut</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Alla sidor</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">Vä&amp;lj sidor</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Alternativ för fil</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">Kompatibilit&amp;et:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">&amp;Bindning:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Vänster sida</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Höger sida</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Skapa &amp;miniatyrer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Spara &amp;länkade textramar som PDF-artiklar</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Inkludera bokmärken</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete">dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Upplösning:</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Bildinställningar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automatisk</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metod:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Maximum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Hög</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Låg</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Minimum</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Mi&amp;nska upplösningen på bilder till:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Allmänt</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">Bä&amp;dda in alla teckensnitt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Inbädda</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Tillgängliga teckensnitt:</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="obsolete">&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="obsolete">&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Teckensnitt att bädda in:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Sida</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">Förhands&amp;visning</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">Tid&amp;d för bildvisning:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Tid för effek&amp;t:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">T&amp;yp av effekt:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">Rörliga lin&amp;jer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">F&amp;rån:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">R&amp;iktning:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Blinds</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Box</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Dissolve</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Split</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Wipe</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horisontell</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Inifrån</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Utifrån</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Vänster till höger</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Uppifrån och ner</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Nerifrån och upp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Höger till vänster</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Diagonalt från övre vänstra hörnet</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">&amp;Applicera effekten på alla sidor</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">&amp;Använd kryptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Lösenord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Användare:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">�amp;gare:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Inställningar</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Tillåt &amp;utskrift av dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Tillåt än&amp;dring av dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Tillåt kop&amp;iering av text och grafik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Tillåt att lägga till &amp;anteckningar och fält</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">Säk&amp;erhet</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Allmänt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">Utmatning&amp;en är avsedd för:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Skärmen/webben</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Skrivare</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Använd anpassade inställningar för återgivningen</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Inställningar för återgivning</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">Fre&amp;kvens:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">&amp;Punktform:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Enkel punkt</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Linje</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Ellips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Fyllda färger:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Använd ICC-profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">�ergivningsavsikt:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perceptuell</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Relativ kolormetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Mättnad</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Absolut kolormetri</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Använd inte inbäddade ICC-profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">Fä&amp;rg</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">PDF/X-3 Utmatningsavsikt</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;informationssträng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Utmatnings&amp;profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Trimmningsram</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Exportera alla sidor till PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Exportera en grupp av sidor till PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Infoga en kommaseparerad lista med symboler där(new line)
en symbol kan vara * för alla sidor, 1-5 för ett avsnitt(new line)
av sidor eller en enskild sida.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Avgör kompatibiliteten med PDF. Standardvärde är Acrobat 4.0 som ger störst kompatibilitet.(new line)
Välj Acrobat 5.0 om din fil har egenskaper som transparens eller du vill ha 128-bitars kryptering.(new line)
PDF/X-3 används för export för kommersiella utskrifter och är tillgängligt om du aktiverat färghantering.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Avgör sidbindningen i PDF. Om du inte vet(new line)
att du behöver ändra låter du standardvärdet - vänster - vara kvar.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Skapar miniatyrer av alla sidor i PDF-filen.(new line)
En del PDF-läsare kan använda dessa för navigering.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Skapar PDF-artiklar, användbart för navigering av länkade artiklar i PDF</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Bäddar in de bokmärken du skapat i dokumentet.(new line)
Användbart för navigering av långa PDF dokument.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Exportera upplösning för text och vektorgrafik.(new line)
Detta påverkar inte upplösningen för punktuppbyggd grafik, t. ex. foton.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Kompression av text och grafik.(new line)
Om du inte behöver ändra låter du detta var ikryssat. Det minskar storleken på PDF-filen.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressionsalternativ för bilder.(new line)
Automatisk låter Scribus välja den lämpligaste metoden.(new line)
ZIP passar bilder med rena färger/tonplattor.(new line)
JPEG skapar små PDF-filer om du har många foton i dokumentet (med en viss kvalitetsförlust).(new line)
Om du inte behöver någon speciell kompression, använd automatisk.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Kompressionsnivåer: Minimum (25%), Låg (50%), Medium (75%), Hög (85%), Maximum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Minska upplösningen på dina bitmappade bilder till vald DPI.(new line)
Om du inte markerar kryssrutan återges de med urprunglig upplösning.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">(DPI (Dotr Per Inch) för export av bilder.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Inbäddade typsnitt i PDF-filen. Att bädda in typsnitt(new line)
bibehåller layout och utseende på ditt dokument.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Tillåter presentationseffekter när du använder Acrobat Reader i fullskärmsläge.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Förhandsvisa varje sida i listan ovanför.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Tid som sidan vissas innan presentationen startar på vald sida.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Tid för effekten.(new line)
En kortare tis snabbar upp effekten, en längre gör den långsammare.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Typ av visningseffekt.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Riktning för effekten vid användande av effekterna Blinds eller Split.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Startposition för effekterna Box eller Split.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Riktning för effekterna Wipe eller Glitter.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Använd vald effekt på alla sidor.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktivera säkerhetsfunktionerna i din exporterade PDF-fil.(new line)
Om du valt Acrobat 4.0 används en 40-bitars kryptering i din PDF-fil.(new line)
Om du valt Acrobat 5.0 används en 128-bitars kryptering i din PDF-fil.(new line)
Observera! Krypteringen i PDF är inte lika pålitlig som GPG eller PGP och har vissa begränsningar.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Välj ett huvudlösenord som aktiverar eller avaktiverar alla(new line)
säkerhetsfunktioner i din exporterade PDF-fil.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Välj ett lösenord för dina användare som kan användas för att läsa din PDF-fil.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Tillåt utskrift av PDF-filen. Om ej markerad så tillåts ej utskrift.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Tillåt ändring av PDF-filen. Om ej kryssad är ändring förhindrad.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Tillåt kopiering av text eller grafik från din PDF-fil.(new line)
Om ej kryssad kan text och grafik inte kopieras.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Tillått tillägg av anteckningar och fält till din PDF-fil.(new line)
Om ej kryssat är tillägg av anteckningar och fält inte tillåtet.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Färgmodell för utmatning av din PDF-fil.(new line)
Välj Skärm/Webben för PDF-filer som ska visas på skärmen eller för utskrift på enklare bläckstråleskrivare.(new line)
Välj Skrivare för utskrift på skrivare för äkta 4-färg CMGS.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Detta är en avancerad inställning, ej aktiverad från start. Den skall endast aktiveras(new line)
om din tryckare särskilt kräver det och ger dig nödvändiga uppgifter.(new line)
Annars riskerar din exporterade PDF-fil att inte tryckas korrekt och portabiliteten mellan olika system försvinner.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Bädda in en färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">�ergivningsavsikt för tonplattor</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Bädda in en färgprofil för bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Använd inte färgprofiler som är inbäddade i ursprungliga bildfiler.</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Färgprofil för bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">�ergivningsavsikt för bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Utmatningsprofil för utskrift. Om möjligt bör du rådgöra med din tryckare för korrekt val av profil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Obligatorisk sträng för PDF/X-3 annars följer inte PDF-filen(new line)
reglerna för PDF/X-3. Vi rekommenderar att du använder dokumentets titel.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Avstånd för utfall räknat från övre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Avstånd för utfall räknat från nedre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Avstånd för utfall räknat från vänstra kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Avstånd för utfall räknat från högra kanten av pappret</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Komprimera text och &amp;vektorgrafik</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Aktiver&amp;a presentationseffekter</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Presentation</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotation:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">&amp;Gruppera alla typsnitt</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Typsnitt att gruppera:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Spegla sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Spegla sida(or) vertikalt</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Förhandsvisning</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Anti-aliasing för &amp;text</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Anti-aliasing för &amp;grafik</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Visa trans&amp;parens</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Underfärgsborttagning (UCR)</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Visa CMGS</translation>
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
        <translation>&amp;G</translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation>&amp;S</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Visar en mer tilltalande bild av text i förhandsvisaren, på bekostnad av
en något långsammare laddning av dokumentet. Detta gäller endast Typ1 teckensnitt</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Visar en mer tilltalande bild av TT typsnitt, Open Type typsntt, EPS, PDF och
vektorgrafik i förhandsvisaren, på bekostnad av en något långsammare laddning av dokumentet.</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Visar transparens och transparenta objekt i ditt dokument. Kräver Ghostscript 7.07 eller högre.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Visar en förhandsvisning med simulerad CMGS, istället för RGB färger</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Aktivera/inaktivera plåten för C (Cyan)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Aktivera/inaktivera plåten för M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Aktivera/inaktivera plåten för G (Gul)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Aktivera/inaktivera plåten för S (Svart)</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En metod för att ersätta några av de grå nyanserna som är
sammansatt av cyan, magenta och gult med svart istället.
UCR påverkar mest de delar av en bild som innehåller neutrala och/eller
mörka toner nära grått. Användning av UCR kan ge förbättrat tryckresultat
för en del bilder. Bästa resultat kräver experiment och tester från fall till fall.
UCR minskar risken för övermättnad av CMG färger.</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Allt</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">En metod för att ta bort en del av de gråtoner som kan uppstå 
vid blandning av cyan, gult och magenta genom att ersätta med svart.
UCR påverkar de delar av bilden som är neutrala och/eller har mörka toner
i närheten av grått. Användande av UCR kan förbättra tryckkvalitetn för en del bilder
och för bästa resultat krävs testning och experiment från fall till fall.
UCR förhindrar risken för övermättnad med CMG färger.
</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Tillhandahåller en bättre återgivning av TrueType teckensnitt, OpenType teckensnitt, EPS, PDF och
vektorgrafik vid förhandsvisning på bekostnad av långsammare visning</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Cyan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Gult</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Svart</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="unfinished">Skriv ut...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Stäng</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopiera hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Flytta hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">Klistr&amp;a in</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Visa &amp;marginaler</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Visa &amp;ramar</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Visa b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Visa stö&amp;drutor</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Visa stö&amp;dlinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Visa &amp;baslinjerna</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Lås till rutmönster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Lås till stödlin&amp;jerna</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Fil:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Ursprunglig PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Verklig PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Länkad text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Textram</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Text på kurvlinje</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Stycken:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Ord:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Tecken:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Skriv ut:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Aktiverad</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Inaktiverad</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">In&amp;fo</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Hämta bild...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">B&amp;ild synlig</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Uppdatera bild</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">R&amp;edigera bild</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Anpassa ram till bild</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Hämta text</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">L&amp;ägg till text...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">R&amp;edigera text...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">� ett PDF-&amp;bokmärke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">� en PDF-a&amp;nteckning</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Egenskaper för anteckning</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Fält e&amp;genskaper</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF-alternativ</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Redigera text...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Lås</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås upp</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Lås objekt&amp;storlek</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Lås upp objekt&amp;storlek</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Skicka till &amp;klippbok</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Skicka till la&amp;ger</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Infoga dummytext</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Gruppera</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Lås upp &amp;grupp</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Ni&amp;vå</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Flytta till &amp;nederst</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Fly&amp;tta till överst</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">Flytta &amp;ned en nivå</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Flytta &amp;upp en nivå</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">&amp;Bildram</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Pol&amp;ygon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Konturer</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Textram</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezierkurva</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Konve&amp;rtera till</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Klipp u&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Rensa innehå&amp;ll</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Visa egenskape&amp;r</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Göm egenskape&amp;r</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Vill du verkligen ta bort all text?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Programmet</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">saknas!</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">En kopia av</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linje</translation>
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
        <translation>Kurvtext</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">En kopia av</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Värde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Ra&amp;dera</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="unfinished">Arrangera sidorna</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="unfinished">Dokumentsidor:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Sida</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete">av %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Letter</translation>
    </message>
    <message>
        <source>Government Letter</source>
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
        <translation type="unfinished">Medium</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Sökresultat för:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Förhandsvisning</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Välj</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Bilder</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Kurva</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Sida</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Skriv ut</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gå till</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Saknas</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Sök</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Bildhantering</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished">Spara som b&amp;ild</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Importera</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Okänd</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>How to run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Fil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nej</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygonegenskaper</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Hö&amp;rn:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotation:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Antal hörn för polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Rotationsvinkel för polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Förhandsvisning av polygon</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Använd &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Använd konvex-/konkavfaktorn för att ändra formen på poygonen</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Ett negativt värde gör polygonen konkav (stjärnformad) och
ett positivt värde gör den konvex</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotation:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Använd &amp;faktor</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Antal hörn för polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Rotationsvinkel för polygoner</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished">Använd konvex-/konkavfaktorn för att ändra formen på poygonen</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Förhandsvisning av polygon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished">Ett negativt värde gör polygonen konkav (stjärnformad) och
ett positivt värde gör den konvex</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Inställningar</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allmänt</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Stödlinjer</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Verktyg</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Klippbok</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visning</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">Externa verktyg</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">�rigt</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>T&amp;ypgrad:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Enheter</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Punkter (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Tum (tum)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pica (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Inställningar för musen</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Mus&amp;hjul:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete">px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">Hämt&amp;a radien:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menyer</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Senas&amp;te dokument:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Sökvägar</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokument:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skript:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidstorlek</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höjd:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Uppslag</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Vänster &amp;sida först</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marginallinjer</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>�amp;verst:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vänster:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autospar</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">Aktiv&amp;erad</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Rutmönster</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">St&amp;orlek smårutor:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Stor&amp;lek stora rutor:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Låsradie till stödlinjer:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Färg på rutmönster</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Smårut&amp;or:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Sto&amp;ra rutor:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">A&amp;nvändarens stödlinjer:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">&amp;Baslinjer:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Placering</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">I &amp;bakgrunden</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">I för&amp;grunden</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Baslinjer</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">&amp;På</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">A&amp;v</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Nedsänkt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Förflyttning</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Storleksförändring:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Upphöjt</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Förfl&amp;yttning</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">S&amp;torleksförändring:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kapitäler</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Stor&amp;leksförändring:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Annat</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Baslin&amp;jer:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Baslinjer o&amp;ffset:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Automatiskt rada&amp;vstånd:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Standard&amp;teckensnitt:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">&amp;Storlek:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">&amp;Färg på text:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Spa&amp;lter:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">Spalt&amp;mellanrum:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Färg på &amp;linje:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Toning:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">&amp;Fylnadsfärg:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">To&amp;ning:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">Linjet&amp;yp:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">Linje&amp;bredd:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Ma&amp;ximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Steglängd:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">&amp;Fri storleksförändring</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Horisontell storleksförändring:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">&amp;Vertikal storleksförändring</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">För&amp;stora bilden till ramstorlek</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Behåll p&amp;roportioner</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">F&amp;yllnadsfärg:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Hörn:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotation:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Andra alternativ</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Spara innehåll &amp;vid ändringar</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Förhandsvisning</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Liten</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Medium</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Stor</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Vi&amp;sa sidor som uppslag</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Sidfärg</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Bakgrund:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Marginaler:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Visa icke &amp;utskrivbar yta i marginalfärg</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Använd egenskaper i PDF 1.4 för &amp;transparens </translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Anpassa visningsstorlek</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">För att anpassa visning drag markören nedan till önskat värde</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Postscripttolk</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Namn på program.</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Anti-alias &amp;text</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Anti-alias &amp;grafik</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Verktyg för bildhantering</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Namn på &amp;program:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Utskrift</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Klipp efter sidans &amp;marginaler</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Använd &amp;underfärgsborttagning</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Använd standardval för fönsterdekor och utseende.
Scribus ärver tillgängliga teman från KDE och Qt.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Standardteckengrad för menyer och fönster</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standardmåttenhet för redigering av dokument</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Antal rader som Scribus rullar för varje steg på mushjulet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Radien för det område som Scribus tillåter att du markerar handtagen på ett objekt</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Antal tidigare redigerade dokument som ska visas i Fil-menyn</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standardkatalog för dokument</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Standardkatalog för ICC-profiler</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standardkatalog för skript till Scripter</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard sidstorlek</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standardriktning för dokumentsidor</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentets sidbredd, redigerbar om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentets sidhöjd, redigerbar om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktivera enkelsidig eller dubbelsidig layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Gör dokumentets förstasida till en vänstersida</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan övre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan nedre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan vänster marginallinje och papperskanten.(new line)
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan höger marginallinje och papperskanten.(new line)
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Om aktiverad kommer Scribus att spara en backup av din fil med filtillägget .bak
efter varje tidsperiod</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Tidsperiod mellan automatspar</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Avstånd mellan smårutor i stödraster</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Avstånd mellan stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Avstånd inom vilket ett objekt låses till dina utplacerade stödlinjer</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Färg på smårutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Färg på stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Färg på införda stödlinjer</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Placera stödraster bakom sidans objekt</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Placera stödraster framför sidans objekt</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Slå på standardraster</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Slå av standardraster</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Förskjutning ovanför teckensnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Relativ storlek för upphöjd text jämfört med normal teckengrad</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Förskjutning nedanför typsnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Relativ storlek för nedsänkt text jämfört med normal typgrad</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Relativ storlek för kapitäler jämfört med normal typgrad</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Procentuell ökning av radavstånd baserat på typgraden </translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Egenskaper för textramen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Egenskaper för bildram</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Egenskaper för geometriska figurer</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Standardvärden för förstoringsgrad</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Egenskaper för linjeritning</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Egenskaper för polygonritning</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Typsnitt i nya textramar</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Typgrad för nya textramar</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Färg på typsnitt</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Antal spalter i textramen</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Spaltmellanrum mellan spalter i textramen</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Typsnittsexempel</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Bildramar tillåter bilder att storleksförändras till vilken storlek som helst</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Horisontell storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Vertikal storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Bibehåll proportionerna vid storleksförändring</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Bilder i bildram storleksförändras till ramens storlek</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Automatiskt storleksförändrade bilder bibehåller ursprungliga proportioner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Bildramens fyllnadsfärg</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Fyllnadsfärgens mättnad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Färg på formens konturlinje</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Linjefärgens mättnad</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Formernas fyllnadsfärg</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Formernas linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Formernas linjebredd</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Minsta tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Största tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">�dring av förstoringsgrad vid varje zoomning</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Färg på linjer</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Färgmättnad</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Stil på linjer</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Linjebredd</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Antal hörn för polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Rotationsvinkel för polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Förhandsvisning av polygon</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Välj storlek på förhandsvisning i Klippboken</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Spara innehållet i Klippboken efter varje ändring</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Vid användandet av motstående sidor visa dessa sida vid sida</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Färg för papper</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Färg för marginallinjer</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Markera område utanför marginalerna i samma färg som marginallinjerna</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Aktivera funktionerna för transparens vid export som PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Ange standardvärde för zoomnivå</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Ghostscript-tolkens plats i filsystemet</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Anti-alias av text vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Anti-alias av grafik vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Bildbehandlingsprogrammets plats i filsystemet</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Visa inte objekt utanför marginalerna vid utskrift eller export av filen</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Välj en katalog</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Innanför:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Utanför:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">&amp;Mallar:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>�amp;ndra...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Använd &amp;faktor</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Ytterligare katalog för dokumentmallar</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Använd konvex-/konkavfaktorn för att ändra formen på poygonen</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Ett negativt värde gör polygonen konkav (stjärnformad) och
ett positivt värde gör den konvex</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En metod för att ta bort en del av de gråtoner som kan uppstå
vid blandning av cyan, gult och magenta genom att ersätta med svart.
UCR påverkar de delar av bilden som är neutrala och/eller har mörka toner
i närheten av grått. Användande av UCR kan förbättra tryckkvalitetn för en del bilder
och för bästa resultat krävs testning och experiment från fall till fall.
UCR förhindrar risken för övermättnad med CMG färger.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Ledger</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">S&amp;pråk:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Färg:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>dpi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Fil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nej</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Vänster sida</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Höger sida</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guttenberg</source>
        <translation type="unfinished"></translation>
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
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Guttenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">�amp;ppna...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Spara &amp;som...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation type="unfinished">Skriptkonsol</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varning</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
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
        <translation>Fär&amp;gton:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>Färgmä&amp;ttnad:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>Färg&amp;valör:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Röd:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grön:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;å:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lfakanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Basfärger</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Anpassade färger</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definiera anpassade färger &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>Lägg till till &amp;anpassade färger</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Markera färg</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopiera eller flytta en fil</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Läs: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Skriv: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Fil&amp;namn:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Fil&amp;typ:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>En katalognivå högre</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alla filer (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Storlek</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Titta &amp;i:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Tillbaka</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Skapa ny mapp</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Listvy</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Detaljvy</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Förhandsvisa filinformation</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Förhandsvisa Filinnehåll</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Läs-Skriv</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Enbart läsning</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Enbart skrivning</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Ej åtkomlig</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symbolisk länk till fil</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symbolisk länk till katalog</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symbolisk länk till Special</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Katalog</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Special</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>�amp;ppna</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Byt namn</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>�&amp;erskapa</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Sortera efter &amp;namn</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Sortera efter &amp;storlek</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Sortera efter &amp;datum</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Osorterad</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sortera</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Visa &amp;dolda filer</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>filen</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>katalogen</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>symboliska länken</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Ta bort %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Vill du verkligen ta bort %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Ny mapp 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Ny mapp</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Ny mapp %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Hitta katalog</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Kataloger</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Spara</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fel</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Filen hittades inte.
Kontrollera sökväg och filnamn.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alla filer (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Välj en katalog</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Katalog:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Snittvar&amp;ianter</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Storlek</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>�ersty&amp;kning</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Understrykning</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Färg</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Prov</translation>
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
        <translation>Utför</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Stäng</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Välj teckensnitt</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Rensa</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Markera allt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>�&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Gör &amp;om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Klipp u&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiera</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Klistr&amp;a in</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Rada upp</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Anpassa...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Om Qt&lt;/h3&gt;&lt;p&gt;Detta program använder Qt version %1.&lt;/p&gt;Qt är en C++-verktygslåda för utveckling av grafiska gränssnitt på flera plattformar&lt;/p&gt;&lt;p&gt;Qt tillhandahåller portabilitet mellan MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux och alla större kommersiella versioner av Unix.&lt;br&gt;Qt finns också tillgängligt för inbäddade system.&lt;/p&gt;&lt;p&gt;Qt är en produkt från Trolltech, se &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; för mer information.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Hämtar text</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alla format som stöds</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alla filer (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>initierar...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrund</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varning</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Vill du verkligen skriva över filen:
%1?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Infoga special</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Ny &amp;från mall...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Nyhetsbrev</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Broschyrer</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Kataloger</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Flygblad</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Skyltar</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kort</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Brevhuvuden</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Kuvert</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visitkort</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendrar</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Annonser</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiketter</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menyer</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programblad</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF formulär</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF presentationer</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Tidskrifter</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Affischer</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Tillkännagivanden</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Textdokument</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Foldrar</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Egna mallar</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Spara som b&amp;ild</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Spara som bild</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Fel  vid skrivning till fil(er).</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fel  vid skrivning till fil(er).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Exporten lyckades.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Filen existerar redan. Vill du skriva över?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>existerar redan. Villdu skriva över?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ja, allt</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Skriv ut förhandsvisning</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Skriv ut förhands&amp;visning</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Importera &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alla format som stöds (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Spara som &amp;mall...</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Oj! Du försöker ladda in en bild till ett objekt som inte finns eller inte är valt!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Oj! Du försöker llåsa/låsa uppl ett objekt som inte finns! Inte heller är något valt.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Oj! Du försöker få kontakt med ett objekt som inte finns! Inte heller är något valt.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oj! Du försöker få kontakt med ett objekt som inte finns!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oj! Du försöker ta bort ett objekt som inte finns!</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Oj! Objektet du vill flöda finns inte!</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">S&amp;cripter handbok...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">&amp;Scribus skripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Utför skript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">Se&amp;naste skript</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Visa &amp;konsol</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">Skri&amp;pt</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Online referenshandbok</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skript (*.py);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Spara sida som &amp;SVG</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-bilder (*.svg *.svgz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-bilder (*.svg);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Importera &amp;SVG...</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Filer med kommaseparerade värden (CSV)</translation>
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
        <translation>HTML-filer</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Textfiler</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Teckensnitt %1 är skadat, använder det inte</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Mall:</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Externa länkar
</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Textfilter</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanska</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskiska</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgariska</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasilianska</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalanska</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Kinesiska</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tjekiska</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danska</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holländska</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engelska</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Engelska (British)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Tyska</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finska</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Franska</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galiciska</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grekiska</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungerska</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesiska</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italienska</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreanska</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litauiska</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norska (Bokmål)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norska (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norska</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polska</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ryska</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Svenska</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanska</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spanska (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovakiska</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovenska</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbiska</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Försökte sätta progress &gt; maximum progress</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished">&amp;Om Skript...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished">Om Skript</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta teckengraden från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta teckensnitt från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta textstorlek från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta spaltantal från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta radavstånd från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta spaltmellanrum från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta text från icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte ange text för icke-text ram.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan inte infoga text i icke-text ram.</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="obsolete">Infogningsindex är felaktigt</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Justeringen felaktig. Använd någon konstanterna scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange textjustering i en icke-text ram</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Teckengrad felaktig - måste vara 1 &lt;= size &lt;= 512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange teckengrad för en icke-text ram</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange teckensnitt för en icke-text ram</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Teckensnitt hittades inte</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Radavstånd felaktigt, måste vara &gt;= 0.1</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange radavstånd för en icke-text ram</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Spaltmellanrum felaktigt. måste vara positivt</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange spaltmellanrum för en icke-text ram</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Antalet spalter felaktigt, måste vara &gt; 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange antalet kolumner för en icke-text ram</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Markeringsindex felaktigt</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte markera text i en icke-text ram</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ta bort text i en icke-text ram</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange fyllnadsfärgen på text i en icke-text ram</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange konturfärgen på text i en icke-text ram</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kan inte ange skuggfärg på text i en icke-text ram</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Endast textramar kan länkas</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Målramen måste vara tom</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Målramen länkar till en annan ram</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Målramen är redan länkad från en annan ram</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Källa och mål är samma objekt</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="unfinished">Afrikaan</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Anpassad</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished">p</translation>
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
        <translation type="unfinished">tum</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="unfinished">Punkter (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Tum (tum)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Pica (p)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">E&amp;rsätt</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Allt</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Portugisiska</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Scribus har avbrutits</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Scribus har avbrutits på grund av signal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>2A0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comm10E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DLE</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open output file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
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
        <source>Freetype2 library not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
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
        <source>Uses xx as shortcut for a language</source>
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
        <source>Scribus Version </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> cm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>cm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>c</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">&amp;Förhandsvisning av teckensnitt...</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
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
        <source>OpenDocument Text Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopia #%1 av</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Svart</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Cyan</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Gult</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation type="unfinished">Ny från mall</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Sök efter teckensnitt</translation>
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
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
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
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Rensa</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Markera allt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>�&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Gör &amp;om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiera</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Klistr&amp;a in</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Systemmeny</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Rulla upp</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Rulla ner</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>�erställ</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimera</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximera</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Stäng</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>�e&amp;rställ</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Flytta</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Storlek</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimera</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximera</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Lägg övers&amp;t</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimera</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>�erställ nedåt</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Stäng</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Rull&amp;a upp</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>R&amp;ulla ned</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Dokumentinställningar</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marginallinjer</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>�ers&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Vänster:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Uppslag</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Vänst&amp;er sida först</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktivera enkel eller uppslagsbaserad layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Gör dokumentets förstasida till en vänstersida</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan övre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Avstånd mellan nedre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan vänster marginallinje och papperskanten.
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Avstånd mellan höger marginallinje och papperskanten.
Om Uppslag är valt kan denna marginal justeras för att uppnå korrekt marginalavstånd för bindning</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Insida:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">&amp;Utsida:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidstorlek</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Storlek:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Sidriktning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskap</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Bredd:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Höjd:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Förstas&amp;idans nummer:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Storlek:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Höjd:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Autospar</translation>
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
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Stödlinjer</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Färg:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Visa icke &amp;utskrivbar yta i marginalfärg</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="unfinished">Visning</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Typografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Verktyg</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="unfinished">Färg för papper</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Markera område utanför marginalerna i samma färg som marginallinjerna</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Alternativ</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Dokumentinformation</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Justerar färger</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Python skript (*.py);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Inställningar stilmallar</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stil på valt stycke</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Inställning fyllnadsfärg</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Fyllnadsfärg på text</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Mättnad på fyllnadsfärg för text</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Inställning av konturfärg</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Konturfärg för text</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Mättnad på konturfärg på text</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Inställningar teckensnitt</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Teckensnitt för markerad text</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Teckengrad</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Storleksförändra teckenbredd</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Inställningar för tecken</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuell kerning</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
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
        <translation type="unfinished">Spara sida som &amp;SVG</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="unfinished">Importera &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Spara som &amp;mall...</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Importera</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Okänd</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopia #%1 av</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrund</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Skapa ett nytt dokument</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">�pna ett dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Spara aktuellt dokument</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Stäng aktuellt dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Skriv ut aktuellt dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Spara aktuellt dokument som PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Sök efter teckensnitt</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Det finns inga Postscript-teckensnitt installerade</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Avslutar nu</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Allvarligt fel</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Smart avstavning</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Vänsterjustera</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Högerjustera</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Centrera</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Infoga sidnummer</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Sammanfoga text och kurva</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Visa lager</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Javascripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">�gra</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Visa sidpalett</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Lås/Lås upp</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Hårt mellanslag</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Läser inställningar</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Hämtar ICC profiler</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Startar avstavningen</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Skapar genvägar</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Färghantering...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Läser Klippboken</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Startar insticksprogram</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">�amp;ppna...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>�pna s&amp;enaste</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Ny</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">�pna...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Stäng</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Spara</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Spara &amp;som...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Spara som...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">�er&amp;gå till senast sparad</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Samla ihop för utskrift...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Hämta text/bild...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Lägg &amp;till text...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Im&amp;portera sida/sidor:</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importera</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Spara &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Spara sida som &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Spara som P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportera</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Dokument&amp;information...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Dokumentinformation...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Dokumentin&amp;ställningar...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Dokumentinställningar...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Skriv ut...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Skriv ut...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Avsluta</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Avsluta</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">�&amp;gra</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Klipp u&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiera</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">Klistr&amp;a in</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Ra&amp;dera</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">M&amp;arkera allt</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Klipp ut</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopiera</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Klistra in</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Rensa</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Markera allt</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Sök/Ersätt...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">Fä&amp;rger...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Färger...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">St&amp;yckemallar...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">&amp;Linjemallar...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Stilmallar...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Mallar...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Mallar...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Välj nytt teckensnitt</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">D&amp;uplicera</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplicera</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Duplicera flera gånger</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Duplicera flera gånger</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Ta bort</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Gruppera</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Gruppera</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">Lås &amp;upp grupp</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Lås upp grupp</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Lås</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Flytta till &amp;nederst</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Flytta till nederst</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Fly&amp;tta till överst</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Flytta till överst</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">F&amp;lytta ned en nivå</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Flytta ned en nivå</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Flytta &amp;upp en nivå</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Flytta upp en nivå</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Fördel&amp;a/Justera...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Fördela/Justera...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">R&amp;edigera form</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">S&amp;ammanfoga text och kurva</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">Lös&amp;gör text från kurva</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">Sammanf&amp;oga polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Dela upp &amp;polygoner</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">K&amp;onvertera till konturer</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Infoga...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Infoga...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Ta bort...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Flytta...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Flytta...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Använd mall...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Använd mall...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Hantera stö&amp;dlinjer...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Hantera stödlinjer...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">Anpassa till &amp;fönster</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Anpassa till fönster</translation>
    </message>
    <message>
        <source>50%</source>
        <translation type="obsolete">50%</translation>
    </message>
    <message>
        <source>75%</source>
        <translation type="obsolete">75%</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">Minia&amp;tyrer</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Miniatyrer</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Visa &amp;marginaler</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Dölj marginaler</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Visa &amp;ramar</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Dölj ramar</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Visa b&amp;ilder</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Dölj bilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Visa stö&amp;drutor</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Dölj stödrutor</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Visa stö&amp;dlinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Visa &amp;baslinjerna</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Lås till rutmönster</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Lås till rutmönster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Lås till stödlin&amp;jerna</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">Egenska&amp;per</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Egenskaper</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">�amp;versikt</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">�ersikt</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Klippbok</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Klippbok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Lager</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">Sidp&amp;alett</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Bokmärken</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Bild&amp;hantering</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Bildhantering</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">A&amp;vstava text</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Avstava text</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Verktygsti&amp;ps</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Ver&amp;ktyg</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">P&amp;DF verktyg</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Verktyg</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Verktygstips</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">Inställninga&amp;r...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">T&amp;ypsnitt...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Teckensnitts...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">A&amp;vstavning...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">Sna&amp;bbkommandon...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Om &amp;Qt</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">Scribus &amp;handbok</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">On-line hjälp...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;il</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>E&amp;nhet</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Sida</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Vy</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">Ins&amp;tällningar</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Fönste&amp;r</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjälp</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Vänster</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Centrerat</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Marginajustera</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Hård marginaljustering</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Annan...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Understruken</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">�erstruken</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kapitäler</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Upphöjt</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Nedsänkt</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Konturerad</translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Klar</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Kaskad</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Sida vid sida</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Hämta text/bild...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Hämta bild...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Färg</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Invertera</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Hämta text</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Storlek</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Justering</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Nyan&amp;s</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabulatorer...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Hämta text...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Teckensnitt</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Storlek</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Stil</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Färg</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Nyans</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås upp</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importerar sida/sidor...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importera sida/sidor</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Du försöker importera fler sidor än det finns i aktuellt dokument räknat från aktiv sida.&lt;/p&gt;
Välj något av följande:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Skapa&lt;/b&gt; de saknade sidorna&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importera&lt;/b&gt;befintliga sidor&lt;/li&gt;
&lt;li&gt;&lt;b&gt;Avbryt&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Skapa</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importera</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import klar</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Hittade inget att importera</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Laddar...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alla format som stöds</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alla filer (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Kan inte skriva till fil:
%1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Sparar...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Skriver ut...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Utskriften misslyckades!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus handbok</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textfiler (*.txt);;Alla filer (*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Nyan&amp;s:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Dölj baslinjerna</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Visa baslinjerna</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Följande program saknas:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript: Du kan inte använda EPS-bilder</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Allt</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-filer (*.eps);;Alla filer (*)</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Några objekt är låsta</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Lås alla</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Lås upp alla</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Lås upp</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Lås</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Laddar:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Justerar färger</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">�&amp;gra Ta bort objekt</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">�&amp;gra Flytta objekt</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">�&amp;gra �dra objekt</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Tyska</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Polska</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Engelska</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Spanska</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italienska</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Franska</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Ryska</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Danska</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slovakiska</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Ungerska</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Tjekiska</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Holländska</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugisiska</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrainska</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Grekiska</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalanska</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finska</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irländska</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Litauiska</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Svenska</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovenska</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Välj en katalog</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Invertera</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Scribus har avbrutits</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus har avbrutits på grund av signal #%1</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Filen %1 är inte i Scribusformat</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikaan</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Teckensnittshanteringen initierad</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="obsolete">Gör &amp;om</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="obsolete">&amp;Understrykning</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="obsolete">R&amp;edigera form...</translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation type="obsolete">&amp;50%</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation type="obsolete">&amp;75%</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation type="obsolete">&amp;200%</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulgariska</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Programmet</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Information</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">saknas!</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Skicka till la&amp;ger</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;PDF-alternativ</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Tecken</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Spara som</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Namn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Prov</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation type="unfinished">Nytt lager</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Bakgrund</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Lager</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Allt</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Kopiera hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Flytta hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="unfinished">Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Fil:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">Ursprunglig PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">Verklig PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Länkad text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Textram</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Text på kurvlinje</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Stycken:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Ord:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Tecken:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Skriv ut:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Aktiverad</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Inaktiverad</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">In&amp;fo</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">B&amp;ild synlig</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Uppdatera bild</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">R&amp;edigera bild</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Anpassa ram till bild</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">R&amp;edigera text...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">� ett PDF-&amp;bokmärke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">� en PDF-a&amp;nteckning</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Egenskaper för anteckning</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Fält e&amp;genskaper</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;PDF-alternativ</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Redigera text...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Lås</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås upp</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Lås objekt&amp;storlek</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Lås upp objekt&amp;storlek</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Skicka till &amp;klippbok</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Skicka till la&amp;ger</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Infoga dummytext</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Gruppera</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Lås upp &amp;grupp</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">Ni&amp;vå</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Flytta till &amp;nederst</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Fly&amp;tta till överst</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Flytta &amp;upp en nivå</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">&amp;Bildram</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Pol&amp;ygon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Konturer</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Textram</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezierkurva</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">Konve&amp;rtera till</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Rensa innehå&amp;ll</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">En kopia av</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Okänd</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMGS</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">Avs&amp;luta ändå</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">S&amp;täng ändå</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>har ändrats sedan senast sparat.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Spara nu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">&amp;Scribus skripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Utför skript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">Se&amp;naste skript</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;Om Skript...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">Skri&amp;pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">�pna</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skript (*.py);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation type="unfinished">Skriptfel</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished">Om du kör ett officiellt skript, vänligen rapportera till &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished">Detta meddelande finns också på ditt klippbord. Använd Ctrl+V för att klistra in i felhanteraren.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Göm &amp;konsolen</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">Om Skript</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Avancerat</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
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
        <translation>Förhandsvisning</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Visa mallnamn</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Sök/Ersätt</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Sök efter:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Styckemallar</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Teckensnitt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Teckengrad</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Teckensnittseffekter</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Fyllningsfärg</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Fyllningsskugga</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Konturfärg</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Konturskugga</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vänster</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrera</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Höger</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Marginaljustera</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Hård marginaljustering</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Ersätt med:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Hela ord</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorera stora/små bokstäver</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Sö&amp;k</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>E&amp;rsätt</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Ersätt &amp;alla</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Sökning avslutad</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Arrangera sidorna</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Tillgängliga mallar:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Dokumentsidor:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Uppslag</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Vänster sida först</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Dra sidor eller mallsidor till papperskorgen om du vill ta bort dem.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Förhandsvisa alla sidor i dokumentet.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Här finns alla dina mallar. För att skapa en ny sida
drar du en mall till Sidvisaren nedan.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Välj fält</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Tillgängliga fält</translation>
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
        <translation>Markerade fält</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>�rigt...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Nyan&amp;s:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Nyans</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>X-Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stilmall</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stilmall</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Redigera stilmall</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopia av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stilmall</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Vill du verkligen ta bort denna stilmall?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importera</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Texthanteraren</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>Hämta texten igen f&amp;rån textramen</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Spara till fil...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>Hämta från fi&amp;l...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Spara &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Uppdatera textramen och avsluta</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Avsluta utan att uppdat&amp;era textramen</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>M&amp;arkera allt</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opiera</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Klis&amp;tra in</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Sök/Ersätt...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Infoga special...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>R&amp;edigera stilmallar...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Förhandsvisning av teckensnitt...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Uppdatera textramen</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Bakgrund...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Visa teckensnitt...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Ins&amp;tällningar</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Rensa all text</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Hämta text från fil</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Spara text till fil</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Uppdatera textramen och avsluta</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Avsluta utan att uppdatera textramen</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Hämta texten igen från textramen</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Uppdatera textramen</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Sök/Ersätt</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktivt stycke:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tecken:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totalt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Stycken:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Vill du spara ändringarna?</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Infoga special...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Förhandsvisning av typsnitt</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Vill du verkligen förlora alla ändringar?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Vill du verkligen radera all text?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>�pna</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textfiler (*.txt);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Smart textmarkering</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Understruken</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitäler</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Nedsänkt</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Upphöjt</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>�erstruken</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Konturtext</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="unfinished">�ersikt</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Uppdatera styckemallarna</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Använd dokumentnamn som prefix för styckemallar</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Fråga inte igen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Skall importhanteraren lägga till dokumentnamnet
framför namnet på styckemallar i Scribus</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Om en styckemall med samma namn som den aktuella OpenOffice.org-mallen redan existerar
ska då stilmallen i Scribus redigeras så att den matchar den importerade, eller ska den
lämnas orörd</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Importalternativ för OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Ska importhanteraren alltid använda aktuellt värde vid import av 
dokument från OpenOffice.org och aldrig mer fråga igen</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>At the end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> px</source>
        <translation type="unfinished">px</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="obsolete">Stödlinjer</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Färg:</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Baslinjer</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished">Visa baslinjerna</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Automatiskt rada&amp;vstånd:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Baslin&amp;jer:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Baslinjer o&amp;ffset:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished">Avstånd mellan smårutor i stödraster</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Avstånd mellan stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Avstånd inom vilket ett objekt låses till dina utplacerade stödlinjer</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Radien för det område som Scribus tillåter att du markerar handtagen på ett objekt</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Färg på smårutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Färg på stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Färg på införda stödlinjer</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Färg för marginallinjer</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Procentuell ökning av radavstånd baserat på typgraden </translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Hantera tabulatorer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Skriv ut</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;Alla sidor</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">Vä&amp;lj sidor</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotation:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Alternativ för fil</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">Kompatibilit&amp;et:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">&amp;Bindning:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Vänster sida</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Höger sida</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Skapa &amp;miniatyrer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Spara &amp;länkade textramar som PDF-artiklar</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">&amp;Inkludera bokmärken</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Upplösning:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="unfinished">Bildinställningar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automatisk</translation>
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
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metod:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Maximum</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Hög</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Låg</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Minimum</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Mi&amp;nska upplösningen på bilder till:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">&amp;Allmänt</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">Bä&amp;dda in alla teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">&amp;Gruppera alla typsnitt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Inbädda</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Tillgängliga teckensnitt:</translation>
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
        <translation type="unfinished">Teckensnitt att bädda in:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Typsnitt att gruppera:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">Förhands&amp;visning</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">Tid&amp;d för bildvisning:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Tid för effek&amp;t:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">T&amp;yp av effekt:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">Rörliga lin&amp;jer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">F&amp;rån:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">R&amp;iktning:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished">sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Ingen effekt</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Inifrån</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Utifrån</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Vänster till höger</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Uppifrån och ner</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Nerifrån och upp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Höger till vänster</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Diagonalt från övre vänstra hörnet</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">&amp;Applicera effekten på alla sidor</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">&amp;Använd kryptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Lösenord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;Användare:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">�amp;gare:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Inställningar</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Tillåt &amp;utskrift av dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Tillåt än&amp;dring av dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Tillåt kop&amp;iering av text och grafik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Tillåt att lägga till &amp;anteckningar och fält</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">Säk&amp;erhet</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Allmänt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">Utmatning&amp;en är avsedd för:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Skärmen/webben</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Skrivare</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">&amp;Använd anpassade inställningar för återgivningen</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Inställningar för återgivning</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">Fre&amp;kvens:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">&amp;Punktform:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Enkel punkt</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Linje</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Ellips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Fyllda färger:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Använd ICC-profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished">�ergivningsavsikt:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished">Perceptuell</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Relativ kolormetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Mättnad</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Absolut kolormetri</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">Använd inte inbäddade ICC-profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">Fä&amp;rg</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">PDF/X-3 Utmatningsavsikt</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">&amp;informationssträng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">Utmatnings&amp;profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Trimmningsram</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished">Inbäddade typsnitt i PDF-filen. Att bädda in typsnitt(new line)
bibehåller layout och utseende på ditt dokument.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Tillåter presentationseffekter när du använder Acrobat Reader i fullskärmsläge.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Förhandsvisa varje sida i listan ovanför.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Tid som sidan vissas innan presentationen startar på vald sida.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Tid för effekten.(new line)
En kortare tis snabbar upp effekten, en längre gör den långsammare.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Typ av visningseffekt.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Riktning för effekten vid användande av effekterna Blinds eller Split.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Startposition för effekterna Box eller Split.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Riktning för effekterna Wipe eller Glitter.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Använd vald effekt på alla sidor.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Exportera alla sidor till PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Exportera en grupp av sidor till PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Infoga en kommaseparerad lista med symboler där(new line)
en symbol kan vara * för alla sidor, 1-5 för ett avsnitt(new line)
av sidor eller en enskild sida.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Avgör kompatibiliteten med PDF. Standardvärde är Acrobat 4.0 som ger störst kompatibilitet.(new line)
Välj Acrobat 5.0 om din fil har egenskaper som transparens eller du vill ha 128-bitars kryptering.(new line)
PDF/X-3 används för export för kommersiella utskrifter och är tillgängligt om du aktiverat färghantering.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Avgör sidbindningen i PDF. Om du inte vet(new line)
att du behöver ändra låter du standardvärdet - vänster - vara kvar.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished">Skapar miniatyrer av alla sidor i PDF-filen.(new line)
En del PDF-läsare kan använda dessa för navigering.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Skapar PDF-artiklar, användbart för navigering av länkade artiklar i PDF</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Bäddar in de bokmärken du skapat i dokumentet.(new line)
Användbart för navigering av långa PDF dokument.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished">Exportera upplösning för text och vektorgrafik.(new line)
Detta påverkar inte upplösningen för punktuppbyggd grafik, t. ex. foton.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Kompression av text och grafik.(new line)
Om du inte behöver ändra låter du detta var ikryssat. Det minskar storleken på PDF-filen.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressionsalternativ för bilder.(new line)
Automatisk låter Scribus välja den lämpligaste metoden.(new line)
ZIP passar bilder med rena färger/tonplattor.(new line)
JPEG skapar små PDF-filer om du har många foton i dokumentet (med en viss kvalitetsförlust).(new line)
Om du inte behöver någon speciell kompression, använd automatisk.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Kompressionsnivåer: Minimum (25%), Låg (50%), Medium (75%), Hög (85%), Maximum (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Minska upplösningen på dina bitmappade bilder till vald DPI.(new line)
Om du inte markerar kryssrutan återges de med urprunglig upplösning.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">(DPI (Dotr Per Inch) för export av bilder.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktivera säkerhetsfunktionerna i din exporterade PDF-fil.(new line)
Om du valt Acrobat 4.0 används en 40-bitars kryptering i din PDF-fil.(new line)
Om du valt Acrobat 5.0 används en 128-bitars kryptering i din PDF-fil.(new line)
Observera! Krypteringen i PDF är inte lika pålitlig som GPG eller PGP och har vissa begränsningar.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="unfinished">Välj ett huvudlösenord som aktiverar eller avaktiverar alla(new line)
säkerhetsfunktioner i din exporterade PDF-fil.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Välj ett lösenord för dina användare som kan användas för att läsa din PDF-fil.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Tillåt utskrift av PDF-filen. Om ej markerad så tillåts ej utskrift.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Tillåt ändring av PDF-filen. Om ej kryssad är ändring förhindrad.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Tillåt kopiering av text eller grafik från din PDF-fil.(new line)
Om ej kryssad kan text och grafik inte kopieras.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Tillått tillägg av anteckningar och fält till din PDF-fil.(new line)
Om ej kryssat är tillägg av anteckningar och fält inte tillåtet.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished">Färgmodell för utmatning av din PDF-fil.(new line)
Välj Skärm/Webben för PDF-filer som ska visas på skärmen eller för utskrift på enklare bläckstråleskrivare.(new line)
Välj Skrivare för utskrift på skrivare för äkta 4-färg CMGS.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">Detta är en avancerad inställning, ej aktiverad från start. Den skall endast aktiveras(new line)
om din tryckare särskilt kräver det och ger dig nödvändiga uppgifter.(new line)
Annars riskerar din exporterade PDF-fil att inte tryckas korrekt och portabiliteten mellan olika system försvinner.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished">Bädda in en färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished">�ergivningsavsikt för tonplattor</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Bädda in en färgprofil för bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">Använd inte färgprofiler som är inbäddade i ursprungliga bildfiler.</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Färgprofil för bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">�ergivningsavsikt för bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Utmatningsprofil för utskrift. Om möjligt bör du rådgöra med din tryckare för korrekt val av profil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Obligatorisk sträng för PDF/X-3 annars följer inte PDF-filen(new line)
reglerna för PDF/X-3. Vi rekommenderar att du använder dokumentets titel.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Avstånd för utfall räknat från övre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Avstånd för utfall räknat från nedre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Avstånd för utfall räknat från vänstra kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Avstånd för utfall räknat från högra kanten av pappret</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Spegla sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Spegla sida(or) vertikalt</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression &amp;Method:</source>
        <translation type="unfinished"></translation>
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
        <source>Enables lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF file size.</source>
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
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Teckensnitt:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Storlek:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">Färg på &amp;linje:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Toning:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">&amp;Fylnadsfärg:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">To&amp;ning:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Linje&amp;bredd:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">&amp;Horisontell storleksförändring:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">&amp;Vertikal storleksförändring</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">För&amp;stora bilden till ramstorlek</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">Behåll p&amp;roportioner</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">F&amp;yllnadsfärg:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">Ma&amp;ximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Steglängd:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Egenskaper för textramen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Egenskaper för bildram</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Egenskaper för geometriska figurer</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Standardvärden för förstoringsgrad</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Egenskaper för linjeritning</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Egenskaper för polygonritning</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Typsnitt i nya textramar</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished">Typgrad för nya textramar</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Färg på typsnitt</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Antal spalter i textramen</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Spaltmellanrum mellan spalter i textramen</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Typsnittsexempel</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Bildramar tillåter bilder att storleksförändras till vilken storlek som helst</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Horisontell storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Vertikal storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished">Bibehåll proportionerna vid storleksförändring</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Bilder i bildram storleksförändras till ramens storlek</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Automatiskt storleksförändrade bilder bibehåller ursprungliga proportioner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Bildramens fyllnadsfärg</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Fyllnadsfärgens mättnad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Färg på formens konturlinje</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Linjefärgens mättnad</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Formernas fyllnadsfärg</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Formernas linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Formernas linjebredd</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Minsta tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Största tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">�dring av förstoringsgrad vid varje zoomning</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Färg på linjer</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Stil på linjer</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Linjebredd</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Anpassad</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Nedsänkt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">&amp;Förflyttning</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">&amp;Storleksförändring:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Upphöjt</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Förfl&amp;yttning</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">S&amp;torleksförändring:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Kapitäler</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">Stor&amp;leksförändring:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished">Förskjutning ovanför teckensnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Relativ storlek för upphöjd text jämfört med normal teckengrad</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Förskjutning nedanför typsnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Relativ storlek för nedsänkt text jämfört med normal typgrad</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Relativ storlek för kapitäler jämfört med normal typgrad</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Understruken</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="unfinished">Linjebredd:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">�erstruken</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Procentuell ökning av radavstånd baserat på typgraden </translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Vänster</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Höger</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Fast utslutning</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrera</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Första &amp;raden:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">I&amp;ndrag:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Ta bort allt</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Indrag från första raden i stycket</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Vänsterindrag för hela stycket</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Rensa alla tabulatorer</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">tum</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">Vänster in&amp;drag:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Inget</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Anpassad</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>�ersikt</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Information</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Namnet &quot;%1&quot; är inte unikt.
Vänligen välj ett annat.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="obsolete">X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="obsolete">Y:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="obsolete">Teckensnitt:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppera</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Bild</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Linje</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Kurvtext</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Sida</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Sida</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="unfinished">Gruppera</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Skapa</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="unfinished">Lås</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Lås upp</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Ta bort</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Byt namn</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="unfinished">Klistra in</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Klipp ut</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create table</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set language</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Polygon</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Polyline</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Get image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Text på kurvlinje</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Action History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">�&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">Gör &amp;om</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Infoga värde</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Skriv in värde och klicka sedan OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Skriv in värde och klicka sedan OK</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Skicka dit värde till skriptet</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hjälp</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Verktyg</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Markera enheter</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Infoga textram</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Infoga bild</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Infoga tabell</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaper...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Infoga polygoner</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Infoga linjer</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Infoga Bezierkurvor</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Infoga frihandslinjer</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Rotera enhet</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Zooma in eller ut</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Redigera raminnehåll</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Redigera text med Texthanteraren</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Länka textramar</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Avbryt länk mellan textramar</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Mätning</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Rita olika former</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-verktyg</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfält</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kryssruta</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Radioknapp</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Infoga PDF-fält</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Länk</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Infoga PDF-anteckning</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Välj tecken:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Infoga</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Infoga tecken vid markören i texten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Ta bort aktuell(a) markering(ar).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Stäng denna dialogruta och återvänd till textredigering.</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Välj importhanterare</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Hämta endast text</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importera text utan formattering</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importhanterare:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodning:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Välj importhanterare</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Kom ihåg associationen</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Kom ihåg associationen filändelse - importhanterare
och fråga inte igen när du väljer importhanterare
för denna filtyp.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Ny från mall</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>�amp;ppna</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Allt</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidstorlek</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Färger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivning</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Användning</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Skapad med</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Författare</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Laddar ner mallar</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokumentmallar finns på http://www.scribus.net i sektionen Downloads.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Installerar mallar</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Packa upp paketet i mallkatalogen ~/.scribus/templates för aktuell användare eller PREFIX/share/scribus/templates för alla användare på systemet.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Förbereder en mall</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Se till att bilder och teckensnitt får användas fritt. Om teckensnitt inte får delas sparar du dem inte tillsammans med mallen.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Skaparen av mallen måste också se till att avdelningen Installation av mallar gäller även den egna mallen. detta betyder att en användare ska kunna ladda ner ett mallpaket och kunna packa upp det i mallaktalogen och sedan börja använda den.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Ta bort en mall</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Att ta bort en mall från dialogrutan Ny från mallar tar endast bort posten från template.xml. Dokumentfilerna kommer inte att tas bort. En popup-meny med Ta bort visas endast om du har skrivrättigheter till filen template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>�ersätter template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopiera en existerande template.xml till en fil kallad template.lang_COUNTRY.xml (använd samma landskod som finns i qm-filen för ditt språk). För till exempel en finsk template.xml skriver du template.fi.xml. Kopian måste sparas i samma katalog som den ursprungliga template.xml för att Scribus ska hitta den.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Spara som mall</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidstorlek</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Färger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivning</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Användning</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Författare</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-post</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Fler detaljer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Färre detaljer</translation>
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
        <translation>landskap</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>porträtt</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>anpassad</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Skapa filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Välj ett tidigare sparat filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Ange ett namn på filtret för att spara</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Ange ett namn för att spara</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Aktivera eller avaktivera denna filterrad</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Ta bort denna filterrad</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Lägg till en ny filterrad</translation>
    </message>
    <message>
        <source>to</source>
        <translation>till</translation>
    </message>
    <message>
        <source>and</source>
        <translation>och</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>ta bort matchning</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ta inte bort matchning</translation>
    </message>
    <message>
        <source>words</source>
        <translation>ord</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Ta bort</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Ersätt</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Utför</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Värdet till vänster är ett reguljärt uttryck</translation>
    </message>
    <message>
        <source>with</source>
        <translation>med</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>styckemall</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>alla instanser av</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>alla stycken</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>stycken som börjar med</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>stycken med mindre än</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>stycken med mer än</translation>
    </message>
</context>
</TS>
