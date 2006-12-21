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
        <translation>newPage(where [,&quot;masterpage&quot;])

Skapar en ny sida. Om &quot;where&quot; är lika med -1 läggs den nya sidan till
dokumentet. Annars infogas den nya sidan före &quot;where&quot;. Sidnumreringen
räknas från 1 och uppåt oavsett vilket nummer som visas på första sidan
i dokumentet. Den extra parametern &quot;masterpage&quot; anger vilken mallsida
som skall användas för den nya sidan.

Om sidnumret är orimligt kan ett felindex skapas
</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;string&quot;)

Strängen måste vara ett giltigt filnamn till en SVG-bild. Texten
måste vara UTF-8 kodad eller en &apos;unicode&apos;-sträng (rekommenderas).
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

The values for width, height and the margins are expressed in the given unit
for the document. PAPER_* constants are expressed in points. If your document
is not in points, make sure to account for this.

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.
</source>
        <translation type="obsolete">newDocument(size, margins, orientation, firstPageNumber,
                        unit, pagesType, firstPageOrder) -&gt; bool

Skapa ett nytt dokument och returnera SANT om det lyckas. Parametrarna
har följande innebörd:

size = En tupel (bredd, höjd) som beskriver dokumentets storlek. Du kan
använda den fördefinierade konstanten PAPER_&lt;format&gt;, t. ex. PAPER_A4 etc.

margins = En tupel (vänster, höger, överkant, nederkant) som beskriver
dokumentets begränsningslinjer

orientation = Sidriktning - konstanterna PORTRÄTT, LANDSKAP

firstPageNumber = Numret på den första sidan i dokumentet och används för 
sidnumreringen. Även om du vanligtvis vill ha 1 är det ibland nödvändigt 
med högre nummer, t. ex. när dokumentet delats upp i olika delar.

unit: this value sets the measurement units used by the document. Use a(new line)
predefined constant for this, one of: UNIT_INCHES, UNIT_MILLIMETERS,(new line)
UNIT_PICAS, UNIT_POINTS.(new line)
unit -- detta värde anger den måttenhet som ska användas i dokumentet. Använd
en fördefinierad konstant till detta, t. ex någon av: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICA, UNIT_POINTS.</translation>
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
        <translation>Översä&amp;ttningar</translation>
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
        <translation>Övrig dokumentation:</translation>
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
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>%1 %2 %3</source>
        <translation>%1%2%3</translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation>%3-%2-%1 %4 %5</translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation>Använder Ghostscript version %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ingen Ghostscript-version tillgänglig</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Bygg-ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Den här panelen visar version, byggdatum och inkompilerat stöd i Scribus. C-C-T-F ska uttydas som C=littlecms C=CUPS T=stöd för TIFF och F=stöd för Fontconfig. Sista bokstaven anger renderaren C=cairo eller A=libart. Ett saknat stödbibliotek visas med *. Detta anger också med vilken version av Ghostscript som Scribus har identifierat.</translation>
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
        <translation>Ikoner från projekt Tango:</translation>
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
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nej</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Filnamn:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Aktiverad:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Publiceringsdatum:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivning:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Författare:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licens:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: Om insticksprogram</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
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
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ppna...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Sp&amp;ara som...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Åter&amp;gå till senast sparad</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Samla ihop för &amp;utskrift...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Hämta text...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Lägg &amp;till text...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Hämta bild...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Spara &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Spara sida som &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Spara som P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokumentin&amp;ställningar...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Skriv ut...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Avsluta</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ån&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Gö&amp;r om</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Akt&amp;ivitetsläge för enhet</translation>
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
        <translation>K&amp;listra in</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Rensa innehå&amp;ll</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>M&amp;arkera allt</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>Ångra &amp;markering</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Sök/Ersätt...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Redigera bild...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Fä&amp;rger...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>St&amp;yckemallar...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Linjemallar...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Mallsidor...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>Inställninga&amp;r...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Annan...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Vänster</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Centrerat</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>Höge&amp;r</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Marginajustera</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Hård marginaljustering</translation>
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
        <translation>&amp;Understrykning</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>&amp;Understrukna ord</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Genomstrykning</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>Vers&amp;aler</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapitäler</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>U&amp;pphöjt</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Ne&amp;dsänkt</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">�amp;versikt</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>S&amp;kugga</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>B&amp;ildeffekter</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorer...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicera</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Duplicera flera gånger</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppera</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Lås &amp;upp grupp</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Är &amp;låst</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Storle&amp;ken är låst</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Lä&amp;gg underst</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Lyf&amp;t till överst</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>F&amp;lytta ner en nivå</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Flytta &amp;upp en nivå</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Skicka till &amp;klippbok</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>Egensk&amp;aper...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>B&amp;ild synlig</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Uppdatera bild</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Justera ramen till bild</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Utökade bildegenskaper</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Låg upplösning</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normal upplösning</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Full upplösning</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Är ett PDF-&amp;bokmärke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Är en PDF-a&amp;nteckning</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Egenskaper för anteckning</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Fält e&amp;genskaper</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>R&amp;edigera form...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>S&amp;ammanfoga text med kurva</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Lös&amp;gör text från kurva</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Sammanf&amp;oga polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Dela upp &amp;polygoner</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezierkurva</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>B&amp;ildram</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Konturer</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polygon</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textram</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Tecken...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Exempeltext</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Infoga...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;portera...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Ta bort...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopiera...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Flytta...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Använd mallsidor...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Hantera stö&amp;dlinjer...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Hantera sidegenskaper...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="obsolete">Anpassa till &amp;fönster</translation>
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
        <translation>Minia&amp;tyrer</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Visa &amp;marginaler</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Visa &amp;ramar</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Visa b&amp;ilder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Visa stö&amp;drutor</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Visa stö&amp;dlinjer</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Visa &amp;baslinjerna</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Visa &amp;textflöde</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Visa kontrolltecken</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Linjaler relativt sida</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Lås till rutmönster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Lås till stödlin&amp;jerna</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>Egenska&amp;per</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Klippbok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Lager</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>Ordn&amp;a sidor</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmärken</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Måttangivelser</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Historik över händelser</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Verifiering (Pre-flight)</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>Juster&amp;a och fördela</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Verk&amp;tyg</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF verktyg</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Välj objekt</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abell</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Frihandslinje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotera objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Förstora eller förminska</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Förstora</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Förminska</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Redigera raminnehåll</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Redigera text...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Länka textramar</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Avbryt länk mellan textramar</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>Droppv&amp;erktyg</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopiera objektegenskaper</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Redigera text med Texthanteraren</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Infoga textram</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Infoga bildram</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Infoga tabell</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Infoga form</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Infoga polygon</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Infoga linje</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Infoga Bezierkurva</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Infoga frihandslinje</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Bild&amp;hantering</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>A&amp;vstava text</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Ångra avstavning</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>Skapa inne&amp;hållsförteckning</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Om &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Verktygsti&amp;ps</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;handbok...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Smart avstavnin&amp;g</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Hårt bindestreck</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Fast mellan&amp;slag</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Sid&amp;nummer</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Ny rad</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Bryt ram</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Kolumnbryt</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registrerat varumärke</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Varumärke</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Bomber</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Långt tankstreck (helfyrkant)</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Kort tankstreck (halvfyrkant)</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Citattecken</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostrof</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Dubbel divis</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Enkelt vänster</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Enklet höger</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Dubbelt vänster</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Dubbelt höger</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Enkelt omvänt</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Dubbelt omvänt</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Enkelt vänster gåsöga</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Enkelt höger gåsöga</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Dubbelt vänster gåsöga</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Dubbelt höger gåsöga</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Enkelt komma</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Dubbelt komma</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>CJK Enkelt vänster</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>CJK Enkelt höger</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>CJL Dubbelt vänster</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>CJK Dubbelt höger</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Växla paletter</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Växla hjälplinjer</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Skriv ut förhands&amp;visning</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Omvandla till mallsidor...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskad</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Sida vid sida</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>Om insticksprogr&amp;am</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Mer info...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Utskrift aktiverad</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>&amp;Vänd horisontellt</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>&amp;Vänd vertikalt</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Visa linjaler</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>Öve&amp;rsikt</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Snedstreck</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Mager punkt</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Halv fyrkant</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Hel fyrkant</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Smalt mellanslag</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Tjockt mellanslag</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Normalt mellanslag</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Hårtunnt mellanslag</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Infoga smart avstavning</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Infoga icke-brytande bindestreck</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Infoga icke-brytande mellanslag</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Infoga sidnummer</translation>
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
        <translation>S&amp;tilmallar...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>K&amp;ontur</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Konturer</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Klistra in (&amp;Absolut)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
    <message>
        <source>Insert PDF Push Button</source>
        <translation>Infoga PDF tryckknapp</translation>
    </message>
    <message>
        <source>Insert PDF Text Field</source>
        <translation>Infoga PDF textfält</translation>
    </message>
    <message>
        <source>Insert PDF Check Box</source>
        <translation>Infoga PDF kryssruta</translation>
    </message>
    <message>
        <source>Insert PDF Combo Box</source>
        <translation>Infoga PDF kombobox</translation>
    </message>
    <message>
        <source>Insert PDF List Box</source>
        <translation>Infoga PDF listbox</translation>
    </message>
    <message>
        <source>Insert Text Annotation</source>
        <translation>Infoga Textmarkering</translation>
    </message>
    <message>
        <source>Insert Link Annotation</source>
        <translation>Infoga länkmarkering</translation>
    </message>
    <message>
        <source>Save as &amp;EPS...</source>
        <translation>Spara som &amp;EPS...</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Visa kolumner i textram</translation>
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
    <name>AlignDistributeBase</name>
    <message>
        <source>Align and Distribute</source>
        <translation type="unfinished">Justera och fördela</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Justera</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished">Vald hjälp&amp;linje:</translation>
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
        <translation type="unfinished">Fördela</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation type="unfinished">Avstån&amp;d:</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Justera och fördela</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Justera</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativt:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Först valda</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Sist valda</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Sida</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marginaler</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hjälplinje</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Urval</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="obsolete">Justera höger kanter av objekt till vänster kant på fästpunkten</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="obsolete">Justera vänster kanter av objekt till höger kant på fästpunkten</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Justera nederkanter</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Justera höger kanter</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="obsolete">Justera överkant på objekt till nederkant på fästpunkt</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Centrera runt vertikal axel</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Justera vänsterkanter</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Centrera runt horisontell axel</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="obsolete">Justera nederkant på objekt till överkant på fästpunkt</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Justera överkanter</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>Vald hjälp&amp;linje:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="obsolete">Fördela</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="obsolete">Gör det horisontella avståndet mellan objekten lika</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="obsolete">Gör det horisontella avståndet mellan objekten lika med angivet värde</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Fördela höger kanter på lika avstånd</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Fördela nederkanterna på lika avstånd</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Fördela mittpunkterna på lika avstånd horisontellt</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation type="obsolete">Gör det vertikala avståndet mellan objekten lika</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="obsolete">Gör det vertikala avståndet mellan objekten lika med angivet värde</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Fördela vänster kanter på lika avstånd</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Fördela mittpunklterna på lika avstånd vertikalt</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Fördela överkanterna på lika avstånd</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>Avstån&amp;d:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Fördela objekten med angivet avstånd</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Inget valt</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
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
        <translation type="obsolete">Verktygstips:</translation>
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
        <translation type="obsolete">Exportera inte värdet</translation>
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
        <translation>Återställ</translation>
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
        <translation>Ändra...</translation>
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
        <translation>Åtgärd</translation>
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
        <translation>Öppna</translation>
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
        <translation>Ändring av markering</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alla filer (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Bilder (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Alla filer(*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Inget</translation>
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
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *epsi);;All Files (*)</source>
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
        <translation>Öppna</translation>
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
        <translation>Ä&amp;ndra...</translation>
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
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Använd mallsida</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Mallsida:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Använd på</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Aktuell &amp;sida</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Jämna sid&amp;or</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>U&amp;dda sidor</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alla sidor</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>Ino&amp;m givet område</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>to</source>
        <translation>till</translation>
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
        <translation>Använd den valda mallsidan jämna, udda eller alla sidor inom följande område</translation>
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
        <translation>Inget</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>Strec&amp;kkodsgenerator...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribus framsida för Pure Postscript Streckkodskrivare</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Fel vid öppnande av fil: %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 eller 13 siffror</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 siffror</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 eller 12 siffror</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 eller 8 siffror</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 siffror</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 siffror</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 eller 10 siffror korrekt separerade med bindestreck</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Varierande antal tecken, siffror och någon av symbolerna -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Variabelt antal ASCII-tecken och särskilda funktionssymboler börjande med lämpligt starttecken från den inledande teckenuppsättningen. UCC/EAN-128s måste ha en obligatorisk FNC1 symbol omedelbart efter starttecknet.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Variabelt antal siffror eller någon av symbolerna -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Variabelt antal siffror. En ITF-14 har 14 tecken och saknar kontrollsiffra</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Variabelt antal siffror</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Variabelt antal siffror eller versaler</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Variabelt antal hexadecimala tecken</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Streckkoden är ofullständig</translation>
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
        <translation>Skapare av streckkoden</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Streckkod</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Välj en av de tillgängliga streckkodstyperna här</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Den numeriska representationen av koden själv. Se hjälpmeddelandet nedan</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Återställ exemplen på streckkod</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Inkludera text i streckkoden</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Om kontrollerad så kommer det att finnas siffror även i streckkoden</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>Bevaka &amp;mellanrummen</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Rita pilar för att se till att det finns utrymme vid sidan av koden</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Färger</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Bakgrund</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Bakgrundsfärg - under kodraderna</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Linjer</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Färger på raderna i streckkoden</translation>
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
        <translation>Färg på text och siffror</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Förslag och hjälp visas här</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Förhandsvisa resultatet. 72dpi provexempel.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Ko&amp;d:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>I&amp;nkludera kontrollsumma</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Skapa och inkludera kontrollsumma i streckkoden</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Inkl&amp;udera siffra från kontrollsumma</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Inkludera siffra från kontrollsumma i streckkoden</translation>
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
        <translation type="obsolete">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="obsolete">&amp;Ladda...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Spara</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Spara &amp;som...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Liten</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">&amp;Medium</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="obsolete">S&amp;tor</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="obsolete">För&amp;handsvisning</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Klippbok (*.scs);;Alla filer (*)</translation>
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
        <translation>Namnet &quot;%1&quot; är inte unikt
Välj ett nytt.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Välj en katalog för klippboken</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Välj en katalog</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Klippbok (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Välj en klippboksfil att importera</translation>
    </message>
    <message>
        <source>Create a new scrapbook page</source>
        <translation>Skapa en ny klippbokssida</translation>
    </message>
    <message>
        <source>Load an existing scrapbook</source>
        <translation>Läs in en befintlig klippbok</translation>
    </message>
    <message>
        <source>Save the selected scrapbook</source>
        <translation>Spara den valda klippboken</translation>
    </message>
    <message>
        <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
        <translation>Importera en klippboksfil från Scribus &lt;=1.3.2</translation>
    </message>
    <message>
        <source>Close the selected scrapbook</source>
        <translation>Stäng den valda klippboken</translation>
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
        <translation>Återställ</translation>
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
        <translation type="obsolete">&amp;Fyllda färger:</translation>
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
        <translation>Återgivningsavsikt</translation>
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
        <translation type="obsolete">M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="obsolete">S&amp;krivare:</translation>
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
        <translation type="obsolete">Standard färgprofil för tonplattor på sidan</translation>
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
        <translation type="obsolete">Standardvärde för återgivningen på din monitor. Om du inte vet varför du bör ändra
bör du välja Relative Colorimetric eller Perceptual.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="obsolete">Standardvärde för återgivningen på din skrivare. Om du inte vet varför du bör ändra
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
        <translation>&amp;RGB-bilder:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK-bilder:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Standard färgprofil för importerade CMYK-bilder</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Standard färgprofil för importerade RGB-bilder</translation>
    </message>
    <message>
        <source>&amp;RGB Solid Colors:</source>
        <translation>&amp;RGB Tonplattor:</translation>
    </message>
    <message>
        <source>&amp;CMYK Solid Colors:</source>
        <translation>&amp;CMYK Tonplattor:</translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Sol&amp;id Colors:</source>
        <translation>Tonp&amp;lattor:</translation>
    </message>
    <message>
        <source>Convert all colors to printer space</source>
        <translation>Konvertera alla färger till skrivarens färgrymd</translation>
    </message>
    <message>
        <source>Default color profile for solid RGB colors on the page</source>
        <translation>Standard färgprofil för RGB tonplattor på sidan</translation>
    </message>
    <message>
        <source>Default color profile for solid CMYK colors on the page</source>
        <translation>Standard färgprofil för CMYK-tonplattor på sidan</translation>
    </message>
    <message>
        <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standardrendering avsedd för tonplattor -- såvida du inte vet varför det ska ändras.
Relativ colormetri eller perceptuell bör användas.</translation>
    </message>
    <message>
        <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standardrendering avsedd för bilder -- såvida du inte vet varför det ska ändras.
Relativ colormetri eller perceptuell bör användas.</translation>
    </message>
    <message>
        <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
        <translation>Simulerar full färghanteringsmiljö:
Alla färger, rgb eller cmyk, konverteras till skrivarens färgrymd.</translation>
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
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Du kan inte skapa en färg med namnet &quot;%1&quot;.
Namnet är reserverat för en transparent färg</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Är en tonplatta</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Är en färgseparation</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Du kan inte skapa en färg med namnet &quot;%1&quot;.
Det är ett reserverat namn för genomskinlig färg</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="obsolete">Färgens namn är inte unikt</translation>
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
        <translation type="obsolete">Normalsyn</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Protanopia (Red)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Deuteranopia (Green)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Tritanopia (Blue)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Full färgblindhet</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished">Sammanfoga färger</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished">Fel: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="unfinished">Färgen %1 finns redan!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="unfinished">Färgen %1 tillagd.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished">Nu öpnnas färghanteraren.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished">Färger sammanfogas</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished">Kan inte hitta den begärda färgen. Du har antagligen valt svart, grå eller vit. Det finns inget sätt att hantera denna färg.</translation>
    </message>
</context>
<context>
    <name>CWDialogBase</name>
    <message>
        <source>Color Wheel</source>
        <translation type="unfinished">Färghjul</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMGS</translation>
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
        <source>RGB:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
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
        <source>CMYK:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished">Välj en av metoderna för att skapa ett färgschema. Mer information finns i dokumentationen.</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Vinkel:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished">Skillnad upptäckt mellan valt värde och de beräknade. Mer information finns i dokumentationen.</translation>
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
        <translation type="unfinished">Sammanfoga skapade färger med dokumentets färger</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">E&amp;rsätt</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished">Alt+R</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished">Byt ut skapade färger i dokumentets färger</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="unfinished">Lämna färger orörda</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished">Simulera vanliga synfel här. Välj typ av synfel.</translation>
    </message>
    <message>
        <source>Color Scheme Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HSV:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="unfinished">HSV</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished">H:</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished">S:</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished">V:</translation>
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
        <translation type="obsolete">Du kan se en miniatyrbild om du trycker ner och håller den högra musknappen nere. Kommandot Infoga infogar ett smalt avstånd i urvalet nedan, och kommandot Ta bort tar bort den senast infogade</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Välj tecken:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Teckensnitt:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Teckenklass:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Infoga</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Infoga tecken vid markören i texten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Ta bort aktuell(a) markering(ar).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Stäng denna dialogruta och återvänd till textredigering.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Full teckenuppsättning</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Grundläggande Latin</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 tillägg</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin Utökad-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latin Utökad-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Allmän interpunktion</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Upp- eller nedsänkt</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Valutasymboler</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Bokstavsliknande symboler</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Nummerformer</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Pilar</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematiska operatorer</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Rita rutor</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Större beståndsdel</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometriska former</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Blandade symboler</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Teckensymboler (dingbats)</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Små storlekar</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Special</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grekiska</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Grekiska (utökad)</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Kyrillisk</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Kyrillisk (tillägg)</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabiska</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabiska Utökad A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabiska Utökad B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebreiska</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="obsolete">&amp;Infoga kod:</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation type="obsolete">Stäng dialogrutan och återvänd till textredigering</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="obsolete">Skriv in ett fyrsiffrigt unicode-värde här</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished">Du kan se en miniatyrbild om du trycker ner och håller den högra musknappen nere. Kommandot Infoga infogar ett smalt avstånd i urvalet nedan, och kommandot Ta bort tar bort den senast infogade</translation>
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
        <translation type="unfinished">Fel</translation>
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
</context>
<context>
    <name>CharTable</name>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Ta bort</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation>Aktuell profil:</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problem</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Saknade tecken</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Text överflödar</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objektet finns inte på någon sida</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Bild saknas</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objekt har genomskinlighet</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objektet är en monterad PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Inga problem funna</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Sida </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Lösgör objekt</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Problem hittade</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Förhandskontroll</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objektet är en PDF-notering eller -fält</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorera fel</translation>
    </message>
    <message>
        <source>Check again</source>
        <translation>Kontrolera igen</translation>
    </message>
    <message>
        <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
        <translation>Bildens upplösning är mindre än %1 DPI, aktuellt värde är %2 x %3 DPI</translation>
    </message>
    <message>
        <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
        <translation>Bildens upplösning är större än %1 DPI, aktuellt värde är %2 x %3 DPI</translation>
    </message>
    <message>
        <source>Image is GIF</source>
        <translation>Bilden är en GIF</translation>
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
        <translation>Välj stilmall</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Tillgängliga stilmallar</translation>
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
        <translation>Välj en katalog</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Samlar ihop...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Kan inte samla ihop alla filer för filen:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Kan inte samla ihop filen:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Färger</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importera</translation>
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
        <source>Choose a color set to load</source>
        <translation>Välj färggrupp att ladda</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Spara aktuell färggrupp</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Ta bort oanvända färger från aktuellt dokuments färggrupp</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importera färger till aktuell färggrupp från ett existerande dokument</translation>
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
        <translation>Välj ett namn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öppna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.scd);;Alla filer (*)</translation>
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
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Other Files (*eps *.epsi *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monokromatisk</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Motsvarande</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementär</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Dela komplementär</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadisk</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetraidisk (dubbelt komplementärt)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Basfärg</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Ljust monokromatiskt</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Mörkt monomkromatiskt</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>Första motsvarande</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Andra motsvarande</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>Första delade</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>Andra delade</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>Tredje delade</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>Fjärde delade</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>Första triaden</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>Andra triaden</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>Första tetraeden (motsatt bas)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>Andra tetraeden (vinkel)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Tredje tetraeden (motsatt vinkel)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Normal Vision</source>
        <translation type="obsolete">Normalsyn</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Full färgblindhet</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation type="obsolete">Synfel:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation type="obsolete">Färghjul</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Färg</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Namn</translation>
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
        <translation type="obsolete">Y</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">K</translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation type="obsolete">Välj metod:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation type="obsolete">Vinkel (0 - 90 grader):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation type="obsolete">Sa&amp;mmanfoga färger</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation type="obsolete">Byt ut fä&amp;rger</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation type="obsolete">Sammanfoga skapade färger med dokumentets färger</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="obsolete">Byt ut skapade färger i dokumentets färger</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="obsolete">Lämna färger orörda</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="obsolete">Sammanfoga färger</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="obsolete">Fel: </translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="obsolete">Nu öpnnas färghanteraren.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="obsolete">Färger sammanfogas</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation type="obsolete">Skapa fär&amp;g...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation type="obsolete">&amp;Importera existerande färg...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation type="obsolete">Sa&amp;mmanfoga färger</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation type="obsolete">Byt ut fä&amp;rger</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="obsolete">A&amp;vsluta</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">Fä&amp;rg</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="obsolete">Skillnad upptäckt mellan valt värde och de beräknade. Mer information finns i dokumentationen.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="obsolete">Klicka på hjulet för att få basfärgen. Tonen ges i HSV-värde.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation type="obsolete">Exempel på färgschema</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="obsolete">Välj en av metoderna för att skapa ett färgschema. Mer information finns i dokumentationen.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation type="obsolete">Färger från det valda färgschemat</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="obsolete">Simulera vanliga synfel här. Välj typ av synfel.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Ny färg</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="obsolete">Kan inte hitta den begärda färgen. Du har antagligen valt svart, grå eller vit. Det finns inget sätt att hantera denna färg.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="obsolete">Färgk&amp;omponenter...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Protanopia (Red)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Deuteranopia (Green)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Tritanopia (Blue)</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="obsolete">Färgen %1 finns redan!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="obsolete">Färgen %1 tillagd.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>Färg&amp;hjul...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Hjälpreda för färginställning</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Färgvälhjare med inkluderad färgteori.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvänd</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varning</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Enkelsidig</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Dubbelsidig</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3-vik</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4-vik</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Vänster sida</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">Mellan</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">Mellan vänster</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Mellan höger</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Höger sida</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Måndag</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Tisdag</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>Onsdag</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>Torsdag</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>Fredag</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>Lördag</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Söndag</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Januari</translation>
    </message>
    <message>
        <source>February</source>
        <translation>Februari</translation>
    </message>
    <message>
        <source>March</source>
        <translation>Mars</translation>
    </message>
    <message>
        <source>April</source>
        <translation>April</translation>
    </message>
    <message>
        <source>May</source>
        <translation>Maj</translation>
    </message>
    <message>
        <source>June</source>
        <translation>Juni</translation>
    </message>
    <message>
        <source>July</source>
        <translation>Juli</translation>
    </message>
    <message>
        <source>August</source>
        <translation>Augusti</translation>
    </message>
    <message>
        <source>September</source>
        <translation>September</translation>
    </message>
    <message>
        <source>October</source>
        <translation>Oktober</translation>
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
        <translation type="unfinished">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Nej</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Left Page</source>
        <comment>Left page location</comment>
        <translation type="unfinished">Vänster sida</translation>
    </message>
    <message>
        <source>Middle</source>
        <comment>Middle page location</comment>
        <translation type="unfinished">Mellan</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <comment>Middle Left page location</comment>
        <translation type="unfinished">Mellan vänster</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <comment>Middle Right page location</comment>
        <translation type="unfinished">Mellan höger</translation>
    </message>
    <message>
        <source>Right Page</source>
        <comment>Right page location</comment>
        <translation type="unfinished">Höger sida</translation>
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
        <translation type="unfinished">Normalsyn</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <comment>Color Blindness - Red Color Blind</comment>
        <translation type="unfinished">Protanopia (Red)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <comment>Color Blindness - Greed Color Blind</comment>
        <translation type="unfinished">Deuteranopia (Green)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <comment>Color Blindness - Blue Color Blind</comment>
        <translation type="unfinished">Tritanopia (Blue)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <comment>Color Blindness - Full Color Blindness</comment>
        <translation type="unfinished">Full färgblindhet</translation>
    </message>
    <message>
        <source>Custom: </source>
        <comment>Custom Tab Fill Option</comment>
        <translation type="unfinished">Anpassad: </translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation>Flytta vektor</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Flytta början på toningsvektorn med vänster musknapp nedtryckt och flytta slutet på toningsvektorn med höger musknapp nedtryckt</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Blandningsläge:</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Mörkare</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Ljusare</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Flerfaldiga</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Raster</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Överlägg</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Hårt ljus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Mjukt ljus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Skillnad</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Uteslutning</translation>
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
        <source>Hue</source>
        <translation type="unfinished">Färgton</translation>
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
        <translation type="unfinished">Luminans</translation>
    </message>
    <message>
        <source>Offsets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="unfinished">X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Uteslutning</translation>
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
        <translation type="unfinished">Till:</translation>
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
        <translation type="unfinished">T&amp;a bort</translation>
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
        <translation type="unfinished">&amp;Avbryt</translation>
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
        <translation type="obsolete">Inget</translation>
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
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Inget</translation>
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
        <translation type="obsolete">Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
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
        <translation type="obsolete">Denna dialogruta visar olika utskriftsalternativ för CUPS.(new line)
De exakta parametrar som är tillgängliga beror på din skrivardrivrutin.(new line)
Du kan kontrollera stöd för CUPS genom att välja Hjälp &gt; Om.(new line)
Kontrollera koden C-C-T. Dessa motsvarar stöd för C=CUPS, C=littlecms och T=Tiff.(new line)
Eventuellt saknat stöd markeras med *.</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Denna panel visar olika CUPS-alternativ vid utskrift. De tillgängliga parametrarna bestämms av skrivarens drivrutin. Du kan bekräfta stöd för CUPS genom att välja Hjälp&gt;Om. Sök efter: C-C-T vilket översätts till C=CUPS, C=littlecms och T=TIFF-stöd. Saknas ett bibliotek markeras det med *</translation>
    </message>
</context>
<context>
    <name>CurveWidget</name>
    <message>
        <source>Open</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Curve Files (*.scu);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Spara som</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished">Kan inte skriva till filen:
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
    <message>
        <source>&amp;Include ICC Profiles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation type="obsolete">Ange färgkomponenter</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="obsolete">CMGS</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="obsolete">RGB</translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="obsolete">HSV</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="obsolete">H:</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="obsolete">S:</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="obsolete">V:</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="obsolete">R:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="obsolete">G:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="obsolete">B:</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="obsolete">C:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="obsolete">M:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="obsolete">Y:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="obsolete">S:</translation>
    </message>
    <message>
        <source>Set &amp;RGB</source>
        <translation type="obsolete">Ange &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="obsolete">Ange C&amp;MGS</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="obsolete">Ange &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Avbruten av användaren</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation>Ta bort färg:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ersätt med:</translation>
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
        <translation>Ta bort från:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Ta bort stilmall</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Ta bort stilmall:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ersätt med:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stilmall</translation>
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
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Importen misslyckades</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Import av word-dokument misslyckades
%1</translation>
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
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Person eller organisation primärt ansvarig för skapandet av innehållet. Detta fält kan antingen bäddas in i Scribus-dokumentet som referens eller som metadata i en PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Dokumentets namn. Fältet kan antingen bäddas in i Scribus-dokumentet eller som metadata i en PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>En sammanfattning av dokumentinnehållet.Fältet är en kort beskrivning eller sammanfattning av dokumentet och bäddas in i PDF vid export</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Ämnet för dokumentinnehållet. Detta fält innehåller nyckelord för dokumentinnehållet som du vill bädda in i PDF för att förenkla sökning och indexering av PDF-filer</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Den fysiska eller digitala beskrivningen av dokumentet. Mediatyp och dimensioner bör anges. RFC2045, RFC2046 för MIME-typer är också bra att ange</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Det språk i vilket dokumentet är skrivet, vanligtvis en ISO-639 kod med (möjligen) ett tillägg iform av bindestreck följt av en landskod (ISO-3166), t. ex. en-GB, sv-SE</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Lägg till en sidnumrerat avsnitt till dokumentet. Det nya avsnittet kommer att läggas till efter det markerade avsnittet.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Ta bort det markerade avsnittet.</translation>
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
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Namn:&lt;/b&gt;Valfritt namn för avsnitt, t. ex. Index&lt;br /&gt;&lt;b&gt;Visat:&lt;/b&gt;Välj för att visa sidnummer i etta avsnitt om det finns en eller flera textramar för detta ändamål.&lt;br /&gt;&lt;b&gt;Från:&lt;/b&gt;Det sidindex som detta avsnitt börjar på.&lt;br /&gt;&lt;b&gt;Till:&lt;/b&gt;Det sidindex som detta avsnitt ska sluta på.&lt;br /&gt;&lt;b&gt;Stil:&lt;/b&gt;Välj den stil som ska användas.&lt;br /&gt;&lt;b&gt;Start:&lt;/b&gt;Anger vilken position i stilen som skall inleda, t. ex. om Start=2 och Stil=a, b, c, ... så inleds indexeringen på b. I det första avsnittet i dokumentet kommer detta att ersätta sidnumret på förstasidan i den nya filen.</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Sidnumret utanför gränserna</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Det värde du har angett är utanför gränserna för det aktuella dokumentet (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokumentavsnitt</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Från</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Till</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Visad</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Hänger samman med</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Är förälder till</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Är barn till</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Textramar</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Bildramar</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Logisk (Boolesk)</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Heltal</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Sträng</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Värde</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Ra&amp;dera</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Reella tal</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Attribut för dokumentenhet</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Värde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Samband</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Samband med</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Lägg till automatiskt till</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiera</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
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
        <translation>Ä&amp;ndra...</translation>
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
        <translation>Skriv ut normalt</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Skriv ut separationer</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Skriv ut i färg om tillgängligt</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Skriv ut i gråskala</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript Nivå 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript Nivå 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript Nivå 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Sida</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Spegelvänd sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Spegelvänd sida(or) vertikalt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Ange storlek på media</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Färg</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Använd underfärgsborttagning</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Omvandla tonplattor till processfärger</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Använd ICC-profiler</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Avancerade alternativ</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Förhandsgranska...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Sätter PostScript-nivå.
Att sätta till Nivå 1 eller 2 kan skapa mycket stora filer</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript-filer (*.ps);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Använd en annan utskriftshanterare, t. ex. kprinter eller gtklp, för att erhålla ytterligare utskriftsalternativ</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Ett sätt att stänga av några av de gråtoner som består av cyan, gult och magenta för att istället använda svart. Underfärgsborttagning påverkar huvudsakligen delar av bilder som är neutrala och/eller mörka toner som är nära grått. Användning av detta kan förbättra utskriften av vissa bilder och kräver experimenterande med testutskrifter från fall till fall. Underfärgsborttagning minskar risken för övermättnad av CMG-färger.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Tillåter att tonplattor konverteras till sammansatta färger. Om du inte tänker skriva ut färger som tonplattor på ett tryckeri bör du helst lämna denna aktiverad.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Gör det möjligt att bädda in ICC-profiler i utskriftsströmmen när färghantering är aktiverad</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Möjliggör för dig att uttryckligen ange mediastorlek för PostScript-filen. Bör ej användas om det inte uttryckligen krävs av tryckeriet.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Beskär till sidmarginalerna</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Visa inte objekt utanför marginalerna vid utskrift eller export av filen</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Misslyckades att hitta skrivarinställningarna</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Visa inte objekt utanför marginalerna för den tryckbara sidan</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Påtvingad övertryckning</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Tillåter övertryckning av hela dokumentet oavsett inställningar för objekt</translation>
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
        <translation type="unfinished">Höger:</translation>
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
    <message>
        <source>Error</source>
        <translation>Fel</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation type="obsolete">Importerar PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Analyserar PostScript:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Skapar objekt</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Konvertering av %1 bilder misslyckades!</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Avstånd</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast radavstånd</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatiskt radavstånd</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Justera mot baslinjemönstret</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Anfanger</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Avstånd till text:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Förhandsvisning av stilmallar för stycke</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Beräknar avståndet mellan anfang och texten</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Växla mellan exempeltexter med styckets stilmall</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Stilmallens namn är inte unikt</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrund</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="obsolete">Välj denna för ökad läsbarhet vid ljust färgade tecken</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuell spärrning</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Avvikelse från tecknens baslinje</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Klicka för val av radavstånd</translation>
    </message>
    <message>
        <source>Select for easier reading of light colored text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished">Automatiskt</translation>
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
        <translation>Ån&amp;gra</translation>
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
        <translation>JavaScripts (*.js);;Alla filer (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Bildeffekter</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Alternativ:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Färg:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Nyans:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Ljushet:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Radie:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Värde:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Tillgängiga effekter</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Smeta ut</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Ljushet</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Färga</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertera</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Öka skärpan</translation>
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
        <translation>Effekter som används</translation>
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
        <translation type="unfinished">Filen existerar redan. Vill du skriva över?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished">existerar redan. Villdu skriva över?</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Allt</translation>
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
        <translation>Ä&amp;ndra...</translation>
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
        <translation>Ändra utmatningskatalog</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Bildstorlekar. 100% för ursprungsstorlek, 200% för dubbla storleken etc.</translation>
    </message>
    <message>
        <source>Image size in Pixels</source>
        <translation>Bildstorlek i pixlar</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Utökade bildegenskaper</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Mörkare</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Ljusare</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Färgton</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mättnad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Färg</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Luminans</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Flerfaldiga</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Raster</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Upplösa</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Överlägg</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårt ljus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Mjukt ljus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Skillnad</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Uteslutning</translation>
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
        <translation type="obsolete">Uteslutning</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandningsläge:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Täckning:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Bakgrund</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lager</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Använd ingen kurvlinje</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Sökvägar</translation>
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
        <translation>Upplösning:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
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
        <source>Colorspace:</source>
        <translation>Färgrymd:</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Färger</translation>
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
        <source>&amp;Edit</source>
        <translation type="obsolete">R&amp;edigera</translation>
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
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">&amp;Ta bort oanvända</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Färggrupper</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Aktuell färggrupp:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Spara färggrupp</translation>
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
        <translation type="obsolete">Välj färggrupp att ladda</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Spara aktuell färggrupp</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Ta bort oanvända färger fån aktuellt dokuments färggrupp</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Lägg till färger till aktuell färggrupp från ett existerande dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Skapa en ny färg i färggruppen</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Redigera vald färg</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Gör en kopia av vald färg</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Ta bort vald färg</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Gör aktuell färggrupp till standardfärggrupp</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Namn:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Välj namn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopia av %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Ny färg</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importera</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Några teckensnitt som använts i detta dokument har ersatts:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> ersattes av: </translation>
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
        <translation>Ä&amp;ndra...</translation>
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
        <translation>Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Använd teckensnitt</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="obsolete">Inbäddad i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Delmängd</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Sökväg till teckensnittsfil</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="obsolete">Sökvägar för teckensnitt kan endast anges i Inställningar, och endast när inga dokument är öppna. Stäng alla öppna dokument, välj Redigera-&gt;Inställningar för att ändra sökväg till teckensnitt.</translation>
    </message>
    <message>
        <source>Embed in PostScript</source>
        <comment>font preview</comment>
        <translation>Inbakad i PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in File &gt; Preferences, and only when there is no document currently open. Close any open documents, then use File &gt; Preferences &gt; Fonts to change the font search path.</source>
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
        <translation type="obsolete">Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Delmängd</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Användare</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>System</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Lägg till det valda teckensnittet till menyerna Stil och Teckensnitt</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Avbryt Förhandsvisning</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">Sö&amp;k</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Påbörja sökning</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Storleken på valt teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Exemplet visas så snart tangenten släpps</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Skriv in text här för snabbsökning på teckensnittsnamn. Sökningen skiljer mellan stora och små bokstäver. Du kan använda de vanliga jokertecknen (*, ?, [...]) i sökfrasen.
Exempel: t* eller T* listar alla teckensnitt som börjar på t eller T. *bold* listar alla teckensnitt med bold  [fet], bolder [fetare] i namnet.</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>Snabbsö&amp;kning:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Sö&amp;k</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Teckensnittsnamn</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Delmängd</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Åtkomst</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Teckengra&amp;d:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Exempeltext att visa</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>S&amp;täll in</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Återställ text</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Stäng</translation>
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
        <translation>&amp;Förhandsvisning av teckensnitt...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Förhandsvisning av teckensnitt</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Sortering, sökning visning av tillgängliga teckensnitt.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Teckensnittsersättning</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Ursprungligt teckensnitt</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Ersättningsteckensnitt</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Gör ersättningarna bestående</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Detta dokument innehåller teckensnitt som inte finns installerade i ditt system. Välj en lämplig ersättning för dem. Avbryt gör att dokumentet inte läses in.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Avbryter teckensnittsersättningen och stoppar inläsningen av dokumentet.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Aktivering av dessa innebär att Scribus ersätter de saknade teckensnitten även i framtida layouter. Denna ersättning kan tas bort eller ändras i Redigera &gt; Inställningar &gt; Teckensnitt.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Om du väljer OK och sedan sparar kommer dessa ersättningar att bli bestående i dokumentet.</translation>
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
        <translation type="obsolete">Här kan du justera var färgerna börjar och slutar.
Du kan också lägga till ytterligare färger i toningen.</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Lägg till, ändra eller ta bort färgslut här</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation type="obsolete">Hantera stödlinjer</translation>
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
        <translation type="obsolete">Horisontella stödlinjer</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="obsolete">&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="obsolete">T&amp;a bort</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation type="obsolete">Vertikala stödlinjer</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="obsolete">&amp;X-Pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="obsolete">&amp;Lägg till...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="obsolete">T&amp;a bort</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="obsolete">&amp;Lås stödlinjer</translation>
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
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Sida</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Stäng</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Förhandsvisning</translation>
    </message>
    <message>
        <source>Edit Guide</source>
        <translation>Redigera hjälplinje</translation>
    </message>
    <message>
        <source>Enter a position:</source>
        <translation>Ange en position:</translation>
    </message>
    <message>
        <source>New Guide</source>
        <translation>Ny hjälplinje</translation>
    </message>
</context>
<context>
    <name>GuideManagerBase</name>
    <message>
        <source>Manage Guides</source>
        <translation>Hantera stödlinjer</translation>
    </message>
    <message>
        <source>&amp;Standard</source>
        <translation type="obsolete">&amp;Standard</translation>
    </message>
    <message>
        <source>Horizontals</source>
        <translation>Horisontella</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hjälplinje</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Enhet</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>T&amp;a bort</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Verticals</source>
        <translation>Vertikala</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>T&amp;a bort</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Lås hjälplinjer</translation>
    </message>
    <message>
        <source>Appl&amp;y to All Pages</source>
        <translation>Använ&amp;d på alla sidor</translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation>Alt+Y</translation>
    </message>
    <message>
        <source>A&amp;utomatic</source>
        <translation type="obsolete">A&amp;utomatisk</translation>
    </message>
    <message>
        <source>&amp;Number:</source>
        <translation>&amp;Nummer:</translation>
    </message>
    <message>
        <source>U&amp;se Gap:</source>
        <translation>Använd mellan&amp;rum:</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Nu&amp;mber:</source>
        <translation>Nu&amp;mmer:</translation>
    </message>
    <message>
        <source>Use &amp;Gap:</source>
        <translation>Använd mellan&amp;rum:</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Refer To</source>
        <translation>Hänvisar till</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Sida</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>M&amp;argins</source>
        <translation>M&amp;arginaler</translation>
    </message>
    <message>
        <source>S&amp;election</source>
        <translation>Urv&amp;al</translation>
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
    <message>
        <source>&amp;Single</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Column/Row</source>
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
        <translation>&amp;Innehåll</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Sök</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>&amp;Sök</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Ta b&amp;ort allt</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Bok&amp;märken</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Skriv ut...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>A&amp;vsluta</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Sökning är oberoende av stora eller små bokstäver</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Hitta</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Sökterm:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nytt bokmärke</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Titel på nytt bokmärke:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Hitta...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Hitta &amp;nästa</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Hitta &amp;föregående</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>Lägg till &amp;bokmärke</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>Ta b&amp;ort allt</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmärken</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Relevans</translation>
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
        <translation>Bildinformation</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Allmän information</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Datum/Tid:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Har inbäddad profil:</translation>
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
        <source>Profile Name:</source>
        <translation>Profilnamn:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Har inbäddade markeringslinjer:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Har lager:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF info</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Artist:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Kommentar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Användarkommentar:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kameramodell:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kameratillverkare:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivning:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright::</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Skannermodell:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Skannertillverkare:</translation>
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
        <translation>Importera &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importera EPS-filer</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importerar de flesta EPS-filer till aktuellt dokument,
och konverterar deras vektordata till Scribusobjekt.</translation>
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
        <translation type="obsolete">Normal</translation>
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
        <translation>&amp;Infoga</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Mallsidor</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Mallsida:</translation>
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
        <source>Move Objects with their Page</source>
        <translation>Flytta objekt tillsammans med sidan</translation>
    </message>
</context>
<context>
    <name>InsertAFrame</name>
    <message>
        <source>Open</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Textram</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished">B&amp;ildram</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished">T&amp;abell</translation>
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
        <translation type="unfinished">Polygon</translation>
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
        <translation type="unfinished">X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Storlek</translation>
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
        <translation type="unfinished">Höjd:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Bredd:</translation>
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
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Link Created Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="unfinished">Alla sidor</translation>
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
        <translation>Antal rader:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Antal kolumner:</translation>
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
        <translation type="obsolete">&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Vill du verkligen ta bort detta skript?</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Vill du verkligen ta bort det här skriptet?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Lägger till ett nytt skript och fördefinierar en funktion med samma namn. Om du vill använda detta skript i &quot;Open Action&quot; var noga med att inte ändra nmanet på funktionen.</translation>
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
        <translation type="obsolete">Aktivitet</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation type="obsolete">Aktuell tangent</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation type="obsolete">Välj tangent för denna aktivitet</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="obsolete">&amp;Ingen tangent</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="obsolete">&amp;Användardefinierad tangent</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="obsolete">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="obsolete">Välj &amp;tangent</translation>
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
        <translation type="obsolete">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="obsolete">Ctrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="obsolete">Shift</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="obsolete">Shift+</translation>
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
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Den här tangentkombination används redan</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="obsolete">Inläsningsbara uppsättningar av genvägar</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="obsolete">&amp;Läs in</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="obsolete">&amp;Importera...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="obsolete">&amp;Exportera...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="obsolete">Åte&amp;rställ</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="obsolete">Tangentbordsgenvägar är tillgängliga för inläsning</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="obsolete">Läs in den valda uppsättningen av genvägar</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="obsolete">Importera en uppsättning genvägar till aktuell konfiguration</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="obsolete">Exportera aktuella genvägar till en importerbar fil</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="obsolete">Ladda om Scribus standardgenvägar</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="obsolete">Key Set XML filer (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="obsolete">Den här knappsekvensen används redan</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="obsolete">Meta</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="obsolete">Meta+</translation>
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
        <translation>Namn</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Vill du även ta bort alla objekt i detta lager?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Lägg till ett nytt lager</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Ta bort lager</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Lyft upp lager</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Sänk ner lager</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Täckning:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandningsläge:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Mörkare</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Ljusare</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Flerfaldiga</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Raster</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Överlägg</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårt ljus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Mjukt ljus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Skillnad</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Uteslutning</translation>
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
        <source>Exclusion</source>
        <translation type="unfinished">Uteslutning</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Färgton</translation>
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
        <translation type="unfinished">Luminans</translation>
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
        <translation type="obsolete">&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öppna</translation>
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
        <translation>&amp;Importera</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Vill du verkligen ta bort den här stilen?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%</source>
        <translation></translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linjebredd:</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <source> pt</source>
        <translation>pt</translation>
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
        <source>Solid Line</source>
        <translation type="obsolete">Heldragen linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="obsolete">Streckad linje</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="obsolete">Punktlinje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="obsolete">Streck-punkt linje</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="obsolete">Streck-punkt-punkt linje</translation>
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
        <translation>Alla filer (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Välj Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Författare:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Hämta mer:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML fil:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Stycken:</translation>
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
        <translation>Hantera sidproportioner</translation>
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
        <source>Move Objects with their Page</source>
        <translation>Flytta objekt tillsammans med sidan</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marginallinjer</translation>
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
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>Övers&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Vä&amp;nster:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Avstånd mellan övre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Avstånd mellan nedre marginallinjen och papperskanten</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Insida:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Ytt&amp;ersida:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Förinställda layouter:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="obsolete">Använd marginalinställningar på alla sidor</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Använd marginaländringar på alla sidor i dokumentet</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Avståndet mellan vänster marginal och papprets kant. Om &quot;Uppslag&quot; valts kan marginalen användas för att skapa ett korrekt utrymme för bindning</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Avståndet mellan högermarginal och papprets kant. Om &quot;Uppslag&quot; valts kan marginalen användas för att skapa ett korrekt utrymme för bindning</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Skrivarmarginaler...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importera marginaler för vald sidstorlek från tillgängliga skrivare.</translation>
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
        <translation type="unfinished">Marginallinjer</translation>
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
        <translation type="unfinished">Höger:</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Redigera mallsidor</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Vill du verkligen ta bort mallsidan?</translation>
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
        <translation>&amp;Namn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Ny mallsida</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopia av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Namn:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Ny mallsida</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopia #%1 av</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Duplicera den valda mallsidan</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Ta bort den valda mallsidan</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Lägg till en ny mallsida</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Importera mallsida från ett annat dokument</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Ny mallsida %1</translation>
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
        <translation>pt</translation>
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
        <translation>Öppna</translation>
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
        <translation>&amp;Från dokument:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>Änd&amp;ra...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importera sida/sidor:</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Före sida</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Efter sida</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>I slutet</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importera</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Importera mallsida</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importera mallsida</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Infoga en kommaseparerad lista av tokens där en token kan vara * för alla sidor, 1-5 för ett sidavsnitt eller ett enskilt sidnummer.</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Saknat teckensnitt</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Teckensnitt %1 är inte installerat.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Använd</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>istället</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
</context>
<context>
    <name>ModeToolBar</name>
    <message>
        <source>Tools</source>
        <translation>Verktyg</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaper...</translation>
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
        <translation>Flytta sida/sidor</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Före sidan</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Efter sidan</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>I slutet</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Till:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Antal kopior:</translation>
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
        <translation type="obsolete">&amp;Mellanrum:</translation>
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
        <translation type="obsolete">&amp;Flöda text runt ramen</translation>
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
        <translation type="obsolete">S&amp;pråk:</translation>
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
        <translation>Återgivningsavsikt:</translation>
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
        <translation type="obsolete">Låt texter i ramar på lägre nivåer flöda runt objektets form</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Använd en avgränsande ram istället för objektets form för textflöde</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Använd en extra linje baserat på objektets form för textflöde</translation>
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
        <translation type="obsolete">Avstavningsspråk för ramen</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Ändra inställningar för vänsterpunkt eller slutpunkt</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation>Startpil:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Avslutspil:</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast radavstånd</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatiskt radavstånd</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Justera mot baslinjemönstret</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Faktisk X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Faktisk Y-DPI:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Avvikelse från tecknens baslinje</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Storleksförändring av teckenhöjd</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuell spärrning</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Namnet &quot;%1&quot; är inte unikt. &lt;br /&gt;Välj ett annat.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Fyllningsregel</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Jämn-Udda</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation></translation>
    </message>
    <message>
        <source>Overprinting</source>
        <translation>Övertryckning</translation>
    </message>
    <message>
        <source>Knockout</source>
        <translation>Utslagning</translation>
    </message>
    <message>
        <source>Overprint</source>
        <translation>Övertryck</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Färg på textkontur och/eller textskugga, beroende på vad som valts. Om båda valts så får de samma färg.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Färg på vald text. Om Kontur valts som teckendekor blir detta fyllnadsfärgen. Om Textskugga har valts blir detta den översta färgen.</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation type="obsolete">Klicka för val av radavstånd</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Bredd:</translation>
    </message>
    <message>
        <source>Text &amp;Flow Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Inaktiverad</translation>
    </message>
    <message>
        <source>Use Frame &amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished">Bildeffekter</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished">Utökade bildegenskaper</translation>
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
        <translation type="unfinished">&amp;Gruppera</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Täckning:</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Blandningsläge:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">Mörkare</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Ljusare</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Flerfaldiga</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Raster</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Överlägg</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Hårt ljus</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Mjukt ljus</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Skillnad</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Uteslutning</translation>
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
        <source>Hue</source>
        <translation type="unfinished">Färgton</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
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
        <translation type="unfinished">Automatiskt</translation>
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
        <translation>Namnet &quot;%1&quot; är inte unikt. &lt;br /&gt;Välj ett annat.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Förlopp</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Förloppet i sin helhet:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>MultipleDuplicate</name>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">&amp;Horisontell förflyttning:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">&amp;Vertikal förflyttning:</translation>
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
        <translation type="unfinished">Duplicera flera gånger</translation>
    </message>
    <message>
        <source>&amp;By Number of Copies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished">A&amp;ntal kopior:</translation>
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
        <translation type="unfinished">&amp;Horisontell förflyttning:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">&amp;Vertikal förflyttning:</translation>
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
        <translation type="unfinished">Antal rader:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="unfinished">Antal kolumner:</translation>
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
        <translation>Min &amp;plugin</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Min plugin</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Plugin fungerade!</translation>
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
        <translation type="obsolete">Sidstorlek</translation>
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
        <translation type="obsolete">Första s&amp;idans nummer:</translation>
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
        <translation type="obsolete">Förstasidans nummer i dokumentet</translation>
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
        <translation>Visa inte denna dialog igen</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Ursprungligt antal sidor i dokumentet</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Antal sid&amp;or:</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alla filer (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öppna</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nytt dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Öppna ett &amp;existerande dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Öppna senaste &amp;dokument</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished">Sidlayout</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation type="unfinished">Första sidan är:</translation>
    </message>
    <message>
        <source>Show Document Settings After Creation</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Ny &amp;från mall...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Ladda dokument med fördefinierad layout</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Starta dokumentet från en mall gjord av någon annan än du själv (t. ex. för dokument som ska ha en enhetlig stil).</translation>
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
        <translation>Åte&amp;rställ konturlinje</translation>
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
        <translation>Återställ kontrollpunkter</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Återställ denna kontrollpunkt</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Öppnar en polygon eller skär upp en Bezierkurva</translation>
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
        <translation type="obsolete">Minska storleken på kurvbanan med visad %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Öka storleken på kurvbanan med visad %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotationsvinkel</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">% att öka eller minska med</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivera redigering av konturlinje</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Återställ konturlinje till ursprunglig ramform</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">När denna ruta kryssas används koordinater relativt sidan,
annars används koordinater relativt objektet.</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Om ikryssad använd koordinater relativt sidan, annars använd koordinater relativt objektet.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Minska storleken på kurvbanan med visad %</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Minska storleken på kurvbanan med angivet värde</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Öka storleken på kurvbanan med angivet värde</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>Förstora eller förminska med %</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Förstora eller förminska med värdet</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Detta dokument verkar inte vara en OpenOffice.org Draw-fil.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importera &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importera OpenOffice.org Draw-filer</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importerar de flesta OpenOffice.org Draw-filer till aktuellt dokument med konvertering av vektordata till Scribus-objekt.</translation>
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
        <source>OpenDocument Importer Options</source>
        <translation>OpenDocument importalternativ</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Skriv över stilmallar</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Aktivering kommer att skriva över existernade stilmallar i aktuellt Scribusdokument</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sammanfoga stilmallar</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sammanfoga stilmallar efter attribut. Det resulterar i färre likartade stilmallar och bibehåller stilattribut även om dokumentets stilmallar är annorlunda namngivna.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Använd dokumentnamn som prefix för styckemallar</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Lägg till dokumentnamn i början av stilmallsnamnet i Scribus.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Fråga inte igen</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Gör dessa inställningar till standard och fråga inte igen vid import av OASIS OpenDocument.</translation>
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
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribusdokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokument</translation>
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
        <translation type="unfinished">Storlek</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Bredd:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="unfinished">Längd:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished">Höjd:</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Vinkel:</translation>
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
        <translation>Radbredd</translation>
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
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Spara som PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Sk&amp;icka till fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ä&amp;ndra...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Skicka en fil för varje si&amp;da</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
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
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Detta aktiverar export av en namgiven PDF-fil för varje sida i dokumentet. Sidnummer läggs automatiskt till. Oftast användbart för utskjutning av PDF för kommersiell tryckning.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Spara-knappen fungerar inte om du försöker exportera till PDF/X-3 och info-strängen saknas i PDF/X-3 fliken.</translation>
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
        <translation>PDF-verktyg</translation>
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
        <translation type="obsolete">Sk&amp;icka till fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">�amp;ndra...</translation>
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
        <translation type="obsolete">&amp;Spara</translation>
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
        <translation type="obsolete">Spara som</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF filer (*.pdf);;Alla filer (*)</translation>
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
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Sparar PDF</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Exporterar objekt från aktuell sida:</translation>
    </message>
    <message>
        <source>Exporting Master Page:</source>
        <translation>Exporterar mallsida:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Exporterar sida:</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="unfinished">Sida:</translation>
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
        <translation>Förhandsvisning</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">Anti-aliasing för &amp;text</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">Anti-aliasing för &amp;grafik</translation>
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
        <translation>Namn på separationen</translation>
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
        <source>Scaling:</source>
        <translation>Storleksförändring:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Skriv ut...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Tillhandahåller en snyggare vy på textobjekt i betraktaren på bekostnad av något långsammare förhandsvisning. Detta gäller bara teckensnitt av Typ 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Tillhandahåller en snyggare vy på TrueType, OpenType, EPS, PDF eller vektorgrafik i förhandsvisningen på bekostnad av något långsammare förhandsvisning</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Ett sätt att ta bort några av de gråa nyanser som skapas av en kombination av cyan, gult, magenta och svart. UCR påverkar mest delar av bilden som är neutrala och/eller mörka toner nära de grå nyanserna. Användningen av detta kan förbättra utskriften av vissa bilder och experiment och testning från fall till fall är nödvändigt. UCR förhindrar överstyrning av färgmättnad för CMG tryckfärger.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Återställ storleksvisningen av sidan.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Stäng</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Forcera övertryckning</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Spegelvänd sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Spegelvänd sida(or) vertikalt</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Beskär till sidmarginalerna</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Skriv ut i gråskala</translation>
    </message>
    <message>
        <source>Convert Spot Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Använd ICC-profiler</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Tillåter att tonplattor konverteras till sammansatta färger. Om du inte tänker skriva ut färger som tonplattor på ett tryckeri bör du helst lämna denna aktiverad.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished">Tillåter övertryckning av hela dokumentet oavsett inställningar för objekt</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">Gör det möjligt att bädda in ICC-profiler i utskriftsströmmen när färghantering är aktiverad</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Page:</source>
        <translation>Behandlar mallsida:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Exporterar sida:</translation>
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
        <translation type="unfinished">Egenskaper</translation>
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
        <translation type="unfinished">Anfanger</translation>
    </message>
    <message>
        <source>Parent&apos;s Drop Cap Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulatorer och indrag</translation>
    </message>
    <message>
        <source>Ch&amp;aracter Style</source>
        <translation type="unfinished"></translation>
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
        <translation>En kopia av</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Hänger samman med</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Är förälder till</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Är barn till</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Värde</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Ta bort</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Ra&amp;dera</translation>
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
        <source>None</source>
        <comment>relationship</comment>
        <translation>Inget</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Attribut för sidobjekt</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Namn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Värde</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Samband</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Samband med</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiera</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Ra&amp;dera</translation>
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
        <translation>&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Sidlayout</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Första sidan är:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Dubbelsidig</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Mellan höger</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Dra sidor eller mallsidor till papperskorgen för att radera</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Här är alla dina mallsidor. För att skapa en ny sida drar du en mallsida till sidvyn nedan</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Arrangera sidorna</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Tillgängliga mallsidor:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokumentsidor:</translation>
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
        <translation>%1 av %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 av %1</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation type="obsolete">Kvarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letter</translation>
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
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">C</translation>
    </message>
</context>
<context>
    <name>PatternDialog</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Välj en katalog</translation>
    </message>
    <message>
        <source>Loading Patterns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Alla filer (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation type="obsolete">Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="obsolete">Sökresultat för:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Förhandsvisning</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Välj</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Avbryt</translation>
    </message>
</context>
<context>
    <name>PicSearchBase</name>
    <message>
        <source>Result</source>
        <translation type="unfinished">Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="unfinished">Sökresultat för:</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="unfinished">För&amp;handsvisning</translation>
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
        <translation type="obsolete">Ja</translation>
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
        <translation>Avbryt sökning</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Bildhantering</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - bildsökning</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Sökningen misslyckades: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Inga bilder med namnet &quot;%1&quot; hittades.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Välj ett basbibliotek att starta sökning från</translation>
    </message>
    <message>
        <source>Search Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Stäng</translation>
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
        <translation>Spara som b&amp;ild...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportera som bild</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exporterar valda sidor som bitmappade bilder.</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished">Spara som bild</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">Fel  vid skrivning till fil(er).</translation>
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
        <translation>Kan inte hitta plugin</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>Okänt fel</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Kan inte hitta symbol (%1)</translation>
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
        <translation>Plugin: Laddar %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>init misslyckades</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>okänd typ av plugin</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 laddad</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 laddningen misslyckades: %2</translation>
    </message>
    <message>
        <source>Plugin: %1 initialized ok </source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 uppstart ok </translation>
    </message>
    <message>
        <source>Plugin: %1 failed post initialization</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 misslyckades efter uppstart</translation>
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
        <translation>Plugin-hanterare</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Så kör du</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Ladda?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Plugin ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
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
        <source>You need to restart the application to apply the changes.</source>
        <translation>Du måste starta om applikationen för att ändringarna ska få effekt.</translation>
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
        <translation>&amp;Hörn:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Använd &amp;faktor</translation>
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
        <translation>Antal hörn för polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Rotationsvinkel för polygoner</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Använd konvex-/konkavfaktorn för att ändra formen på polygonen</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Förhandsvisning av polygon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Ett negativt värde gör polygonen konkav (stjärnformad) och
ett positivt värde gör den konvex</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Ett negativt värde gör polygonen konkav (stjärnformad) och ett positivt värde gör den konvex</translation>
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
        <translation type="unfinished">Klippbok</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visning</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externa verktyg</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">�rigt</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="obsolete">GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="obsolete">&amp;Tema:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">T&amp;ypgrad:</translation>
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
        <translation type="obsolete">Mus&amp;hjul:</translation>
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
        <translation type="obsolete">Senas&amp;te dokument:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="obsolete">Sökvägar</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="obsolete">&amp;Dokument:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">Ä&amp;ndra...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="obsolete">&amp;ICC profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="obsolete">Ä&amp;ndra...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="obsolete">&amp;Skript:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="obsolete">Ändr&amp;a...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Sidstorlek</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Anpassad</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Storlek:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Landskap</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Höjd:</translation>
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
        <translation type="obsolete">Marginallinjer</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">Övers&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Vänster:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Autospar</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">Aktiv&amp;erad</translation>
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
        <translation type="obsolete">Andra alternativ</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="obsolete">Spara innehåll &amp;vid ändringar</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Förhandsvisning</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Liten</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Medium</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="obsolete">Stor</translation>
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
        <translation type="obsolete">Visa icke &amp;utskrivbar yta i marginalfärg</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Använd egenskaper i PDF 1.4 för &amp;transparens </translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="obsolete">&amp;Anpassa visningsstorlek</translation>
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
        <translation type="obsolete">&amp;Namn på program.</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="obsolete">Anti-alias &amp;text</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="obsolete">Anti-alias &amp;grafik</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="obsolete">Verktyg för bildhantering</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="obsolete">Namn på &amp;program:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Utskrift</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">Klipp efter sidans &amp;marginaler</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="obsolete">Använd &amp;underfärgsborttagning</translation>
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
        <translation type="obsolete">Använd standardval för fönsterdekor och utseende.
Scribus ärver tillgängliga teman från KDE och Qt.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="obsolete">Standardteckengrad för menyer och fönster</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="obsolete">Standardmåttenhet för redigering av dokument</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="obsolete">Antal rader som Scribus rullar för varje steg på mushjulet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Radien för det område som Scribus tillåter att du markerar handtagen på ett objekt</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="obsolete">Antal tidigare redigerade dokument som ska visas i Fil-menyn</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="obsolete">Standardkatalog för dokument</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Standardkatalog för ICC-profiler</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="obsolete">Standardkatalog för skript till Scripter</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="obsolete">Standard sidstorlek</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="obsolete">Standardriktning för dokumentsidor</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumentets sidbredd, redigerbar om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumentets sidhöjd, redigerbar om du valt anpassad sidstorlek</translation>
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
        <translation type="obsolete">Om aktiverad kommer Scribus att spara en backup av din fil med filtillägget .bak
efter varje tidsperiod</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="obsolete">Tidsperiod mellan automatspar</translation>
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
        <translation type="obsolete">Välj storlek på förhandsvisning i Klippboken</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="obsolete">Spara innehållet i Klippboken efter varje ändring</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Vid användandet av motstående sidor visa dessa sida vid sida</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Färg för papper</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Färg för marginallinjer</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Markera område utanför marginalerna i samma färg som marginallinjerna</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Aktivera funktionerna för transparens vid export som PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="obsolete">Ange standardvärde för zoomnivå</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="obsolete">Ghostscript-tolkens plats i filsystemet</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Anti-alias av text vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Anti-alias av grafik vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Bildbehandlingsprogrammets plats i filsystemet</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Visa inte objekt utanför marginalerna vid utskrift eller export av filen</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Välj en katalog</translation>
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
        <translation type="obsolete">Ä&amp;ndra...</translation>
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
        <translation type="obsolete">En metod för att ta bort en del av de gråtoner som kan uppstå
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
        <translation type="obsolete">S&amp;pråk:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="obsolete">Enheter:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="obsolete">Ångra/Gör om</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="obsolete">Längd på aktivitetshistorik</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Avstavare</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Teckensnitt</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Färghantering</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tangentbordsgenvägar</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Sidvisning</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Färg:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Visa bilder</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Visa textkedjor</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Visa ramar</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="obsolete">Kladdyta</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation></translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Slår av eller på visning av ramar</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Slår av eller på visning av bilder</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="obsolete">Dokum&amp;entmallar:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Verifiering (Pre-flight)</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Attribut för dokumentobjekt</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innehållsförteckning och index</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Visa kontrolltecken</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Linjaler relativt sidan</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Mellanrum mellan sidor</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horisontell:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Vertikal:</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="obsolete">För att justera visningen dra linjalen nedanför skjutreglaget.</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="obsolete">Fråga alltid vid laddning av dokument om teckensnitt ska ersättas</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="obsolete">Förhandsvisning av aktuell stilmall när stilar redigeras</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Visa uppstartsdialog</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="obsolete">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="obsolete">Använd alltid standard Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="obsolete">Antal stycken:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Diverse</translation>
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
        <translation type="obsolete">Visa osynliga tecken, t. ex. styckemarkörer i textramar</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="obsolete">Ytterligare katalog för dokumentmallar</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="obsolete">Placera en linjal mot skärmen och dra skjutreglaget för zoom-nivå tills Scribus visar sidor och objekt i korrekt storlek</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Definierar tillgängligt utrymme på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Definierar tillgängligt utrymme till höger på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Definierar tillgängligt utrymme upptill på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Definierar tillgängligt utrymme underst på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="obsolete">(Av)aktiverar  visning av länkade textramar.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Välj det språk du vill att Scribus ska använda. Lämna detta blankt om du vill använda miljövariablerna. Du kan gå förbi dessa genom att ange en modifierare vid start av Scribus från kommandoraden</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="obsolete">Teckengra&amp;d (Menyer):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="obsolete">Teckengrad (&amp;Paletter):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="obsolete">Välj grundläggande utseende på fönster. Scribus ärver från tillgängliga KDE eller Qt-teman, om Qt är konfigurerat för att använda KDE-insticksprogram.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="obsolete">Standard teckengrad för verktygsfönster</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="obsolete">Standardbibliotek för ICC-profiler. Kan inte ändras med ett dokument öppet. Som standard söker Scribus i systembiblioteken under Mac OSX och Windows. På Linux och Unix söker Scribus i $home/.color/icc, /usr/share/color/icc och /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="obsolete">Om aktiverad sparar Scribus en kopia av filen med ändelsen .bak varje gång en viss tidsperiod har gått</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Ange längden på historiken stegvis. Om den anges till 0 kommer ett obegränsat antal steg i historiken att sparas.</translation>
    </message>
    <message>
        <source>Show S&amp;plashscreen On Startup:</source>
        <translation type="obsolete">Visa en s&amp;plash vid uppstart:</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Skrivare</translation>
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
        <translation>Stan&amp;dardvärden</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation type="obsolete">Spara...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Spara inställningar</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished"></translation>
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
        <source>Left Page</source>
        <translation type="obsolete">Vänster sida</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Höger sida</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Föra över inställningar från tidigare Scribus?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus har upptäckt en existerande inställningsfil för Scribus 1.2.
Vill du föra över dem till den nya versionen av Scribus?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Kan inte öppna inställningsfilen &quot;%1&quot; för skrivning: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Skrivning till inställningsfilen &quot;%1&quot; misslyckades: QIODevice statuskod %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Misslyckades att öppna inställningsfilen &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Misslyckades att läsa XML inställningar från &quot;%1&quot;: %2 på rad %3, kolumn %4</translation>
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
        <translation>Fel vid skrivande till Inställningar</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus kunde inte spara sina inställningar:&lt;br /&gt;%1&lt;br /&gt;kontrollera rättigheterna till filen och katalogen samt tillgängligt diskutrymme.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Fel vid inläsning av Inställningar</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scrius kunde inte läsa in Inställningar:&lt;br /&gt;%1&lt;br /&gt;Standardinställningar kommer att läsas in.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Magasin</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Gyllene snittet</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Nio delar</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Du kan välja en fördefinierad sidlayout här. &apos;Inget&apos; behåller marginalerna som de är, Gutenberg anger klassiska mått och &apos;Magazine&apos; sätter alla marginaler lika. Indrag är värdet för vänster insida.</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Inget</translation>
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
        <translation>Öppn&amp;a...</translation>
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
        <source>&amp;Exit</source>
        <translation>Avsl&amp;uta</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>Kö&amp;r</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Kör som &amp;konsol</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Spara skickat...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus Python-konsol</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Denna är hämtad från Pythons standardkonsol med några begränsningar, framförallt med avseende på mellanrum. Konsultera Scribus handbok förmer information.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Skriptkonsol</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Skriv dina kommandon här. Ett urval processas som skript</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Resultat av ditt skript</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python skript (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Spara Python-kommandon i en fil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varning</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Textfiler (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Spar aktuellt resultat</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Öppna Python skriptfil</translation>
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
        <translation type="obsolete">&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Nej</translation>
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
        <translation>Översty&amp;kning</translation>
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
        <translation>Ån&amp;gra</translation>
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
        <translation type="obsolete">Varning</translation>
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
        <translation type="obsolete">Spara som bild</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Fel  vid skrivning till fil(er).</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="obsolete">Fel  vid skrivning till fil(er).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="obsolete">Exporten lyckades.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="obsolete">Filen existerar redan. Vill du skriva över?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="obsolete">existerar redan. Villdu skriva över?</translation>
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
        <source>Yes all</source>
        <translation type="obsolete">Ja, allt</translation>
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
        <translation type="obsolete">Alla format som stöds (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öppna</translation>
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
        <translation>&amp;Scribus skripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Utför skript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Se&amp;naste skript</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Visa &amp;konsol</translation>
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
        <translation>Media-alternativ</translation>
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
        <translation type="obsolete">Brasilianska</translation>
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
        <translation type="obsolete">Norska (Bokmål)</translation>
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
        <translation>&amp;Om Skript...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Om Skript</translation>
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
        <translation>Enhetet utanför gränserna. Använd en av scribus.UNIT_* konstanter.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Målet är inte en bildram.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Hörnradien måste vara ett positivt tal.</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan inte hämta en färg utan ett namn.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan inte ändra en färg utan namn.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan inte skapa en färg utan namn.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan inte ta bort en färg utan namn.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan inte ersätta en färg utan namn.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org ordbehandlingsdokument</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Färgen hittades inte - python-fel</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Anpassad (valfri) konfiguration: </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardkonfiguration: </translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Korta ord behandlas: Vänligen vänta...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Korta ord behandlas. Klart.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaan</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turkiska</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainska</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Walesiska</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Filnamnet måste vara en sträng.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Kan inte ta bort inställningar för bildtyp.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Bildtypen måste vara en sträng.</translation>
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
        <translation type="obsolete">Anpassad</translation>
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
        <translation type="unfinished">&amp;Skript</translation>
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
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Teckensnitt %1 har ett skadat tecken %2 (teckenkod %3)</translation>
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
        <translation type="unfinished">Färghjul</translation>
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
        <translation type="obsolete">Exportera som bild</translation>
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
        <translation type="unfinished">Skript-editor</translation>
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
        <translation type="unfinished">&amp;Importera</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Arabiska</translation>
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
        <translation type="unfinished">Kvarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="unfinished">Folio</translation>
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
        <source>Scribus 1.3.0-&gt;1.3.3.2 Support</source>
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
        <translation>Ån&amp;gra</translation>
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
        <translation>Återställ</translation>
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
        <translation>Åte&amp;rställ</translation>
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
        <translation>Återställ nedåt</translation>
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
        <translation type="obsolete">Marginallinjer</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">Övers&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Vänster:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">Höge&amp;r:</translation>
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
        <translation type="obsolete">Sidstorlek</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Storlek:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Anpassad</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Sidriktning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Landskap</translation>
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
        <translation type="obsolete">&amp;Storlek:</translation>
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
        <translation type="obsolete">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Bredd:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Höjd:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="obsolete">En&amp;het:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Autospar</translation>
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
        <source>Guides</source>
        <translation>Stödlinjer</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Sidvisning</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Färg:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Visa icke &amp;utskrivbar yta i marginalfärg</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Visa bilder</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Visa textkedjor</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Visa ramar</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visning</translation>
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
        <source>Hyphenator</source>
        <translation>Avstavare</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Teckensnitt</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Färghantering</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Slår av eller på visning av ramar</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Slår av eller på visning av bilder</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Färg för papper</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Markera område utanför marginalerna i samma färg som marginallinjerna</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Alternativ</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformation</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Visa kontrolltecken</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Linjaler relativt sida</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="obsolete">Minimal kladdyta</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Mellanrum mellan sidor</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horisontell:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Vertikal:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Verifiering (Pre-flight)</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Attribut för dokumentobjekt</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innehållsförteckning och index</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Visa osynliga tecken, t. ex. styckemarkörer i textramar</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Justerar färger</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="obsolete">Slå på eller av visning av länkade textramar.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="obsolete">Använd sidinställningar på alla sidor</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Avsnitt</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="obsolete">Använd förändring av sidstorlek på alla sidor i dokumentet</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skript (*.py);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Kör som utvidgat skript</translation>
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
        <translation>Stilmallshanterare</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation type="obsolete">Kolumn 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation type="obsolete">K&amp;lona</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Namn:</translation>
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
        <source>Ca&amp;ncel</source>
        <translation type="obsolete">Av&amp;bryt</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Använd</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Ny</translation>
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
        <translation type="unfinished">&amp;Importera</translation>
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
        <translation type="unfinished">Åte&amp;rställ</translation>
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
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMCharacterStyle</name>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Egenskaper</translation>
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
        <translation type="unfinished"></translation>
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
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="obsolete">Linjer</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">Heldragen linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Streckad linje</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Punktlinje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Streck-punkt linje</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">Streck-punkt-punkt linje</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>SMPStyleWidget</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished">Fast radavstånd</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished">Automatiskt radavstånd</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">Justera mot baslinjemönstret</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
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
        <translation type="unfinished">Anfanger</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulatorer och indrag</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Egenskaper</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished">&amp;Linjer:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation type="unfinished">Avstånd till text:</translation>
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
        <translation type="unfinished">Styckemallar</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Ta bort</translation>
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
        <translation>Ta bort stilmallar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>Av&amp;bryt</translation>
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
        <translation type="unfinished">Ingen stilmall</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation>Ändra teckenhöjd</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuell spärrning</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Spara sida som &amp;SVG</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Exporterar SVG-filer</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Exporterar aktuell sida som SVG-fil.</translation>
    </message>
    <message>
        <source>Save as &amp;SVG...</source>
        <translation>Spara som &amp;SVG...</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importera &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importera SVG-filer</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importerar de flesta SVG-filer till aktuellt dokument,
och konverterar deras vektordata till Scribusobjekt.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG-filen innehåller någon egenskap som saknar stöd</translation>
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
        <translation>Korta ord

Korta ord insticksprogram</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Använd hårt mellanslag på:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>Valda &amp;ramar</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Aktuell &amp;sida</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Alla objekt</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Endast valda ramar har processats.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Endast aktuell sida har processats.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alla objekt i dokumentet har processats.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Användarinställningar</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Systemgemensam konfiguration</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Spara</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Åte&amp;rställ</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Spara användarkonfigurationen</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Ladda om systemgemensam konfiguration och ta bort den användardefinierade</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Redigera anpassad konfiguration. Om du sparar kommer den att användas isället för den systemgemensamma</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Korta ord</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Användarkonfiguration finns redan. Vill du skriva över?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Kaninte skriva till fil %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Användarinställningar sparade</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Systemgemensam konfiguration har laddats om</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Kan inte öppna filen %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Spara som &amp;mall...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Spara dokument som mall</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Spara dokument som mall. Ett bra sätt att förenkla det inledande arbetet med ett enhetligt utseende</translation>
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
        <translation type="unfinished">&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Bestående</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Importera</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Aktivitet</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Läs in/Spara/Importera/Exportera</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Okänd</translation>
    </message>
</context>
<context>
    <name>ScProgressBar</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 av %2</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Överst</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Höger</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Nederst</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vänster</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Tillåt anslutning till...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontell</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Flytande riktning...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Skriver ut...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopia #%1 av</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Bakgrund</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
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
        <translation type="unfinished">Bakgrund</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopia #%1 av</translation>
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
        <translation type="unfinished">Kopia #%1 av</translation>
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
        <translation type="unfinished">Kopia #%1 av</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Fil</translation>
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
        <translation type="obsolete">Sök efter teckensnitt</translation>
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
        <translation type="obsolete">Allvarligt fel</translation>
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
        <translation type="obsolete">Läser inställningar</translation>
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
        <translation type="obsolete">Skapar genvägar</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Färghantering...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Läser Klippboken</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Startar insticksprogram</translation>
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
        <translation type="obsolete">�pna s&amp;enaste</translation>
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
        <translation type="obsolete">&amp;Importera</translation>
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
        <translation type="obsolete">&amp;Exportera</translation>
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
        <translation type="obsolete">&amp;Form</translation>
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
        <translation type="obsolete">Ver&amp;ktyg</translation>
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
        <translation type="obsolete">Om Qt</translation>
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
        <translation type="obsolete">&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">R&amp;edigera</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">St&amp;il</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">E&amp;nhet</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Sida</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Vy</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">Ins&amp;tällningar</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">Fönste&amp;r</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hjälp</translation>
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
        <translation type="obsolete">X-Pos:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Pos:</translation>
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
        <translation type="obsolete">Klar</translation>
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
        <translation type="obsolete">Inget</translation>
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
        <translation type="obsolete">&amp;Färg</translation>
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
        <translation type="obsolete">&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Storlek</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Justering</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">Nyan&amp;s</translation>
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
        <translation type="obsolete">Storlek</translation>
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
        <translation type="obsolete">Nyans</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Lås upp</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">�pna</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Importerar sida/sidor...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Importera sida/sidor</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Du försöker importera fler sidor än det finns i aktuellt dokument räknat från aktiv sida.&lt;/p&gt;
Välj något av följande:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Skapa&lt;/b&gt; de saknade sidorna&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importera&lt;/b&gt;befintliga sidor&lt;/li&gt;
&lt;li&gt;&lt;b&gt;Avbryt&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Skapa</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importera</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Import klar</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Hittade inget att importera</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Laddar...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Alla format som stöds</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alla filer (*)</translation>
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
        <translation type="obsolete">Spara som</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Sparar...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Skriver ut...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Utskriften misslyckades!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Scribus handbok</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Textfiler (*.txt);;Alla filer (*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Storlek:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">Nyan&amp;s:</translation>
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
        <translation type="obsolete">Allt</translation>
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
        <translation type="obsolete">Teckensnittshanteringen initierad</translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Avbryt</translation>
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
        <translation type="obsolete">Information</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">saknas!</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Skicka till la&amp;ger</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF-alternativ</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Tecken</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Spara som</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Namn:</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Prov</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Färg</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation type="obsolete">Dokumentfärger</translation>
    </message>
</context>
<context>
    <name>ScribusCore</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Startar insticksprogram</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Startar Tangentbordsgenvägar</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Läser inställningar</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Läser ICC-profiler</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Sök efter teckensnitt</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Det finns inga teckensnitt i ditt system.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Avslutar nu.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Allvarligt fel</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Teckensnittshanteringen initierad</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nytt lager</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrund</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Vill du verkligen radera all text?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Kan inte ta bort objekt som används</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Objekt %1 redigeras i Texthanteraren. Operationen Ta bort avbryts därför</translation>
    </message>
    <message>
        <source>An error occurred while opening icc profiles, color management is not enabled.</source>
        <translation type="obsolete">Ett fel uppstod vid öppnandet av ICC-profiler. Färghanteringen är därför inte aktiverad.</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Några objekt är låsta.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Lås &amp;upp alla</translation>
    </message>
    <message>
        <source>&amp;Skip locked objects</source>
        <translation>Hoppa över lå&amp;sta objekt</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished">Antal kopior: %1
Horisontell förskjutning: %2
Vertikal förskjutning: %3</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Justerar färger</translation>
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
        <source>None</source>
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Startar insticksprogram</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Läser inställningar</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Starta Texthanteraren</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Starta Avstavningen</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Läser Klippboken</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Skapar genvägar</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Redigera</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Sök efter teckensnitt</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Allvarligt fel</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Teckensnittshanteringen initierad</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Öppna s&amp;enaste</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importera</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportera</translation>
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
        <source>&amp;Color</source>
        <translation>&amp;Färg</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Storlek</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Nyan&amp;s</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>Ob&amp;jekt</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Inställningar Förhandsvisning</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Skicka till la&amp;ger</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-alternativ</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>K&amp;onvertera till</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>I&amp;nfoga</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tecken</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citat</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellanslag</translation>
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
        <source>&amp;Windows</source>
        <translation>Fönste&amp;r</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjälp</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Justering</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Klar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öppna</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importerar sidor...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importera sida/sidor</translation>
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
        <source>File %1 is not in an acceptable format</source>
        <translation>Fil %1 har inte ett passande format</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Laddar...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Några ICC-profiler som används i dokumentet är inte installerade:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> har ersatts av: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(konverterade)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Alla format som stöds</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alla filer (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Kan inte skriva till filen:
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.sla.gz *.scd *scd.gz);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokument (*.sla *.scd);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Sparar...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus har upptäckt några fel. Använd verifieraren (Preflight) för att korrigera dem</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorera</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Avbryt</translation>
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
        <source>Cannot Cut In-Use Item</source>
        <translation>Kan inte klippa ut objekt som används på annat håll</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Objekt %1 redigeras just nu i Texthanteraren. Operationen Klipp ut avbryts därför</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus handbok</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Spara som</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textfiler (*.txt);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Namn:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Konvertera sidan till mallsida</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Storlek:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Storlek</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Nyan&amp;s:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Rulla upp</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stilmall</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Följande program saknas:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: Du kan inte använda EPS-bilder eller Förhandsvisning</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Allt</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus upptäckte några fel.
Använd verifieraren (Preflight) för att korrigera dem.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS-filer (*.eps);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Upptäckte några fel.
Använd verifieraren (Preflight) för att korrigera dem</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Sida%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Några objekt är låsta.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Lås alla</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Lås &amp;upp alla</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Programmet %1 körs redan!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Programmet %1 saknas!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Den valda färgen finns inte dokumentets färguppsättning. Skriv in ett namn för den nya färgen.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Färgen hittades inte</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Det namn du valt finns redan. Ange ett annat namn för den nya färgen.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Nivå</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Skicka till lager</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Förhandsvisa instä&amp;llningar</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Ver&amp;ktyg</translation>
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
        <source>New Master Page %1</source>
        <translation>Ny mallsida %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="obsolete">Antal kopior: %1
Horisontell förskjutning: %2
Vertikal förskjutning: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: Du kan inte använda EPS-bilder eller PostScript Förhandsvisning</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript saknas: PostScript förhandsvisning är inte tillgängligt</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Vill du verkligen ersätta befintlig bild?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innehåll</translation>
    </message>
    <message>
        <source>&amp;Character</source>
        <translation>Te&amp;cken</translation>
    </message>
    <message>
        <source>&amp;Quote</source>
        <translation>Ci&amp;tat</translation>
    </message>
    <message>
        <source>S&amp;paces &amp;&amp; Breaks</source>
        <translation>Me&amp;llanslag &amp;&amp; brytningar</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;turer</translation>
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
        <translation type="unfinished">Vill du verkligen radera all text?</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Ogiltigt argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Filen %1 finns inte - Avbryter.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Användning: scribus [alternativ...] [fil]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Alternativ:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Skriv ut hjälpen (detta meddelande) och avsluta</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Använder xx som genväg för språk, t. ex. &apos;en&apos; eller &apos;sv&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Listar alla tillgängliga gränssnittspråk</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Visar information i konsolen när tecken laddas</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Visa inte splash vid uppstart</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Skriv ut versionsinformation och avsluta</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Använd höger-vänster ordning på dialogknapparna (t. ex. Avbryt/Nej/Ja istället för Ja/Nej/Avbryt)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>filnamn</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Använd filnamn som sökväg för användarangivna inställningar</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Installerade gränssnittspråk för Scribus är följande:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Skriv över standardval för språk:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx eller scribus -lang xx, där xx är beteckningen för valt språk.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus version</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Desktop Publishing med öppen källkod</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hemsida</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentation</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Uppgifter</translation>
    </message>
    <message>
        <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
        <translation>Stoppa visning av splashen vid uppstart. Skriver en tom fil kallad .neversplash i ~/.scribus.</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation type="obsolete">Lager</translation>
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
        <translation>Kopiera hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Flytta hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>K&amp;listra in</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Ursprunglig PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Verklig PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Länkad text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Textram</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text på kurvlinje</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Stycken: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tecken: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Skriv ut: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Aktiverad</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Inaktiverad</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
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
        <translation>&amp;PDF-alternativ</translation>
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
        <translation>Skicka till la&amp;ger</translation>
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
        <translation>Ni&amp;vå</translation>
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
        <translation>Konve&amp;rtera till</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
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
        <translation>Inställningar för förhandsvisning</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Länkning av textramar</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Du försöker länka till en fylld ram eller till samma ram.</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Kan inte konvertera enhet som används</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Enheten %1 används av Textredigeraren. Konvertering till konturtecken av denna enhet avbryts därför</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Sida %1 till %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Färgrymd: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Okänd</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMGS</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innehåll</translation>
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
        <translation type="obsolete">Normalsyn</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Protanopia (Red)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Deuteranopia (Green)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Tritanopia (Blue)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Full färgblindhet</translation>
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
        <translation>&amp;Kasta</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Inbäddat stöd för Python-skript.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skript-editor</translation>
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
        <translation>Skriptfel</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Om du kör ett officiellt skript, vänligen rapportera till &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;, tack.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Detta meddelande finns också på ditt klippbord. Använd Ctrl+V för att klistra in i felhanteraren.</translation>
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
        <translation>Det uppstod ett internt fel när angivet kommando kördes. Detaljer om felet skrevs till stderr. </translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Undersök skriptet</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Inställning av Python-plugin misslyckades. Detaljer om felet skrevs till stderr. </translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="obsolete">Python skript (*.py);;Alla filer (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentation för:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Skript</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> innehåller inte någon &quot;docstring&quot;!</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);;All Files (*)</source>
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
        <translation>Inställningar för Skript-editor</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Tillåt utökande skript</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Utökning</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konsol</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Startskript:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Fel:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentarer:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Nyckelord:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Skyltar:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Nummer:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Strängar:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Grundtexter:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Markera färg</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ändra...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Hitta startskript</translation>
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
        <translation type="obsolete">Inget</translation>
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
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Sökning avslutad, hittade %1 matchningar</translation>
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
        <translation>Övrigt...</translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>X-Offset</source>
        <translation>X-Offset</translation>
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
        <translation>Kor&amp;ta ord...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Korta ord</translation>
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
        <translation type="unfinished">&amp;Ingen tangent</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">&amp;Användardefinierad tangent</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">Välj &amp;tangent</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation>Ingen stilmall</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Redigera stilmall...</translation>
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
        <translation>Öppna</translation>
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
        <translation>&amp;Importera</translation>
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
        <translation type="obsolete">Hämta text från fil</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Spara text till fil</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Uppdatera textramen och avsluta</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Avsluta utan att uppdatera textramen</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Hämta texten igen från textramen</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Uppdatera textramen</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Sök/Ersätt</translation>
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
        <translation>Öppna</translation>
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
        <translation>&amp;Infoga tecken...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Radera all text</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Texthanteraren - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Vill du verkligen förlora alla ändringar?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Vill du verkligen radera all text?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Infoga</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tecken</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citat</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mellanslag &amp;&amp; brytningar</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellanslag</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Automatiskt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Förskjutning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linjebredd</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation type="obsolete">Mer än en enhet är vald</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Namn:</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">Åte&amp;rställ</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importera</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Ta bort</translation>
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
        <translation type="unfinished">Ny</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished">Importera</translation>
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
        <translation type="unfinished">Ta bort</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">R&amp;edigera</translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished">Genväg</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Namn</translation>
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
        <translation type="unfinished">Redigera</translation>
    </message>
    <message>
        <source>New %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished">Den här knappsekvensen används redan</translation>
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
        <translation type="obsolete">Understruken</translation>
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
        <translation type="obsolete">�erstruken</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Konturtext</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">�ersikt</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Versaler</translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="obsolete">�ersikt</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Understruken text. Håll nere knappen kort för att ange alternativ för linjebredd och förskjutning.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Understrukna ord. Håll nere knappen kort för ange alternativ för linjebredd och avvikelse.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Genomstruket. Håll nere knappen kort för att ange alternativ för linjebredd och avvikelse.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <comment>Text Style Selector</comment>
        <translation>Konturlinje. Håll nere knappen kort för att ange bredd på konturlinjen.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Skuggad text. Håll knappen nere kort för ange skuggans avstånd.</translation>
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
        <translation>Skriv över stilmall för stycke</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Tillåter att skriva över befintliga stilmallar i aktuellt Scribus-dokument</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sammanfoga stilmallar</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sammanfoga stilmallar efter attribut. Det resulterar i färre likartade stilmallar och bibehåller stilattribut även om dokumentets stilmallar är annorlunda namngivna.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Lägg till dokumentnamn i början av stilmallsnamnet i Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Gör dessa inställningar till standard och fråga inte igen vid import av OpenOffice.org 1.x dokument.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>I början</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>I slutet</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Visas ej</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innehållsförteckning och index</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innehållsförteckning</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Ram som innehållsförteckningen monteras inuti</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Sidnummer monteras:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Namn på enhetsegenskaper:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Enhetsegenskaper som används på ramar som grund för innehållsrubriker</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Montera sidnummer för innehållsrubriker i början eller slutet av raden. Eller inte alls</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Lista icke utskrivbara innehållsrubriker</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Inkludera även ramar som markerats för icke utskrift</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Stilmall för stycke som används på innehållsrubriker</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Stilmall för stycke:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Målram:</translation>
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
        <translation>Innehållsförteckning och index</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innehållsförteckning</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Lägg till</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ta bort</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Ram som innehållsförteckningen monteras inuti</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Sidnummer monteras:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Namn på enhetsegenskaper:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Enhetsegenskaper som används på ramar som grund för innehållsrubriker</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Montera sidnummer för innehållsrubriker i början eller slutet av raden. Eller inte alls</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Lista icke utskrivbara innehållsrubriker</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Inkludera även ramar som markerats för icke utskrift</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Stilmall för stycke som används på innehållsrubriker</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Stilmall för stycke:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Målram:</translation>
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
        <translation>Ignorera alla fel</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Kontrollera automatiskt före utskrift eller export</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Leta efter saknade tecken</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="obsolete">Leta efter objekt som inte finns på någon sida</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Kontrollera om textramar flödar över</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="obsolete">Kontrollera använda transparenser</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Leta efter saknade bilder</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Kontrollera bildupplösning</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Lägsta tillåtna upplösning</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrollera PDF-anteckningar och fält</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Lägg till profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Ta bort profil</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Kontrollera monterade PDF-filer</translation>
    </message>
    <message>
        <source>Highest allowed resolution</source>
        <translation>Högsta tillåtna upplösning</translation>
    </message>
    <message>
        <source>Check for GIF images</source>
        <translation>Kontrollera bilder i GIF-format</translation>
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
        <translation type="unfinished">Färg för papper</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Markera område utanför marginalerna i samma färg som marginallinjerna</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">(Av)aktiverar  visning av länkade textramar.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Visa osynliga tecken, t. ex. styckemarkörer i textramar</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">Slår av eller på visning av ramar</translation>
    </message>
    <message>
        <source>Turns the display of layer indicators on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">Slår av eller på visning av bilder</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Definierar tillgängligt utrymme på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Definierar tillgängligt utrymme till höger på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Definierar tillgängligt utrymme upptill på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Definierar tillgängligt utrymme underst på dokumentytan tillgängligt för att skapa och modifiera objekt för att sedan kunna dra in dem på den aktiva sidan</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Ange standardvärde för zoomnivå</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished">Placera en linjal mot skärmen och dra skjutreglaget för zoom-nivå tills Scribus visar sidor och objekt i korrekt storlek</translation>
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
        <translation type="unfinished">Sidvisning</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Visa ramar</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Visa textkedjor</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Visa icke &amp;utskrivbar yta i marginalfärg</translation>
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
        <translation type="unfinished">Visa kontrolltecken</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Visa bilder</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Färg:</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">Kladdyta</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">Höge&amp;r:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gaps Between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Horisontell:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Vertikal:</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">För att justera visningen dra linjalen nedanför skjutreglaget.</translation>
    </message>
    <message>
        <source>textLabel16</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Allmänt</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Färger</translation>
    </message>
    <message>
        <source>Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="unfinished">Fyllningsfärg:</translation>
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
</context>
<context>
    <name>TabDocument</name>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Anpassad</translation>
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
        <source>Portrait</source>
        <translation type="unfinished">Porträtt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Landskap</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntering:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished">Enheter:</translation>
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
        <source>Margin Guides</source>
        <translation type="unfinished">Marginallinjer</translation>
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
        <source>Undo/Redo</source>
        <translation type="unfinished">Ångra/Gör om</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished">Längd på aktivitetshistorik</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumentets sidbredd, redigerbar om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumentets sidhöjd, redigerbar om du valt anpassad sidstorlek</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Standard sidstorlek</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Standardriktning för dokumentsidor</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished">Om aktiverad sparar Scribus en kopia av filen med ändelsen .bak varje gång en viss tidsperiod har gått</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Tidsperiod mellan automatspar</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished">Ange längden på historiken stegvis. Om den anges till 0 kommer ett obegränsat antal steg i historiken att sparas.</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">Använd förändring av sidstorlek på alla sidor i dokumentet</translation>
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
        <translation>Hitta Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Hitta din bildredigerare</translation>
    </message>
    <message>
        <source>Locate your web browser</source>
        <translation>Hitta din webbläsare</translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidgetBase</name>
    <message>
        <source>External Tools</source>
        <translation>Externa verktyg</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>PostScript-tolk</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Namn på program:</translation>
    </message>
    <message>
        <source>&amp;Change..</source>
        <translation>Ä&amp;ndra..</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Anti-alias &amp;text</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Anti-alias av text vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Anti-alias &amp;grafik</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Anti-alias av grafik vid visning av EPS eller PDF på skärmen</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Upplösning:</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
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
        <source>&amp;Change...</source>
        <translation>Än&amp;dra...</translation>
    </message>
    <message>
        <source>&amp;Rescan</source>
        <translation>Läs in i&amp;gen</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Lägger till sökväg till Ghostscript-tolken. Observera att på Windows måste sökvägen gå till gswin32c.exe - INTE till gswin32.exe. Risk finns att Scribus låser datorn vid uppstart om fel tolk används.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Sökväg till bildredigerare. Om du använder Gimp och din distribution inkluderar det så rekommenderar vi &apos;gimp-remote&apos;, eftersom det gör att du kan redigera en bild i en redan startad instans av Gimp.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Web Browser</source>
        <translation>Webbläsare</translation>
    </message>
    <message>
        <source>Web Browser to launch with links from the Help system</source>
        <translation>Webbläsare som startas med länkar i hjälpsystemet</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for your web browser. This is used for external links from the Help system.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Sökväg till webbläsare. Används för externa länkar från hjälpsystemet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Rescan for the external tools if they do not exist in the already specified location</source>
        <translation>Läs in externa verktyg igen om de inte redan finns på angiven plats</translation>
    </message>
</context>
<context>
    <name>TabGeneral</name>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">Välj det språk du vill att Scribus ska använda. Lämna detta blankt om du vill använda miljövariablerna. Du kan gå förbi dessa genom att ange en modifierare vid start av Scribus från kommandoraden</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished">Antal tidigare redigerade dokument som ska visas i Fil-menyn</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished">Antal rader som Scribus rullar för varje steg på mushjulet</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Välj grundläggande utseende på fönster. Scribus ärver från tillgängliga KDE eller Qt-teman, om Qt är konfigurerat för att använda KDE-insticksprogram.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Standardteckengrad för menyer och fönster</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">Standard teckengrad för verktygsfönster</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Standardkatalog för dokument</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">Standardbibliotek för ICC-profiler. Kan inte ändras med ett dokument öppet. Som standard söker Scribus i systembiblioteken under Mac OSX och Windows. På Linux och Unix söker Scribus i $home/.color/icc, /usr/share/color/icc och /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Standardkatalog för skript till Scripter</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">Ytterligare katalog för dokumentmallar</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Välj en katalog</translation>
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
        <translation type="obsolete">GUI</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">S&amp;pråk:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">&amp;Tema:</translation>
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
        <translation type="unfinished">Teckengra&amp;d (Menyer):</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished">Teckengrad (&amp;Paletter):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">Mus&amp;hjul:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">Senas&amp;te dokument:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Sökvägar</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Dokument:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">&amp;ICC profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Ä&amp;ndra...</translation>
    </message>
    <message>
        <source>Alt+H</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished">&amp;Skript:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished">Ändr&amp;a...</translation>
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
    <message>
        <source>User Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished">Visa uppstartsdialog</translation>
    </message>
    <message>
        <source>Show Splashscreen on Startup</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Allmänna inställningar</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Placering i dokumenten</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>I bakgrunden</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>I förgrunden</translation>
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
        <translation> px</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="obsolete">Stödlinjer</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Visa stödlinjer</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Visa marginaler</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Visa rutmönster</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Stora rutor</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Färg:</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Avstånd:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Små rutor</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Baslinjer</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Visa rutmönster av baslinjer</translation>
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
        <translation>Mönster av baslin&amp;jer:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Baslinjer o&amp;ffset:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Avstånd mellan smårutor i stödraster</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Avstånd mellan stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Avstånd inom vilket ett objekt låses till dina utplacerade stödlinjer</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radien för det område som Scribus tillåter att du markerar handtagen på ett objekt</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Färg på linjer för smårutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Färg på linjer för stora rutor i stödraster</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Färg på infogade stödlinjer</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Färg för marginallinjer</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Slår av och på baslinjemönstret</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Slår av och på rutmönster</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Slår av och på hjälplinjerna</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Slår av och på marginallinjer</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Procentuell ökning av radavstånd baserat på typgraden </translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Inställningar för baslinjen</translation>
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
    <name>TabKeyboardShortcutsWidget</name>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Key Set XML filer (*.ksxml)</translation>
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
        <translation>Skift</translation>
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
        <translation>Skift+</translation>
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
        <translation>Den här knappsekvensen används redan</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidgetBase</name>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tangentbordsgenvägar</translation>
    </message>
    <message>
        <source>Search:</source>
        <translation>Sök:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Åtgärd</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation>Genväg</translation>
    </message>
    <message>
        <source>Shortcut for Selected Action</source>
        <translation>Genväg för vald åtgärd</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen tangent</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Användardefinierad tangent</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Välj &amp;tangent</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>CTRL+ALT+SHIFT+W</source>
        <translation>CTRL+ALT+SKIFT+W</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Inläsningsbara uppsättningar av genvägar</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Tangentbordsgenvägar är tillgängliga för inläsning</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Läs in</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Läs in den valda uppsättningen av genvägar</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importera...</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importera en uppsättning genvägar till aktuell konfiguration</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Exportera...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Exportera aktuella genvägar till en importerbar fil</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Åte&amp;rställ</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Ladda om Scribus standardgenvägar</translation>
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
    <name>TabMiscellaneousBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="unfinished">Fråga alltid vid laddning av dokument om teckensnitt ska ersättas</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">Förhandsvisning av aktuell stilmall när stilar redigeras</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Visa uppstartsdialog</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">Använd alltid standard Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished">Antal stycken:</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Skriv ut område</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Alla sidor</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Vä&amp;lj sidor</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Alternativ för fil</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Kompatibilit&amp;et:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Bindning:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Vänster marginal</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Höger marginal</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Skapa minia&amp;tyrer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Spara &amp;länkade textramar som PDF-artiklar</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Inkludera bokmärken</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Upplösning:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primera text och vektorgrafik</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Bildinställningar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
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
        <translation>Inget</translation>
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
        <translation>Maximum</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Hög</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Låg</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimum</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Mi&amp;nska upplösningen på bilder till:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Allmänt</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">Bädda in alla t&amp;eckensnitt</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Gruppera alla tecken&amp;snitt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Bäddar in</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tillgängliga teckensnitt:</translation>
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
        <translation>Teckensnitt att bädda in:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Teckensnitt att gruppera:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Teckensnitt</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Aktivera &amp;presentationseffekter</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Sida</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Förhands&amp;visning</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Ti&amp;d för bildvisning:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Tid för effek&amp;t:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>T&amp;yp av effekt:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>Rörliga lin&amp;jer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>F&amp;rån:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>R&amp;iktning:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Persienn</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>LådaBox</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Upplösa</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Uppdelning</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Torka</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontell</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Inifrån</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Utifrån</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Vänster till höger</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Uppifrån och ner</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Nerifrån och upp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Höger till vänster</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Diagonalt från övre vänstra hörnet</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Applicera effekten på alla sidor</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Använd kryptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Lösenord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Användare:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Ä&amp;gare:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Inställningar</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Tillåt &amp;utskrift av dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Tillåt än&amp;dring av dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Tillåt kop&amp;iering av text och grafik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Tillåt att lägga till &amp;anteckningar och fält</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>Säk&amp;erhet</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allmänt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Utmatning&amp;en är avsedd för:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Skärmen/webben</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Skrivare</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Använd anpassade inställningar för återgivningen</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Inställningar för återgivning</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Fre&amp;kvens:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Punktform:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enkel punkt</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linje</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ellips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Fyllda färger:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Använd ICC-profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Återgivningsavsikt:</translation>
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
        <source>Images:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Använd inte inbäddade ICC-profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Fä&amp;rg</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 Utmatningsavsikt</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informationssträng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Utmatnings&amp;profil:</translation>
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
        <translation>Förhandsvisa varje sida i listan ovanför.</translation>
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
        <translation>Typ av visningseffekt.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Riktning för effekten vid användande av effekterna Blinds eller Split.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Startposition för effekterna Låda eller Uppdelning.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Riktning för effekterna Torka eller Glitter.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Använd vald effekt på alla sidor.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportera alla sidor till PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportera en grupp av sidor till PDF</translation>
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
        <translation>Skapar PDF-artiklar, användbart för navigering av länkade artiklar i PDF.</translation>
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
        <translation>DPI (Dots Per Inch) för export av bilder.</translation>
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
        <translation>Välj ett lösenord för dina användare som kan användas för att läsa din PDF-fil.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Tillåt utskrift av PDF-filen. Om ej markerad så tillåts ej utskrift. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Tillåt ändring av PDF-filen. Om ej kryssad är ändring förhindrad.</translation>
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
        <translation>Bädda in en färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Färgprofil för fyllda färger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Återgivningsavsikt för tonplattor</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Bädda in en färgprofil för bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Använd inte färgprofiler som är inbäddade i ursprungliga bildfiler</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Färgprofil för bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Återgivningsavsikt för bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Utmatningsprofil för utskrift. Om möjligt bör du rådgöra med din tryckare för korrekt val av profil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Obligatorisk sträng för PDF/X-3 annars följer inte PDF-filen(new line)
reglerna för PDF/X-3. Vi rekommenderar att du använder dokumentets titel.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Avstånd för utfall räknat från övre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Avstånd för utfall räknat från nedre kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Avstånd för utfall räknat från vänstra kanten av pappret</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Avstånd för utfall räknat från högra kanten av pappret</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Spegla sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Spegla sida(or) vertikalt</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>Upplösning fö&amp;r EPS-grafik:</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Omvandla tonplattor till processfärger</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Komprimeringskvalitet:</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="obsolete">Nivåer av komprimeringskvaliteter för kastande metoder: Minimum (25%), Låg (50%), Medium (75%), Hög (85%) och Maximum (95%). Notera att en given kvalitetsnivå inte bestämmer storleken på den resulterande filen - både storlek och kvalitetsförlust varierar från bild till bild för varje kvalitetsnivå.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Tillåt kopiering av text eller grafik från PDF-filen. Om omarkerad kan inte text eller grafik kopieras.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Tillåt tillägg av anteckningar och fält till PDF-filen. Om omarkerad kan redigering av anteckningar och fält inte göras.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Tillåter att tonplattor konverteras till sammansatta färger. Om du inte tänker skriva ut färger som tonplattor på ett tryckeri bör du helst lämna denna aktiverad.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Inkludera la&amp;ger</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Komprimeringsmeto&amp;d:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Bädda in teckensnitt i PDF-filen. Inbäddning bibehåller layout och utseende på ditt dokument.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Tidsrymd då effekten körs. En kortare tid gör effekten snabbare, en längre gör den långsammare.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Infoga en kommaseparerad lista med symboler där en symbol kan vara * för alla sidor, 1-5 för ett avsnitt av sidor eller ett enskilt sidnummer.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Bestämmer bindningen av sidor i PDF-filen. Om du inte vet med dig att du behöver ändra låt standardvärdet (vänster) gälla.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Skapar miniatyrer av varje sida i PDF-filen. En del visare använder miniatyrer för navigering i dokumentet.</translation>
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
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
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
        <translation type="unfinished">Sidlayout</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished">Enkelsidig</translation>
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
        <source>No Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Beskär till sidmarginalerna</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished">Tillåter övertryckning av hela dokumentet oavsett inställningar för objekt</translation>
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
        <translation type="unfinished">Höger:</translation>
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
</context>
<context>
    <name>TabPrinter</name>
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
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished">Visa inte objekt utanför marginalerna för den tryckbara sidan</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished">Använd en annan utskriftshanterare, t. ex. kprinter eller gtklp, för att erhålla ytterligare utskriftsalternativ</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished">Sätter PostScript-nivå.
Att sätta till Nivå 1 eller 2 kan skapa mycket stora filer</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Ett sätt att stänga av några av de gråtoner som består av cyan, gult och magenta för att istället använda svart. Underfärgsborttagning påverkar huvudsakligen delar av bilder som är neutrala och/eller mörka toner som är nära grått. Användning av detta kan förbättra utskriften av vissa bilder och kräver experimenterande med testutskrifter från fall till fall. Underfärgsborttagning minskar risken för övermättnad av CMG-färger.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Tillåter att tonplattor konverteras till sammansatta färger. Om du inte tänker skriva ut färger som tonplattor på ett tryckeri bör du helst lämna denna aktiverad.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished">Tillåter övertryckning av hela dokumentet oavsett inställningar för objekt</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">Gör det möjligt att bädda in ICC-profiler i utskriftsströmmen när färghantering är aktiverad</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished">Möjliggör för dig att uttryckligen ange mediastorlek för PostScript-filen. Bör ej användas om det inte uttryckligen krävs av tryckeriet.</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Fil</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Allt</translation>
    </message>
</context>
<context>
    <name>TabPrinterBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation type="unfinished">Vald skrivare</translation>
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
        <translation type="unfinished">Alternativ</translation>
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
        <translation type="unfinished">Skriv ut i färg om tillgängligt</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Skriv ut i gråskala</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Sida</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Spegelvänd sida(or) horisontellt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Spegelvänd sida(or) vertikalt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished">Ange storlek på media</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Beskär till sidmarginalerna</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Färg</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished">Använd underfärgsborttagning</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished">Omvandla tonplattor till processfärger</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Använd ICC-profiler</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Allmänt</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="unfinished">Skriv ut normalt</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="unfinished">Skriv ut separationer</translation>
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
        <translation type="unfinished">Höger:</translation>
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
        <translation>Teckensnitt:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Storlek:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Fyllningsfärg:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Färg konturlinje:</translation>
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
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Att lära utan att göra är som att älska utan att röra</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Färg på &amp;linje:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Skuggning:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Fyllnadsfärg:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>S&amp;kuggning:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Linjestil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linje&amp;bredd:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Linjes&amp;til:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Pilar:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Början:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Avslutning:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri storleksförändring</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horisontell storleksförändring:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikal storleksförändring:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Storleksförändra bilden till ramstorlek</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Behåll p&amp;roportioner</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>F&amp;yllningsfärg:</translation>
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
        <translation>&amp;Steglängd:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Egenskaper för textramen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Egenskaper för bildram</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Egenskaper för geometriska figurer</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Standardvärden för förstoringsgrad</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Egenskaper för linjeritning</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Egenskaper för polygonritning</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Teckensnitt i nya textramar</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Teckengrad för nya textramar</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Färg på teckensnitt</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Antal spalter i textramen</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Spaltmellanrum mellan spalter i textramen</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Teckensnittsexempel</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Bildramar tillåter bilder att storleksförändras till vilken storlek som helst</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Horisontell storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Vertikal storleksförändring av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Bibehåll proportionerna vid storleksförändring</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Bilder i bildram storleksförändras till ramens storlek</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automatiskt storleksförändrade bilder bibehåller ursprungliga proportioner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Bildramens fyllnadsfärg</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Fyllnadsfärgens mättnad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Färg på formens konturlinje</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Linjefärgens mättnad</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Formernas fyllnadsfärg</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Formernas linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Formernas linjebredd</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Minsta tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Största tillåtna förstoringsgrad</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Ändring av förstoringsgrad vid varje zoomning</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Färg på linjer</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Färgmättnad</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Stil på linjer</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Linjebredd</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Fyllnadstecken vid tab:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tab-bredd:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Använd inbäddad maskningskurva</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Förhandsvisning</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Förhandsvisning med full upplösning</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Förhandsvisning med normal upplösning</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Förhandsvisning med låg upplösning</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Anpassad:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Anpassad: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Teckenfärg:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Skuggning:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Konturlinje:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestreck</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Understrykning</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Inget</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Text</translation>
    </message>
    <message>
        <source>Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="unfinished">Linjer</translation>
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
        <translation>Nedsänkt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Förskjutning:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Storleksförändring:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Upphöjt</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Förskjutn&amp;ing:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>S&amp;torleksförändring:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitäler</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Stor&amp;leksförändring:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Förskjutning ovanför teckensnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativ storlek för upphöjd text jämfört med normal teckengrad</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Förskjutning nedanför teckensnittets baslinje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativ storlek för nedsänkt text jämfört med normal teckengrad</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativ storlek för kapitäler jämfört med normal teckengrad</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understruken</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Förskjutning:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automatiskt</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linjebredd:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Överstruken</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatiskt &amp;radavstånd</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Radavstånd:</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Procentuell ökning av radavstånd baserat på teckengraden </translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Förskjutning under baslinjen för teckensnittet uttryckt i procent av teckensnittets nerstapel</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Linjebredd uttryckt i procent av teckengrad</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Förskjutning ovan baslinjen för teckensnittet uttryckt i procent av teckensnittets uppstapel</translation>
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
        <translation type="obsolete">Inget</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestreck</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Understrykning</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Anpassad</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Fyllningstecken:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Anpassad:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Anpassad: </translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Inget</translation>
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
        <translation>Hitta din webbläsare</translation>
    </message>
    <message>
        <source>External Web Browser Failed to Start</source>
        <translation>Extern webbläsare kunde inte starta</translation>
    </message>
    <message>
        <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences</source>
        <translation>Scribus kunde inte starta extern webbläsare %1. Vänligen kontrollera inställningarna</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Översikt</translation>
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
        <translation>Lösgör objekt</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Sida </translation>
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
        <translation>Automatiskt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Förskjutning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Radbredd</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Lägg till vertikal hjälplinje</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Lägg till horisontell hjälplinje</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Ta bort vertikal hjälplinje</translation>
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
        <translation type="unfinished">Urval</translation>
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
        <translation type="unfinished">Använd mallsida</translation>
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
        <translation type="unfinished">Ta bort lager</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation type="unfinished">Lyft upp lager</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation type="unfinished">Sänk ner lager</translation>
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
    <message>
        <source>Enable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Ån&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo: %1</source>
        <comment>f.e. Redo: Move</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">Gör &amp;om</translation>
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
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Action History</source>
        <translation>Historik över händelser</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Visa endast markerade objekt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>Ån&amp;gra</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Gör &amp;om</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation>Ursprungligt tillstånd</translation>
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
        <translation>Minsta marginal för sidstorlek %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Använd skrivarmarginaler</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Vä&amp;lj skrivare:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marginaler</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Höger:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>Övers&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Vä&amp;nster:</translation>
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
        <translation>&amp;Avbryt</translation>
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
        <translation type="obsolete">Verktyg</translation>
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
        <translation type="obsolete">Egenskaper...</translation>
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
        <translation type="obsolete">PDF-verktyg</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="obsolete">Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="obsolete">Textfält</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="obsolete">Kryssruta</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="obsolete">Radioknapp</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="obsolete">Lista</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="obsolete">Infoga PDF-fält</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="obsolete">Länk</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="obsolete">Infoga PDF-anteckning</translation>
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
        <translation>Importera endast text</translation>
    </message>
    <message>
        <source>Open</source>
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
        <translation>Kom ihåg koppling mellan filändelse och importfilter och fråga inte efter importfilter för denna filtyp.</translation>
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
        <translation>Öppn&amp;a</translation>
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
        <translation>Översätter template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopiera en existerande template.xml till en fil kallad template.lang_COUNTRY.xml (använd samma landskod som finns i qm-filen för ditt språk). För till exempel en finsk template.xml skriver du template.fi.xml. Kopian måste sparas i samma katalog som den ursprungliga template.xml för att Scribus ska hitta den.</translation>
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
        <translation type="unfinished">Ta bort</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Avbryt</translation>
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
