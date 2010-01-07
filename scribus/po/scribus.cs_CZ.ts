<!DOCTYPE TS><TS>
<defaultcodec></defaultcodec>
<context>
    <name></name>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;name&quot;]) -&gt; float

Vráti velikost písma v bodech rámce &quot;name&quot;. Jestliže je
vybrán nějaký text, vrátí velikost prvního znaku výběru.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Vrátí seznam se jmény všech barev v dokumentu. Jestliže není žádný dokument
otevřen, vrátí seznam implicitních barev.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Zobrazí &quot;Nový dokument&quot; dialogové okno a vytvoří nový dokument poté, co
uživatel potvrdí nastavení. Nic nevytvoří, jestliže uživatel okno zruší. Vrátí true,
pokud je dokument vytvořen.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Vrátí jméno výplňové barvy objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Posune objekt &quot;name&quot; relativně o dx a dy vůči aktuální pozici. Vzdálenosti
jsou vyjádřeny v délkových jednotkách dokumentu (viz konstanty UNIT).
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt. Jestliže &quot;name&quot;
patří do nějaké skupiny, je posunuta celá skupina.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

V případě false zruší překreslování stránek, jinak povolí. Tato změna přetrvá
i po ukončení skriptu, takže se ujistěte, že zavoláte setRedraw(true) v nejvyšší
úrovni skriptu.</translation>
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

Vytvoří nový čtyřúhelník na aktuální stránce a vrátí jeho jméno. X, Y, W a H koordináty
jsou dány ve zvolených měrných jednotkách dokumentu (viz konstanty UNIT*).
&quot;name&quot; musí být unikátní řetězec, protože slouží jako identifikátor. Jestliže není
&quot;name&quot; zadáno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError, když se pokusíte zduplikovat jméno.</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Nastaví gradient výplně objektu &quot;name&quot; na specifikovaný typ. Specifikace barev
je stejná jako v setFillColor() a setFillShade(). Dostupné gradienty viz
konstanty FILL_&lt;typ&gt;.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Zapíše řetězec &quot;string&quot; do stavového řádku Scribusu. Text musí být kódován
UTF8 nebo unicode (např. unicode(&quot;text&quot;, &quot;iso-8859-2&quot;)).</translation>
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

May raise IndexError if the page number is out of range</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).</translation>
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
        <translation>newDocument(size, margins, orientation, firstPageNumber, unit, pagesType, firstPageOrder) -&gt; bool

Creates a new document and returns true if successful. The parameters have the following meaning:

size = A tuple (width, height) describing the size of the document. You can
use predefined constants named PAPER_&lt;paper_type&gt; e.g. PAPER_A4 etc.

margins = A tuple (left, right, top, bottom) describing the document margins

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

example: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS, PAGE_4, 3, 1)

May raise ScribusError if is firstPageOrder bigger than allowed by pagesType.</translation>
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
        <translation>getFont([&quot;name&quot;]) -&gt; string

Vrátí jméno písma textového rámce &quot;name&quot;. Jestliže je v rámu vybraný
nějaký text, pak vrátí písmo prvního znaku výběru.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

Vrátí délku textu textového rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;name&quot;]) -&gt; string

Vrátí onsah textového rámce &quot;name&quot;. Jestliže je vrámu nějaký výběr, pak
je výsledkem obsah výběru. Vrátí celý text, ne jen viditelný.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;name&quot;]) -&gt; string

Vrátí obsah textového rámce včetně kompletního textu všech zřetězených
rámů. Jestliže existuje nějaký výběr, pak je výsledkem obsah výběru.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Vrátí velikost řádkování textového rámce &quot;name&quot; vyjádřené v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Vrátí velikost mezisloupcové mezery textového rámce &quot;name&quot; vyjádřenou v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

Vrátí počet sloupců textového rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Vyplní obsah textového rámce &quot;name&quot; textem &quot;text&quot;. Text musí být v UTF8
kódování - použijte např. unicode(text, &apos;iso-8859-2&apos;). Viz FAQ.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;font&quot;, [&quot;name&quot;])

Nastaví písmo &quot;font&quot; textového rámce &quot;frame&quot;. Jestliže v rámci existuje
výběr, je písmo aplikováno pouze na něj.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, jestliže písmo neexistuje.</translation>
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

Nastaví velikost písma textového rámce &quot;name&quot; na velikost &quot;size&quot;. Velikost
je vyjádřena v bodech. Jestliže je v rámu nějaký výběr, pak je změna
aplikována pouze na něj. Velikost musí být v intervalu &lt;1; 512&gt;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimlu ValueError, jestliže je velikost mimo interval.</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(size, [&quot;name&quot;])

Nastaví řádkování textového rámu &quot;name&quot; na velikost &quot;size&quot;. Velikost
je uváděna v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, jestliže je velikost mimo rozsah.</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(size, [&quot;name&quot;])

Nastaví mezisloupcovou mezeru textového rámce &quot;name&quot; na velikost &quot;size&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, jestliže není velikost kladná hodnota.</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;name&quot;])

Nastaví počet sloupců textového rámce &quot;name&quot; na počet &quot;nr&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, jetsliže je &quot;nr&quot; menší něž 1.</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(align, [&quot;name&quot;])

Nastaví zarovnání textu rámce &quot;name&quot;. Jestliže není &quot;name&quot; uvedeno, použije
vybraný objekt. &quot;align&quot; by měla být jedna z předdefinovaných konstant ALIGN_*.

Může vyvolat výjimku ValueError, jestliže je &quot;align&quot; nepodporované číslo.</translation>
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

Označí jako vybraný počet &quot;count&quot; znaků v textovém rámci &quot;name&quot; od pozice
&quot;start&quot;. První znak rámce má pozici 0. Jestliže je &quot;count&quot; nula, výběr je zrušen.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku IndexError, jestliže je výběr mimo text rámce.</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;name&quot;])

Smaže text z textového rámce &quot;name&quot;. Jestliže je v rámu nějaký výběr,
pak je smazán pouze on. Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Nastaví barvu textu rámce &quot;name&quot; na barvu &quot;color&quot;. Jestliže v rámci
existuje výběr, pak je barva aplikována pouze na něj.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Nastaví barvu &quot;color&quot; tahu textu v textovém rámci &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
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

Nastaví stín textu v textovém rámci &quot;name&quot; na hodnotu &quot;shade&quot;. Jetsliže
v rámci existuje výběr, pak je aplikován pouze na něj. Stín musí být celé
číslo z intervalu &lt;1; 100&gt;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

zřetězí dva textové rámce. Rámec &quot;fromname&quot; je před rámcem &quot;toname&quot;.
&quot;toname&quot; musí být prázdný a ještě neslinkovaný textový rámec.

Může vyvolat výjimku ScribusException, jestliže jsou porušena pravidla řetězení.</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;name&quot;)

Odstraní objekt &quot;name&quot; ze zřetězených textových rámců. Např. sekvence
a-&gt;b-&gt;c bude a-&gt;c po vykonání unlinkTextFrames(b)

Může vyvolat výjimku ScribusException, jestliže jsou porušena pravidla zřetězení.</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;])

Převede textový rámec &quot;name&quot; na obrys.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>getColor(&quot;name&quot;) -&gt; tuple

Vrátí CMYK složky barvy &quot;name&quot; daného dokumentu. Jestliže není otevřený
žádný dokument, vrátí složky implicitní barvy &quot;name&quot;.

Může vyvolat výjimky NotFoundError (jestliže barvu nenajde) a
ValueError (chybné parametry).</translation>
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

Změní barvu &quot;name&quot; na specifikované CMYK hodnoty &quot;c&quot;, &quot;m&quot;, &quot;y&quot;, &quot;k&quot;.
Každá z barevných komponenty musí být z intervalu &lt;0; 255&gt;.

Může vyvolat výjimky NotFoundError (barva neexistuje) a ValueError
(chybné parametry).</translation>
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

Smaže barvu &quot;name&quot;. každý výskyt této barvy je nahrazen  barvou &quot;replace&quot;.
Jestliže není &quot;replace&quot; uvedena, zamění mazanou barvu transparentní 
průhlednou &quot;None&quot;.
Jestliže není otevřený žádný dokument pracuje deleteColor s imlicitní
barevnou množinou. &quot;replace&quot; potom nemá žádnou funkčnost.

Může vyvolat výjimky NotFoundError (barva neexistuje) a ValueError
(chybné parametry).</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurrence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">replaceColor(&quot;name&quot;, &quot;replace&quot;)

Každý výskyt barvy &quot;name&quot; je nahrazen barvou &quot;replace&quot;.
Může vyvolat výjimky NotFoundError (barva neexistuje) a ValueError
(chybné parametry).</translation>
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
        <translation>messageBox(&quot;caption&quot;, &quot;message&quot;, icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
                   button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; integer

Zobrazí message box s titulkem &quot;caption&quot;, textem &quot;message&quot;, ikonou &quot;icon&quot;
a několika tlačítky. Implicitně není nastavena žádná ikona a zobrazí se jediné
tlačitko (OK). Povinné jsou tedy pouze parametry &quot;caption&quot; a &quot;message&quot;.
&quot;message&quot; může obsahovat jednoduché HTML značky.
Vrátí číslo tlačítka, které uživatel stisknul. Tlačítka jsou číslována od 1.
Jednotlivé typy ikon a tlačitek mají své předdefinované konstanty. Jsou to
BUTTON_* a ICON_*. Existují také dvě konstanty, které mohou být použity
v binárním OR s BUTTON konstantami:
    BUTTONOPT_DEFAULT nastaví příznak &quot;stisk enteru stiskne tlačítko&quot;
    BUTTONOPT_ESCAPE nastaví příznak &quot;stisk escape stiskne tlačítko&quot;
Příklady:
    result = messageBox(&apos;Script failed&apos;,
                                   &apos;This script only works when you have a text frame selected.&apos;,
                                   ICON_ERROR)
    result = messageBox(&apos;Monkeys!&apos;, 
                                   &apos;Something went ook! &lt;i&gt;Was it a monkey?&lt;/i&gt;&apos;,
                                   ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                                   BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)
Konstanty: BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, 
    BUTTON_IGNORE, BUTTON_NO, BUTTON_NOALL, BUTTON_OK,
    BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
    ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.</translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; string

Zobrazí jednoduchý dialog, do kterého uživatel zadává hodnotu,
kterou vráti skriptu jako string. Okno má titulek &quot;caption&quot;,
text &quot;message&quot; a může zobrazit implicitní hodnotu &quot;defaultvalue&quot;.
Např.: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Zavře aktuální dokument bez výzvy k uložení.

Může vyvolat výjimku NoDocOpenError, kdyý enní žádný dokument otevřen</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Vrátí true, když je otevřený jakýkoli dokument.</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Otevře dokument &quot;name&quot;.

Může vyvolat výjimku ScribusError, když dokument nejde otevřít.</translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation>saveDoc()

Uloží aktuální dokument pod platným jménem. Vrátí true v případě úspěchu.
Jestliže nebyl dokument ještě niky uložen, zobrazí se &quot;Uložit jako&quot; okno.
Jestliže ukladání selže, neexituje žádný způsob, jak toto zjistit.</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;)

Uloží aktuální dokument pod jménem &quot;name&quot;. &quot;name&quot; může být jak celá, tak
relativní cesta souborového systému.

Může vyvolat výjimku ScribusError, jestliže ukládání selže.</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Nastaví okraje dokumentu - levý, pravý, horní a spodní okraj je zadáván
v aktuálních měrných jednotkách dokumentu. Viz konstanty UNIT_typ.</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Změní měrné jednotky dokumentu. Možná nastavení jsou definována jako
konstanty UNIT_*.

Může vyvolat výjimku ValueError při nepodporovaném typu jednotky.</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (Scribus unit constant)

Vrátí typ měrné jednotky dokumentu. Návratová hodnota je jednou z UNIT_*
konstant: UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;)

Nahraje styly odstavce z dokumentu &quot;filename&quot; do dokumentu aktuálního.</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(facingPages, firstPageLeft)

Nastaví typ dokumentu. Parametr facingPages může být FACINGPAGES (dvoustrany)
nebo NOFACINGPAGES (jedna strana). Jestliže chcete, aby dokument začínal
levou stranou nastavte firstPageLeft na FIRSTPAGELEFT, jinak na FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Vrátí jméno barvy linek objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

Vrátí tloušťku linky objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer

Vrátí stín barvy objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Vrátí typ spojení linek objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Typy spojení: JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Vrátí typ ukončení linky objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Typy jsou: CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Vrátí styl linky objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Styly jsou: LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Vrátí stín výplně objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Vrátí tuple s velikostmi obrázku rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string

Vrátí jméno souboru (obrázku) daného rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (width,height)

Vrátí velikost (tuple) s velikostí rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Velikost je vyjádřena v aktuálních měrných jednotkách (viz UNIT_typ).</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;name&quot;]) -&gt; integer

Vrátí rotaci objektu &quot;name&quot;. Hodnota je vyjádřena ve stupních. Otočení ve směru
hodinových ručiček je kladné.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Vrátí seznam, který obsahuje jména všech objektů na aktuální stránce.</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;name&quot;])

Přesune objekt &quot;name&quot; na nové místo. Paramety x a y jsou vyjádřeny v aktuálních
měrných jednotkách dokumentu (viz konstanty UNIT).
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Jestliže onjekt &quot;name&quot; patří do skupiny onjektů, je posunuta celá skupina.</translation>
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

Otočí relativně objekt &quot;name&quot; o &quot;rot&quot; stupňů. Střed otáčení je aktuální bod otáčení,
implicitně horní levý bod. Kladné hodnoty otáčí po směru hodinových ručiček.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;])

Změní velikost objektu &quot;name&quot; na danou šířku &quot;width&quot; a výšku &quot;height&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

Vrátí jméno vybraného objektu. Jestliže je zadáno &quot;nr&quot;, pak indikuje,
jaký objekt z výběru vrátí. 0 znamená první objekt atd.</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Vrátí počet objektů ve výběru.</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;)

Zařadí objekt &quot;name&quot; do výběru.</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Zruší výběr všech objektů v celém dokumentu.</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list)

Seskupí objekty vyjmenované v seznamu &quot;list&quot;. Jestliže není seznam zadán,
použijí se vybrané objekty.</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;)

Zruší seskupení objektů, do kterého patří objekt &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
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

Změní velikost seskupených objektů, kam objekt &quot;name&quot; patří. Hodnoty &quot;factor&quot;
větší než 1 zvětšují a naopak. Např. 0.5 znamená 50%, 1.5 150% atd. &quot;factor&quot;
musí být větší než 0.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError při chybném nastavení &quot;factor&quot;.</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Nahraje obrázek &quot;picture&quot; do obrázkového rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku WrongFrameTypeError, není-li objekt obrázkovým rámem</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;name&quot;])

Nastaví velikost obrázku v obrázkovém rámu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Číslo 1 znamená 100%.

Může vyvolat výjimku WrongFrameTypeError jestliže rámec není obrázkový</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

Jestliže je objekt &quot;name&quot; zamčený, tak jej odemkne a naopak.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Vrátí true jestliže je objekt zamčený.</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

Vrátí true kdyý je objekt &quot;name&quot; zamčený.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Vrátí seznam se jmény dostupných písem.</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list of tuples

Vrátí více informací o dostupných písmech. Seznam obsahuje tupple:
[(Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Vrátí seznam se jmény všech vrstev.</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;)

Přepne dokument na specifikovanou vrstvu &quot;name&quot;.

Může vyvolat výjimky NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Vrátí jméno aktuální vrstvy.</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Přesune objekt &quot;name&quot; do vrstvy &quot;layer&quot;. Vrstva musí existovat.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;layer&quot;, visible)

Nastaví jestli má být vrstva &quot;layer&quot; viditelná nebo nemá. Je-li &quot;visible&quot;
false, bude vrstva neviditelná.

Může vyvolat výjimku NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;layer&quot;, printable)

Nastaví jestli má být vrstva &quot;layer&quot; tisknutelná nebo nemá. Je-li &quot;printable&quot;
false, nebude vrstva tisknuta.

Může vyvolat výjimku NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;layer&quot;)

Smaže vrstvu &quot;layer&quot;. Neudělá nic, jestliže je to poslední vrstva dokumentu
nebo vrstva neexistuje.

Může vyvolat výjimku NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer)

Vytvoří novou vrstvu se jménem &quot;layer&quot;.

Může vyvolat výjimku ValueError v případě chyby.</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Vrátí řetězec s kódem jazyka, ve kterém Scribus běží (viz přepínač --lang xx).</translation>
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

Vytvoří novou elipsu na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
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

Vytvoří novoý obrázkový rám na aktuální stránce dokumentu a vrátí jeho jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
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

Vytvoří nový textový rámec na aktuální stránce dokumentu a vrátí jeho jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
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

Vytvoří novou linku na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
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

Vytvoří novou lomenou linku na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů má tvar: [x1, y1, x2, y2, ..., xn, yn].

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
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

Vytvoří nový mnohoúhelník na aktuální stránce dokumentu a vrátí jeho jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů objeku má tvar: [x1, y1, x2, y2, ..., xn, yn] a jsou nutné alespoň
tři body. Mnohoúhelník je automaticky uzavřen, takže není třeba zadávat poslední
bod jako první.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
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

Vytvoří novou Bezierovou křivku na aktuální stránce dokumentu a vrátí jeho jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů objeku má tvar: [x1, y1, kx1, ky1, x2, y2, kx2, ky2, ..., xn, yn, kxn, kyn].
x a y jsou koordináty bodů, kx a ky jsou koordináty řídícího bodu křivky.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
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

Vytvoří nový text na křivce na aktuální stránce dokumentu a vrátí jeho jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Text na křivce vyznikne ze dvou objektů - textového rámce &quot;textbox&quot; a Bezierovské
křivky &quot;beziercurve&quot;.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku NotFoundError v případě neexistujících objektů.</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;])

Smaže objekt &quot;name&quot;. Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Povolí/zakáže vlastnost &quot;Text obtéká okolo rámu&quot; objektu &quot;name&quot;.
Jestliže je &quot;state&quot; true, vlastnost povolí a naopak. V případě, že &quot;state&quot; není
zadáno, stav se obrátí.</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; bool

Vrátí příznak, zda objekt &quot;name&quot; v dokuemntu existuje. Když není &quot;name&quot;
zadáno, vrátí true, jestliže je nějaký objekt vybrán.</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Aplikuje styl &quot;style&quot; na objekt &quot;name&quot;. Jestliže není &quot;name&quot; uvedeno,
použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Vrátí seznam všech jmen stylů odstavce v dokumentu.</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Vrátí číslo aktuální stránky dokumentu. Stránky jsou číslovány od 1, přičemž nezáleží
na nastaveném čísle první stránky.</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>Překreslí/obnoví všechny stránky.</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>Uloží aktuální stránku jako EPS do souboru &quot;name&quot;.

Může vyvolat výjimu ScribusErro, dojde-li k chybě.</translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation>deletePage(nr)

Smaže zadanou stránku. Nedělá nic, jestliže dokument obsahuje jedinou stránku.
Stránky jsou číslovány od 1, přičemž nezáleží na nastaveném čísle první stránky.

Může vyvolat výjimku IndexError, jestliže není &quot;nr&quot; číslo existující stránky</translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation>gotoPage(nr)

Nastaví stránku &quot;nr&quot; jako aktuální. Pozor - procedura neposune zobrazení stránky
uživateli, pouze přepne kontext Scripteru (t.j. na jaké stránce budou vykonávány
příkazy).

Může vyvolat výjimku IndexError, jestliže není &quot;nr&quot; číslo existující stránky.</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>Vrátí počet stránek dokumentu.</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Vrátí seznam s pozicemi horizontálních vodítek. Hodnoty jsou v aktuálních
měrných jednotkách. Viz konstanty UNIT.</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(list)

Nastaví horizontální vodítka. Vstupní parametr je seznam jejich pozicí
v aktuálních měrných jednotkách (viz konstanty UNIT).
Např.:
     setHGuides(getHGuides()) + [200.0, 210.0] # prida voditka
     setHGuides([90, 250]) # smaze stara a nastavi nova voditka</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>Viz getHGuides().</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>Viz setHGuides().</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Vrátí tuple s rozměry stránky v aktuálních měrných jednotkách. Viz konstanty UNIT
a getPageMargins()</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; list

Vrátí seznam tuple objektů na aktuální stránce: (jmeno, typ, poradi).
Např.: [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)], což znamená, že objekt &quot;Text1&quot;
je textový rámec (4) a je první v pořadí na stránce...</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Nastavý výplňovou barvu &quot;color&quot; objektu &quot;name&quot;. &quot;color&quot; je jméno
jedné z definovaných barev. Jestliže není &quot;name&quot; uvedeno, použije 
vybraný objekt.</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Nastaví barvu &quot;color&quot; linek objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(width, [&quot;name&quot;])

Nastaví šířku linky objektu &quot;name&quot; na hodnotu &quot;width&quot;. &quot;width&quot; musí
být v intervalu &lt;0.0; 12.0&gt; a je v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolatvýjimku ValueError, když není hodnota v intervalu.</translation>
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

Nastaví stín linek objektu &quot;name&quot; na hodnotu &quot;shade&quot;. &quot;shade&quot; musí
být celé číslo z intervalu &lt;0; 100&gt;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, když je hodnota mimo interval.</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Nastaví typ spojení linek objektu &quot;name&quot; na styl &quot;join&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz předdefinované konstanty JOIN_*.</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;name&quot;])

Nastaví styl konce linek objektu &quot;name&quot; na styl &quot;cap&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz předdefinované konstanty CAP_*.</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;])

Nastaví styl linky objektu &quot;name&quot; na styl &quot;style&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz předdefinované konstanty LINE_*.</translation>
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

Nastaví stín výplně objektu &quot;name&quot; na hodnotu &quot;shade&quot;.
&quot;shade&quot; musí být celé číslo z intervalu &lt;0; 100&gt;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, jestliže je hodnota mimo interval.</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;name&quot;])

Nastaví poloměr zaoblení rohů objektu &quot;name&quot;. Poloměr je vyjádřen v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, když je poloměr negativní.</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Nastaví styl linek objektu &quot;name&quot; na definovaný styl &quot;namedStyle&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku NotFoundError, jestliže styl neexistuje.</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Zruší předchozí nastavení progress baru. Je to vhodné použít před novým použitím P.B.</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Nastaví maximální možný počet kroků (zaplnění) progress baru.</translation>
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

Nastaví pozici progress baru na &quot;nr&quot;. Progress bar využívá koncept &quot;kroků&quot;.
Musíte zadat maximální počet kroků (progressTotal()) a nastavovat je (progressSet()).
Po použití P.B. je vhodné jej vynulovat, t.j. použít progressReset(). Viz dokumentace
Qt</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>[UNSUPPORTED!] This might break things, so steer clear for now.</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Povolí/zakáže ikonu &quot;uložit&quot; a položku menu &quot;Uložit&quot; v hlavním okně Scribusu.
Proceduru volejte, jestliže jste něco ve svém skriptu změnili, protože Scribus
tuto akci nezajistí automaticky.</translation>
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

May raise WrongFrameTypeError.</translation>
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
May raise ValueError if the layer name isn&apos;t acceptable.</translation>
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
May raise ValueError if an empty sample or filename is passed.</translation>
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

May raise WrongFrameTypeError if the target frame is not a text frame</translation>
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

May raise WrongFrameTypeError if the target frame is not a text frame</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().</translation>
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
May raise ValueError if an invalid color name is specified.</translation>
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
        <translation></translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
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
    <message>
        <source>getFillTransparency([&quot;name&quot;]) -&gt; float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
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
        <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
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
        <source>Contributions from:</source>
        <translation>Příspěvky od:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>O &amp;Scribusu</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Autoři</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Překlady</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Vývojový tým:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Oficiální dokumentace:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Ostatní dokumentace:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Webová stránka</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Online odkazy</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Chyby a požadavky na změnu</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation type="obsolete">Mailová skupina</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Oficiální překlady a překladatelé:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Předchozí přispěvatelé:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>O Scribusu %1</translation>
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
        <translation>Ghostscript verze %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ghostscript není dostupný</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus verze %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Build ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Panel zobrazuje verzi programu, datum vytvoření a použité knihovny. Symboly C-C-T-F znamenají podporu C=littlecms C=CUPS T=TIFF F=Fontconfig. Poslední písmeno uvádí způsob vykreslení C=Cairo nebo A=Libart. Chybějící knihovny jsou zobrazeny jako *. Také je zde uvedena verze Ghostscriptu, kterou Scribus našel.</translation>
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
        <translation>Ikony projektu Tango:</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>OS/2&amp;#174;/eComStation&amp;#8482; Port:</translation>
    </message>
    <message>
        <source>Developer Blog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mailing Lists</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Soubor:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Verze:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Povoleno:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Datum vydání:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Popis:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Autor/Autoři:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licence:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: O modulech</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
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
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Návrat k u&amp;loženému</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Ad&amp;resář pro výstup...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Vložit text...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Připojit text...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Vložit obrázek...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Uložit &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Uložit stránku jako &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Uložit jako P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Na&amp;stavení dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Tisk...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Konec</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Reži&amp;m hromadných akcí</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Vybr&amp;at vše</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Zrušit výběr</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hledat/Nahradit...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Upravit obrázek...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Barvy...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Styly odstavce...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Styly l&amp;inek...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Vzorové stránky...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>N&amp;astavení...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Jiný...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>V&amp;levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>Na &amp;střed</translation>
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
        <translation>&amp;Vynucené</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Normální</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>Po&amp;dtržení</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Podtrhnout &amp;slova</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>Př&amp;eškrtnuté</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Verzálky</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapitálky</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Horní index</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;Dolní index</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Stín</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Efekty obrázku</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabelátory...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Seskupit</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Zrušit seskupení</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Je &amp;zamčeno</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Velikost je zamčena</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Přesunout do nej&amp;nižší vrstvy</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Přesunout do nej&amp;vyšší vrstvy</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Přesunout o &amp;vrstvu níž</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Přesunout o v&amp;rstvu výš</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>&amp;Poslat do zápisníku</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>Vl&amp;astnosti...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Obrázek je vi&amp;ditelný</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Aktualizovat obrázek</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Přizpůsobit rámec obrázku</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Rozšířené vlastnosti obrázku</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Nízké rozlišení</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>N&amp;ormální rozlišení</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Plné rozlišení</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Je PDF &amp;záložkou</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Je PDF &amp;anotací</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Vlast&amp;nosti anotace</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Vlastnosti &amp;pole</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Upravit &amp;tvar...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Připojit &amp;text ke křivce</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Odpojit text od křivky</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Kombinovat mnohoúhelníky</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>&amp;Rozdělit mnohoúhelníky</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Beziérova křivka</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Obrázkový rámec</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Mnohoúhelník</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textový rámec</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Znak...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Výplňový text</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Vložit...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;Import...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Smazat...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopírovat...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Přesunout...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Použít vzorovou stránku...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>O&amp;vládání vodítek...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Vlastnosti stránky...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;Přizpůsobit oknu</translation>
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
        <translation>M&amp;iniatury</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Zobrazit o&amp;kraje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Zobrazit &amp;rámce</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Zobrazit &amp;obrázky</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Zobrazit &amp;mřížku</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Zobrazit &amp;vodítka</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Zob&amp;razit řetězení textu</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Zobrazit řídicí znaky</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke stránce</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>M&amp;agnetická mřížka</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Magnetická vodítka</translation>
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
        <translation>&amp;Uspořádat stránky</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Vzdálenosti</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Historie akcí</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Předtisková kontrola</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Zarovnat a rozmístit</translation>
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
        <translation>Vybrat objekt</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>&amp;Tabulka</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linka</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Linka r&amp;ukou</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Otočit prvek</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zvětšit nebo zmenšit</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zvětšit</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zmenšit</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Upravit obsah rámce</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Upravit text...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Propojit textové rámce</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Zrušit propojení textových rámců</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Barevná pipeta</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopírovat vlastnosti objektu</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Upravit text v zápisníku</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Vložit textový rámec</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Vložit obrázkový rámec</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vložit tabulku</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Vložit tvar</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Vložit mnohoúhelník</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Vložit linku</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Vložit Beziérovu křivku</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Vložit linku kreslenou od ruky</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Správa &amp;obrázků</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Dělení slov v textu</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Zrušit dělení slov textu</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Generovat obsah</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>O &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Tipy pro nástroje</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;manuál...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Chytré dělení textu</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Nedělitelná pomlčka</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Nedělitelná &amp;mezera</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Číslo &amp;stránky</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Nová linka</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Zalomení rámce</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Zalomení sloupce</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registrovaná obchodní známka</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Obchodní známka</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Odrážka</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>En dash</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En dash</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Figure dash</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostrof</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Přímé dvojité</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Levé jednoduché</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Pravé jednoduché</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Levé dvojité</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Pravé dvojité</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Jednoduché obrácené</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Dvojité obrácené</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Jednoduché levé Guillemet (francouzské)</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Jednoduché pravé Guillemet (francouzské)</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Dvojité levé Guillemet (francouzské)</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Dvojité pravé Guillemet (framcouzské)</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Dolní jednoduché</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Dolní dvojité</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>Asijské levé jednoduché</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>Asijské pravé jednoduché</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>Asijské levé dvojité</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>Asijské pravé dvojité</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Přepnout palety</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Přepnout vodítka</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Náhled pře&amp;d tiskem</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;Javaskripty...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Převést na vzorovou stránku...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskáda</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Dlaždice</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>O &amp;modulech</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Více informací...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Tisk povolen</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Překlopit &amp;vodorovně</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Překlopit &amp;svisle</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Zobrazit pravítka</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Obrys</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Lomítko</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Middle Dot</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>En mezera</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Em mezera</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Úzká mezera</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Široká mezera</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Střední mezera</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Vlasová mezera</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Vložit chytré dělení textu</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Vložit nedělitelnou pomlčku</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Vložit nedělitelnou mezeru</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Vložit číslo stránky</translation>
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
        <translation>S&amp;tyly...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Obrys</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>O&amp;brysy</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Vložit (&amp;Absolutně)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Zobrazit sloupce textového rámce</translation>
    </message>
    <message>
        <source>&amp;400%</source>
        <translation>&amp;400%</translation>
    </message>
    <message>
        <source>&amp;Manage Images</source>
        <translation>&amp;Správa obrázků</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Zarovnat a rozmístit</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Zarovnat</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativně k:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>prvně vybranému</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>naposledy vybranému</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>stránce</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>okrajům</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>vodítkům</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>výběrům</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Zarovnat pravé strany objektů na levou stranu kotvy</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Zarovnat levé strany na pravou stranu kotvy</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Zarovnat dole</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Zarovnat pravé strany</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Zarovnat horní části objektů na spodní část kotvy</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Vystředit na svislé osy</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Zarovnat levé strany</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Vystředit na vodorovné osy</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Zarovnat spodní části objektů na horní část kotvy</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Zarovnat nahoře</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Vybrané vodítko:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Rozmístit</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Sjednotit vodorovné mezery mezi objekty</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Sjednotit vodorovné mezery mezi objekty na zadanou hodnotu</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Rozmístit pravé strany ve stejné vzdálenosti</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Rozmístit spodní části ve stejné vzdálenosti</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Rozmístit středy ve stejné vzdálenosti vodorovně</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Sjednotit svislé odsazení mezi objekty</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Sjednotit svislé mezery mezi objekty na zadanou hodnotu</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Rozmístit levé strany ve stejné vzdálenosti</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Rozmístit středy svisle ve stejné vzdálenosti</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Rozmístit horní části ve stejné vzdálenosti</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Vzdálenost:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Rozmístit objekty na určenou vzdálenost</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Nic nevybráno</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Některé objekty jsou uzamčeny.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odemknout vše</translation>
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
        <translation>Zarovnat text doleva</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Zarovnat text doprava</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Zarovnat text na střed</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Zarovnat text vyrovnaně</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Zarovnat text vynuceně vyrovnaně</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Vlastnosti pole</translation>
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
        <translation>Políčko k zaškrtnuti</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznam</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Nástrojový tip:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ohraničení</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Šířka:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>tenká</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>normální</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>široká</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Styl:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>plný</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>přerušovaný</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>podtržený</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>zkosený</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>vtlačený</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>jiný</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Jen ke čtení</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Požadované</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Neexportovat hodnotu</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Viditelnost:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Viditelné</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skryté</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Bez tisku</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Bez náhledu</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Vzhled</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text pro stisknuté tlačítko</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text pro přetočení</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikony</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Použít ikony</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Stlačené</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Přetočit</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Umístění ikon...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Zvýraznění</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertovat</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Obrysové</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Stisknout</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Víceřádkový</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Heslo</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Omezení</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Neposouvat</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Nekontrolovat pravopis</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Ověřit styl:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Ověřit</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kříž</translation>
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
        <translation>Hvězda</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Čtverec</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Standardně je zatrženo</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Upravitelné</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Volby</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Jít na</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Potvrdit formulář</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Vynulovat formulář</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importovat data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Událost:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Uvolnění myši</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Stisk tlačítka myši</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Najetí myši</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Opuštění myší</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Po přepnutí na</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Při rozmazání</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Upravit...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Odeslat na URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation type="obsolete">Odeslat údaje jako HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importovat data z:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cíl</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Do souboru:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Změnit...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Stránka:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Poz:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Akce</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Pole je naformátované jako:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Obyčejný</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Číslo</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Procento</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Čas</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formát čísla</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Desetinné:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Použít symbol měny</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Symbol měny vpředu</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formátování</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formát procent</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formát data</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formát času</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Vlastní skripty</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formát:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Stisk klávesy:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formát</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Hodnota není vyhodnocena</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Hodnota musí být větší nebo rovna:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>a menší nebo rovna:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Ověření vlastním skriptem:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Vyhodnotit</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Hodnota není vypočítána</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Hodnota je</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>součet</translation>
    </message>
    <message>
        <source>product</source>
        <translation>součin</translation>
    </message>
    <message>
        <source>average</source>
        <translation>průměr</translation>
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
        <translation>následujících polí:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Vybrat...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Vlastní výpočtový skript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Vypočítat</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Vložte sem čárkami oddělený seznam polí</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Potřebujete minimálně ikonu pro Normal, abyste mohli použít ikony pro tlačítka</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Příklad:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Změna výběru</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Písmo užité v PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Indikátor je v PDF 1.3 ignorován</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF soubory (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Obrázky (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Žádná</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Žádná</translation>
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
        <translation>Vlastnosti anotace</translation>
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
        <translation>Odkaz ven</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Odkaz ven na web</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cíl</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF dokumenty (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Z&amp;měnit...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>Stránk&amp;a:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Poz</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Použít vzorovou stránku</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Vzorová stránka:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Použít na</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;Aktuální stránka</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Sudé stránky</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Liché stránky</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Všechny &amp;stránky</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Interval stránek</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>to</source>
        <translation>po</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Použít vybranou vzorovou stránku na sudé, liché, nebo všechny stránky z tohoto rozmezí</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>Čá&amp;rové kódy...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Rozhraní &quot;Pure Postscript Barcode Writer&quot;</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Chyba při otevírání souboru: %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 nebo 13 číslic</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 číslic</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 nebo 12 číslic</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 nebo 8 číslic</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 číslic</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 číslice</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Volitelný počet znaků, číslic a symbolů: -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Volitelný počet číslic a symbolů: -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Volitelný počet číslic. ITF-14 obsahuje 14 číslic a nemá číslici kontrolní</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Volitelný počet číslic</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Volitelný počet číslic a velkých písmen</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Volitelný počet hexadecimálních číslic</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Čárový kód není kompletní</translation>
    </message>
    <message>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation>12 nebo 13 číslic s mezerami. Formát ISBN-10 vyžaduje 9 nebo 10 číslic s mezerami, ale tento standard se nedoporučuje používat po 1. lednu 2007. (Poznámka: K převodu starého ISBN-10 na nový ISBN-13 stačí uvést před prvními devíti číslicemi předponu 978-, např.  1-56592-479-7 -&gt; 978-1-56592-479. Konečné číslo se spočítá automaticky.)</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Generátor čárových kódů</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Čárový kód</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Vyberte jeden z dostupných typů čárových kódů</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Číselná reprezentace vlastního kódu. Přečtěte si níže uvedenou nápovědu</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Znovu nastavit ukázku čárového kódu</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>Vložit &amp;text do výsledného kódu</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+TAlt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Jestliže je zatrženo, bude ve výsledném kódu také jeho textová reprezentace</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Hlídat okolní místo</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Kreslit pomocné šipky, aby bylo kolem kódu dostatek místa</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Pozadí</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Barva pozadí - pod řádky kódu</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Linky</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Barva linek v čárovém kódu</translation>
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
        <translation>Barva textu a čísel</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Tipy a nápověda</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Náhled výsledného kódu. 72dpi.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>&amp;Kód:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>&amp;Vložit kontrolní součet</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Vytvořit a vložit kontrolní součet do kódu</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Vložit &amp;kontrolní číslici</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Vložit kontrolní číslici do textu kódu</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Smazat</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nová položka</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Přejmenovat</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Načíst...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Náhled</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Jméno %1 není unikátní.
Prosím, zvolte jiné.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Zvolte adresář zápisníku</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Zápisník (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Vyberte zápisník, který importuji</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation>&amp;Import souborů zápisníku...</translation>
    </message>
    <message>
        <source>Main</source>
        <translation>Hlavní</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Záložky</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Přesunout záložku</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Vložit záložku</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
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
        <translation>Umístění ikon</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Návrh úpravy sazby:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Jen popisky</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Jen ikony </translation>
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
        <translation>Popisky vlevo od ikon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Popisky překrývají ikony</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Měřítko:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Vždy</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Když jsou ikony příliš malé</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Když jsou ikony příliš velké</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nikdy</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Jak škálovat:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcionálně</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Neproporcionálně</translation>
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
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Vynulovat</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>Systémové profily</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Účel reprodukce (rendering)</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Implicitní barevný profil pro plné barvy na stránce</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Barevný profil, který máte vygenerován nebo dodán od výrobce zařízení.
Tento profil by měl být nastavený na váše prostředí - ne obecný (např. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Barevný profil vaší tiskárny, který máte od výrobce.
Tento profil by měl být nastavený na váše prostředí - ne obecný (např. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Implicitní účel reprodukce monitoru. Jestliže víte, proč jej změnit, zvolte
relativní kolorimetrickou transformaci nebo perceptuální (fotografickou) transformaci.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Implicitní vykreslování pro tiskárnu. Jestliže víte, proč jej změnit, zvolte
relativní kolorimetrickou transformaci nebo perceptuální (fotografickou) transformaci.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Povolit &quot;soft proofing&quot; (nátisk) založený na vybraném profilu tiskárny.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metoda zobrazení těch barev na obrazovce, které mohou být nesprávně vytištěny.
Vyžaduje to přesné profily a slouží to pouze jako varování.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Mapování černé barvy ze dvou profilů (blackpoint compensation) je způsob,
jakým lze zlepšit kontrast fotografií. Doporučeno, jestliže je máte v dokumentu.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktivovat správu barev</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Tiskárna:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Mo&amp;nitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Tis&amp;kárna:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulace tisku na obrazovce</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Označ netisknutelné barvy (&amp;gamut)</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Použí&amp;t mapování černé ze dvou profilů</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation type="obsolete">&amp;RGB obrázky:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation type="obsolete">&amp;CMYK obrázky:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Implicitní barevný profil importovaných CMYK onrázků</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Implicitní barevný profil importovaných RGB onrázků</translation>
    </message>
    <message>
        <source>&amp;RGB Images:</source>
        <translation>&amp;RGB obrázky:</translation>
    </message>
    <message>
        <source>&amp;CMYK Images:</source>
        <translation>&amp;CMYK obrázky:</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Upravit barvu</translation>
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
        <translation>RGB pro web</translation>
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
        <translation>Mřížky s dynamickými barvami</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Mřížky se statickými barvami</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-barevná mapa</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Barevný model</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Je přímou barvou</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Je &quot;registration color&quot;</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Nelze vytvořit barvu jménem &quot;%1&quot;.
Je rezervováno pro průhlednou barvu</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>Jméno barvy není jedinečné</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation>Výběrem této volby povolíte tisk na všech arších. Registrační barvy jsou použity pro tiskové značky, ořezové značky a registrační značky. Ty ovšem nebývají použity v samotné kompozici.</translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation>Výběrem této volby nastavíte tuto barvu jako přímou barvu, tedy vytvoříte další přímou barvu pro archy nebo separace. Používá se to, když např. logo nebo jiné barvy potřebují přesnou reprezentaci nebo nemohou být replikovány pomocí CMYK inkoustů. Metalické nebo fluorescentní inkousty jsou dobrým příkladem nemožnosti replikace pomocí CMYK inkoustů.</translation>
    </message>
    <message>
        <source>You cannot create a color without a name
Please give it a name</source>
        <translation>Nelze vytvořit barvu bez názvu
Zadejte prosím název</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Náhled uvidíte, pokud stisknete a uvolníte pravé tlačítko myši. Klávesa Insert vloží do výběru znak a Delete smaže naposledy vložený</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Vybrat znak:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Písmo:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Třída znaků:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Vložit znak na pozici kurzoru v textu</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Smazat aktuální výběr(y).</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Plná znaková sada</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Základní latinka</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 doplňky</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Rozšířená latinka A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Rozšířená latinka B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Obecná interpunkce</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Horní a dolní indexy</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Měny</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Symboly dopisu</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Číselné formy</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Šipky</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematické operátory</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Kreslení boxů</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blokové elementy</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometrické tvary</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Různé symboly</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Blbinky</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Varianty minuskulí</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Speciální znaky</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Řečtina</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Rozšířená řečtina</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Azbuka</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Azbuka - doplňky</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabské</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabské rozšířené A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabské rozšířené B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebrejské</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>&amp;Vložit kód:</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Zavřít tento dialog a vrátit se k úpravě textu</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Zadejte přímo čtyřčíselnou hodnotu Unicode</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Chybí znak</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Text přetéká</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekt není na stránce</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Chybí obrázek</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Obrázek má nižší DPI než %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objek onsahuje průhlednost</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt je PDF anotace nebo formulář</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekt je umístěné PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Nenalezeny žádné problémy</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Stránka </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Volné objekty</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Nalezeny problémy</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Onjekty</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problémy</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Aktuální profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorovat chyby</translation>
    </message>
    <message>
        <source>Annotation uses a non TrueType font</source>
        <translation>Anotace používá ne-TrueType písmo</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Vybrat styly</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Dostupné styly</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Provádím export...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Nelze exportovat všechny soubory:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation> Nelze exportovat soubor:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nová</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Odstranit nepoužité</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Množiny barev</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktuálí množina barev:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Uložit množinu barev</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Nahrát množinu barev</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Uložit aktuální množinu barev</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Odstranit nepoužité barvy z aktuální množiny barev</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importovat do současné množiny barvy z jiného dokumentu</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Vytvořit novou barvu v aktuální množině</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Upravit zvolenou barvu</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Vytvořit kopii vybrané barvy</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Smazat vybranou barvu</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Nastavit aktuální množinu barev jako implicitní</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Vybrat jméno</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nová barva</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Pokud je správa barev povolená, varovný trojúhelník upozorňuje, že je barva zřejmě mimo barevný rozsah vybraného barevného profilu současné tiskárny. To znamená, že barva pravděpodobně nebude vytištěna tak, jak je zobrazena na obrazovce. Přímé barvy jsou označeny červeným kolečkem. Registrační barvy jsou označeny značkou. Další tipy ohledně varování v souvislosti s barevným rozsahem naleznete v online nápovědě v sekci Správa barev.</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monochromatické</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analogické</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementární</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Dělené komplementární</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadické</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetradické (zdvojené komplementární)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Základní barva</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monochromatické světlé</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monochromatické tmavé</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>První analogická</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Druhá analogická</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>První dělená</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>Druhá dělená</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>Třetí dělená</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>Čtvrtá dělená</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>První triadická</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>Druhá triadická</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>První tetradická (opak základní)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>Druhá tetradická (úhel)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Třetí tetradická (opačný úhel)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Normal Vision</source>
        <translation>Normální vidění</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Barvoslepost</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Porucha vidění:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Kruhová paleta</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
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
        <translation>Vyberte metodu:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Úhel (0-90 stupňů):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Sloučit barvy</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Zaměnit barvy</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Sloučit vytvořené barvy s barvami dokumentu</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Zaměnit vytvořené barvy v barvách dokumentu</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Ponechat barvy nedotknuté</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Slučování barev</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Chyba:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Spouštění správce barev.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Slučování barev</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>Vy&amp;tvořit barvu...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Importovat existující barvu...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Sloučit barvy</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>Z&amp;aměnit barvy</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Rozdíl mezi vybranou a počítanou hodnotou. Více informací naleznete v dokumentaci.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Klikněte kolečkem pro získání základní barvy. Jedná se o odstín v režimu HSV.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Ukázkové barevné schéma</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Vyberte jednu z metod pro vytvoření barevného schématu. Více informací naleznete v dokumentaci.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Barvy vybraného barevného schématu</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Simulace běžných zrakových vad. Vyberte typ vady.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nová barva</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Není možné nalézt požadovanou barvu. Pravděpodobně jste vybrali černou, šedou nebo bílou. Tuto barvu není možné zpracovat.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>&amp;Barevné komponenty...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Barvoslepost (červená)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Barvoslepost (zelená)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Barvoslepost (modrá)</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Barva %1 už existuje!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Barva %1 přidána.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Kruhová paleta...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Pomocník pro nastavení barev</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Výběr barev s naukou o barvách.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Aplikovat</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Žádný</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Jedna stránka</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Dvojstrany</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3 složení</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4 složení</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Levá stránka</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Střední</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Střední levá</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Střední pravá</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Pravá stránka</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation>Normální vlevo</translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation>Normální na střed</translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation>Normální vpravo</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Pondělí</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Úterý</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>Středa</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>Čtvrtek</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>Pátek</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>Sobota</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Neděle</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Leden</translation>
    </message>
    <message>
        <source>February</source>
        <translation>Únor</translation>
    </message>
    <message>
        <source>March</source>
        <translation>Březen</translation>
    </message>
    <message>
        <source>April</source>
        <translation>Duben</translation>
    </message>
    <message>
        <source>May</source>
        <translation>Květen</translation>
    </message>
    <message>
        <source>June</source>
        <translation>Červen</translation>
    </message>
    <message>
        <source>July</source>
        <translation>Červenec</translation>
    </message>
    <message>
        <source>August</source>
        <translation>Srpen</translation>
    </message>
    <message>
        <source>September</source>
        <translation>Září</translation>
    </message>
    <message>
        <source>October</source>
        <translation>Říjen</translation>
    </message>
    <message>
        <source>November</source>
        <translation>Listopad</translation>
    </message>
    <message>
        <source>December</source>
        <translation>Prosinec</translation>
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
        <translation>Normální</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Vodorovný přechod</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Svislý přechod</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Úhlopříčný přechod</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Obrácený úhlopříčný přechod</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Kruhový přechod</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Neprůsvitnost:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Odstín:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Upravit vlastnosti barvy linek</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Barva výplně</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Sytost barvy</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Normální výplň nebo přechody</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Nastavení průhlednosti vybrané barvy</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Volný lineární přechod</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Volný kruhový gradient</translation>
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
        <source>Move Vector</source>
        <translation>Přesunout vektor</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Začátek přechodového vektoru přesuňte pomocí stisknutého levého tlačítka myši, konec vektoru pomocí pravého stisknutého tlačítka</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Volby CSV importu </translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Oddělovač položek:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Oddělovač hodnot:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>První řádek je hlavička</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Možnosti tiskárny</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Volba</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Nastavení stránky</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Všechny stránky</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Pouze sudé stránky</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Pouze liché stránky</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Zrcadlo</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientace</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Up tisk</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Stránka na list</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Stránky na list</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Tento panel zobrazuje volby CUPS při tisku. Přesné dostupné parametry zavisí na ovladači vaší tiskárny. Podporu pro CUPS si můžete ověřit v nabídce Nápověda &gt; O Scribusu. Hledejte výpis C-C-T, což znamená C=CUPS, C=littlecms a T=TIFF. Chybějící knihovna je označena hvězdičkou *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Kódování:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Přesunutí do adresáře dokumentů.
Lze uvést v Nastavení.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimovaný soubor</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Včetně &amp;písem</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Nastavit barevné složky</translation>
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
        <translation>Nastavit &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Nastavit &amp;CMYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Nastavit &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Zrušeno uživatelem</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Smazat barvu</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Smazat barvu:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Nehradit čím:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Smazat stránky</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>po:</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Smazat od:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Smazat styl</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Smazat styl:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Nehradit čím:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Chyba importu</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Chyba importu Word souboru
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Informace o dokumentu</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Název:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Klíčová slova:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Popis:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Vydavatel:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Př&amp;ispěvatelé:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formát:</translation>
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
        <translation>&amp;Odkaz:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>O&amp;blast:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>P&amp;ráva:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Da&amp;lší informace</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Osoba nebo organizace, která dokument zveřejňuje, publikuje nebo vydává</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Osoby nebo organizace, které přispěly k obsahu dokumentu</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datum přiřazené k události v životním cyklu dokumentu. Použijte RRRR-MM-DD formát podle ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Povaha dokumentu. Např. kategorie, finkce atd</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Jednoznačný odkaz na dokument. Např. ISBN nebo URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Odkaz na dokument, ze kterého tento vychází. Např. ISBN nebo URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Odkaz na související dokument např. ISBN nebo URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Rozšířený rámec obsahu dokumentu - oblast působnosti, datum anebo soudní pravomoc atd</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informace o autorských právech, licencích apod. Např. copyright, patenty nebo obchodní značka</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Osoba nebo organizace primárně odpovědná za obsah dokumentu. Toto pole může být vloženo do dokumentu Scribus jako odkaz, stejně tak do PDF jako metadata</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Název dokumentu. Toto pole může být vloženo do dokumentu Scribus jako odkaz, stejně tak do PDF jako metadata</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Popis obsahu dokumentu. Jedná se stručné shrnutí nebo abstrakt dokumentu. Je vloženo do PDF při exportu</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Námět obsahu dokumentu. Toto pole obsahuje klíčová slova, která si přejete vložit do PDF kvůli vyhledávání a indexování PDF souborů</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fyzická nebo digitální prezentace dokumentu. Neuškodí typ média nebo rozměry. Praktické jsou zde také  RFC2045, RFC2046 jako MIME typy</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Jazyk obsahu dokumentu, obvykle kód jazyka podle ISO-639 doplněný pomlčkou a kódem země podle ISO-3166, např. en-GB, fr-CH, cs-CZ</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Přidat do dokumentu sekci s číslováním stránek. Nová sekce bude přidána za aktuálně vybranou sekci.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Odstranit aktuálně vybranou sekci.</translation>
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
        <translation>&lt;b&gt;Název:&lt;/b&gt; Volitelný název pro sekci, např. Index&lt;br/&gt;&lt;b&gt;Zobrazeno:&lt;/b&gt;Vyberte, pokud chcete v této sekci zobrazit čísla stránek a pokud je v sekci alespoň jeden textový rámec.&lt;br/&gt;&lt;b&gt;Od:&lt;/b&gt; Index stránky, od které se má začít.&lt;br/&gt;&lt;b&gt;Do:&lt;/b&gt; Index stránky, u které se má skončit.&lt;br/&gt;&lt;b&gt;Styl:&lt;/b&gt; Vyberte styl pro číslování stránek.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; Index v rámci rozmezí stylu, kde se má začít, např. pokud je Start=2 a Style=a,b,c ...,potom budou čísla začínat u &quot;b&quot;. U první sekce v dokumentu to nahradí původní první číslo strany v novém okně souboru. (fixme).</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Číslo stránky je mimo použitelnou oblast</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Hodnota, kterou jste zadali, je mimo rozsah stránek v současném dokumentu (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Sekce dokumentu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Od</translation>
    </message>
    <message>
        <source>To</source>
        <translation>po</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Styl</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Zobrazeno</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Vztahuje se k</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je rodičem</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je potomkem</translation>
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
        <translation>Ano-ne</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Číslo</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Řetězec</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Žádný</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Nic</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Reálné číslo</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položek dokumentu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametr</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Vztah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Vztah k</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Automaticky přidat k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+N</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Nastavit tiskárnu</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Volby</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tyrkysová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny stránky, 1-5,
t.j. interval, nebo jediné číslo stránky.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Tisk do</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Volby...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Soubor:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternativní příkaz tisku</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Příkaz:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Interval</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Tisknout &amp;vše</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Ti&amp;sknout aktuální stránku</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>T&amp;isknout interval</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Počet kopií:</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Tisk</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Tisknout normálně</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Tisknout separace </translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Tisknout barevně, pokud lze</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Tisknout v odstínech šedé</translation>
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
        <translation>Stránka</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Zrcadlit stránky vodorovně</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Zrcadlit stránky svisle</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Nastavit velikost média</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Použít během odstranění barvy</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertovat spot barvy na process</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Použít ICC profily</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Pokročilé volby</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Náhled...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Nastaví úroveň PostScriptu.
 Nastavení na Level 1 nebo 2 způsobí vytváření velkých souborů</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Soubory PostScriptu (*.ps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Použít alternativní tiskový program, např. kprinter nebo gtklp, který nabízí další možnosti při tisku</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Způsob, jak odstranit některé odstíny šedé, které jsou tvořeny tyrkysovou, žlutou a purpurovou, a použít místo nich černou. UCR nejvíce ovlivní části obrázků, které jsou neutrální, a/nebo tmavé tóny, které se blíží šedé. Použitím lze vylepšit tisk některých obrázků, ovšem je třeba vše vyzkoušet a otestovat v konkrétních případech. UCR snižuje riziko přesycení v případě CMY inkoustů.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Povolí převod přímých barev na kompozitní. Pokud neplánujete tisk přímých barev na komerční tiskárně, je zřejmě lepší nechat tuto volbu povolenou.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Umožní vložit do tiskového proudu ICC profily, pokud je povolena správa barev</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Povolí výluční nastavení velikosti média v PostScriptu. Nedoporučuje se, pokud to nevyžaduje vaše tiskárna.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Zmenšit na okraje stránky</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Není možné získat nastavení tiskárny</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Nezobrazovat na tištěné stránce objekty přesahující okraje</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Import souboru:
%1
se nepodařil!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Chyba</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation>Importuji PostScript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Analyzuji PostScript:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Vytvářím objekty</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Konverze %1 obrázků selhala!</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Upravit styl</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Řádkování</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Jméno stylu odstavce</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Písmo vybraného textu nebo objektu</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva výplně písma</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva tahu textu</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Určuje celkovou výšku iniciál počtem řádků</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Horní odsazení odstavce</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Dolní odsazení odstavce</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabelátory a odsazení</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linky:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Vzdálenosti</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Pevné řádkování</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatické řádkování</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnat k pomocné mřížce</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Iniciály</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Vzdálenost od textu:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Náhled odstavcového stylu</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Určuje mezeru mezi iniciálou a textem</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Přepíná ukázkový text tohoto odstavcového stylu</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Jméno stylu není jedinečné</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Vyberte pro snazší čtení lehce barevných textových stylů</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Vzdálenost k účaří znaků</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Stiskněte a držte tlačítko pro výběr řádkování.</translation>
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
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Uložit a skončit</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>S&amp;končit bez uložení</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>Získat &amp;jména polí</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripty (*.js);;Všechny soubory (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Efekty obrázku</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Volby:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Odstín:</translation>
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
        <translation>Poloměr:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Hodnota:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterizovat:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Dostupné efekty</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Blur</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Jas</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Kolorovat</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertovat</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterizovat</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Zaostřit</translation>
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
        <translation>Použité efekty</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Všechny stánky</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Změnit výstupní adresář</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Výstupní adresář - místo, kde budou uloženy vaše obrázky.
Jména exportovaných souborů budou ve tvaru &apos;jmenodokumentu-cislostranky.typ&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportovat pouze aktuální stránku</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Dostupné exportní formáty</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Vyberte adresář</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Export do adresáře:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Typ obrázku:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalita:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Uložit jako obrázky</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Volby</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Rozlišení:</translation>
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
        <translation>Interval</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Aktuální stránka</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Interval</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportovat rozsah stránek</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny stránky, 1-5,
t.j. interval, nebo jediné číslo stránky.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Uložit všechny stránky</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Rozlišení obrázků
Použijte 72 dpi, jestliže je obrázek určen pro obrazovku</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kvalita obrázků - 100% je nejvyšší, 1% nejnižší</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Velikost obrázků. 100% beze změny, 200% dvakrát větší atd.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Rozšířené vlastnosti obrázku</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Ztmavit</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Zesvětlit</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Odstín</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Světlost</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Násobit</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Obrazovka</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Rozpuštění</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Overlay</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Studené světlo</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Teplé světlo</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Rozdíl</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Exkluze</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Barevný fígl</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Vypálení barvy</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Exkluze</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Režim směšování:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Neprůsvitnost:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Nepoužívat žádnou cestu</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Zavřít</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Velikost:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Bez názvu</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznámý</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Rozlišení:</translation>
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
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Barevný prostor:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="unfinished">Verze:</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Některá písma, použitá v tomto dokumentu, byla nahrazena:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> nahrazeno: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Dostupná písma</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Náhrady písem</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Dodatečné cesty</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Název písma</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Náhrada</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Dostupná písma</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Náhrady písem</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Dodatečné &amp;cesty</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Připojit...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstranit</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Název písma</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Použít písmo</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Obsažen v:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Podmnožina glyphů</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Cesta k souboru s písmem</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation>Cesty k písmům lze nastavit, jen pokud nejsou otevřené žádné dokumenty. Zavřete všechny dokumenty a potom změňte cestu pro písma v nabídce Soubor - Nastavení - Písma.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Přidat zvolené písmo do Styl, Písmo menu</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Zavřít náhled písem</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Spustit hledání</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Velikost zvoleného písma</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Uživatel</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Systém</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Ukázka bude zobrazena až po stisknutí tlačítka</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Zadejte sem text pro rychlé vyhledávání v názvech písem. Hledání ignoruje velikost znaků. Můžete použít běžné divoké masky (*, ?, [...]). Příklady: t* vypíše seznam písem začínajících na t nebo T. *bold* vypíše seznam písem obsahujících v názvu slovo bold, bolder atd.</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Náhled písem</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Rychlé hledání:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Název písma</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Podmnožina glyphů</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Přístup</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost písma:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Text, který se zobrazí</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>Na&amp;stavit</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt-T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Obnovit text</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>Připoji&amp;t</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Náhled písem</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Náhled písem...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Dialog náhledu písem</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Třídění, prohledávání a procházení písem.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Náhrady písem</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Původní písmo</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Nové písmo</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Nastavit tyto náhrady jako trvalé</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Tento dokument obsahuje některá písma, která nejsou na vašem systému instalována, vyberte prosím jejich odpovídající náhrady. Tlačítko Zrušit načítání dokumentu zastaví.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Zruší náhrady písem a zastaví načítání dokumentu.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Pokud volbu povolíte, Scribus použije tyto náhrady chybějících písem ve všech budoucích dokumentech. Později to lze zrušit v nabídce Úpravy - Nastavení - Písma.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Pokud vyberete OK a uložíte, náhrady budou pro dokument trvalé.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Pozice:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Přidat, změnit nebo odebrat zarážky barev</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Správa vodítek</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Vodorovná vodítka</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Svislá vodítka</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Přip&amp;ojit</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Zamknout vodítka</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Řádky a sloupce - automatická vodítka</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>Řá&amp;dky:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Sloupce:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>&amp;Meziřádková mezera</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Mezi&amp;sloupcová mezera</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>Odkaz na:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Stránka</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Okraje</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Výběr</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>&amp;Aktualizovat</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Nastaví v dokumentu vodítka. Správce vodítek je stále otevřený, ale změny jsou trvalé</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;Použít pro všechny stránky</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Vodítko</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Jednotka</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Náhled</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Již existuje prázdné (0.0) vodítko</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Lituji, ale není dostupný žádný manuál! Navštivte prosím: http://docs.scribus.net
kde naleznete aktuální dokumentaci.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Online nápověda Scribusu</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Obsah</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Najít</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Hledaný řetězec:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Hl&amp;edat</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>S&amp;mazat vše</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Tisk...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Hlednání ignoruje velikost písmen</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nová záložka</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Název nové záložky:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Hledat...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Hledat &amp;další</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Hledat &amp;předchozí</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Přidat záložku</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>S&amp;mazat vše</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Relevance</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Přijatelné dělení slov</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Akceptovat</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Přeskočit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Délka nejkratšího slova, které může být děleno.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximální počet po sobě následujících dělení slov.
Nula (0) funkci vypíná. Pozn. překl.: V české typografii max. 3.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Nejmenší slovo:</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Návrhy &amp;dělení</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;Automaticky dělit slova při psaní</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Po volbě &quot;Extra&quot;, &quot;Dělení slov v textu&quot; se objeví dialog,
ve kterém budou zobrazeny všechny možnosti dělení slova.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Povolí automatické dělení slov během psaní textu.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Maximální počet po sobě následujících dělení:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Informace o obrázku</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Obecné informace</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Datum/čas:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Má vložený profil:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Jméno profilu:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Má vložené cesty:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Má vrstvy:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF informace</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Umělec:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Komentář:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Uživatelův komentář:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Typ fotoaparátu:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Výrobce fotoaparátu:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Popis:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Typ skeneru:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Výrobce skeneru:</translation>
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
        <translation>Import EPS souborů</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje do dokumentu většinu EPS souborů,
přičemž převede vektorová data na objekty Scribusu.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
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
        <translation>Vložit stránku</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>před stránku</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stránku</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Stránka(y)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Vzorové stránky</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Vzorová stránka:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Přesunout objekty společně s jejich stránkou</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Vložit tabulku</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Počet řádek:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Počet sloupců:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Nový skript</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editace Java skriptů</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Upravit...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Přidat...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nový skript:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Opravdu chcete smazat tento skript?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Přidá nový skript a definuje funkci se stejným jménem. Pokud chcete tento skript použít jako skript pro &quot;otevřenou akci&quot;, ujistěte se, že nezměníte jméno funkce.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Akce</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktuální klávesa</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Zvolte klávesu pro tuto akci</translation>
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
        <translation>ctrl+</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>Žádná &amp;klávesa</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Definováno &amp;uživatelem</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Nastavit klávesu</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Sady klávesových zkratek k načtení</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Načíst</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Import...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Export...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Sady klávesových zkratek, které lze načíst</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Načíst vybranou sadu klávesových zkratek</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importovat do současného nastavení sadu klávesových zkratek</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Exportovat současné klávesové zkratky do souboru</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Znovu načíst předdefinované klávesové zkratky</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>XML soubory klávesových zkratek (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>Tato posloupnost kláves se již používá</translation>
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
        <translation>Vrstvy</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Opravdu chcete smazat také všechny objekty v této vrstvě?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Přidat vrstvu</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Vrstvu dopředu</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Vrstvu dozadu</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Upravit styly linek</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nový styl</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Opravdu chcete smazat tento styl?</translation>
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
        <translation>Šířka linky:</translation>
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
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
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
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Plná linka</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Přerušovaná linka</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Tečkovaná linka</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Čerchovaná linka</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Dvojitě čerchovaná linka</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Vybrat Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Více:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML soubor:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Odstavce:</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Správa vlastností stránky</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Přesunout objekty společně s jejich stránkou</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Okrajová vodítka</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Vzdálenost mezi vodítkem horního okraje a okrajem stránky</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Vzdálenost mezi vodítkem dolního okraje a okrajem stránky</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Uvnitř:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Vnější:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Předdefinované vzhledy:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Použít nastavení okrajů na všechny stránky</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Použít změny okrajů na všechny existující stránky v dokumentu</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Vzdálenost mezi vodítkem levého okraje a rohem stránky. Pokud je zvoleno Přilehlé strany, tento prostor může být využit pro vytvoření odpovídajících okrajů pro vazbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Vzdálenost mezi vodítkem pravého okraje a rohem stránky. Pokud je zvoleno Přilehlé strany, tento prostor může být využit pro vytvoření odpovídajících okrajů pro vazbu</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Okraje tiskárny...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importovat okraje pro vybranou velikost stránky z dostupných tiskáren.</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Upravit vzorové stránky</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Opravdu chcete smazat tuto vzorovou stránku?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Nová vzorová stránka</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Nová vzorová stránka</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Duplikovat zvolenou vzorovou stránku</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Smazat zvolenou vzorovou stránku</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Přidat novou vzorovou stránku</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Import vzorových stránek z existujícího dokumentu</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nová vzorová stránka %1</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>Nelze přejmenovat vzorovou stránku</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>Normální stránka nemůže být přejmenována.</translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>Přejmenovat vzorovou stránku</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>Nové jméno:</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Počet kopií:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vodorovné posunutí:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Svislé posunutí:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Vzdálenosti</translation>
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
        <translation>Úhel:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Délka:</translation>
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
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importovat stránku(y)</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>od 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Vytvořit stránku(y)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>od %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Importovat vzorovou stránku</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Z dokumentu:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importovat stránku(y):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>Importovat &amp;vzorovou stránku</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vložte seznam identifikátorů oddělený čárkami. Identifikátor může být * pro všechny stránky, 1-5 pro rozmezí stránek nebo číslo konkrétní stránky.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>před stránku</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>za stránku</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Chybějící písmo</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Písmo %1 není nainstalované.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Použít</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>místo</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Přesunout stránky</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopírovat stránku</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Přesunout stránku(y):</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Přesunout stránku(y)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>před stránku</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>za stránku</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>po:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Počet kopií:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometrie</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Střed otáčení:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Hladina</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Tvar:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Vzdálenost textu</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Zobrazit křivku</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Počáteční posun:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Vzdálenost od křivky:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Vložit profil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Účel reprodukce:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Levý bod</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Koncové body</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Řádkování</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Jméno vybraného objektu</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Vodorovné umístění středu otáčení</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Svislé umístění středu otáčení</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Šířka</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Výška</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotace objektu podle aktuálního středu otáčení</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Bod (počátek souřadnic), od kterého jsou 
odvozeny vzdálenosti nebo úhly rotace</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Střed otáčení vlevo nahoře</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Střed otáčení vpravo nahoře</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Střed otáčení vlevo dole</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Střed otáčení vpravo dole</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Střed otáčení uprostřed</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Překlopit vodorovně</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Překlopit svisle</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>O hladinu výš</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>O hladinu níž</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Přesunout navrch</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Přesunout dospodu</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Zamknout nebo odemknout objekt</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Zamknout nebo odemknout rozměry objektu</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation type="obsolete">Povolit nebo zakázat tisk objektu</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Písmo vybraného textu nebo objektu</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Změna šířky znaků</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Barevná sytost tahu textu</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Sytost barvy výplně písma</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Styl aktuálního odstavce</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Změnit nastavení pro levé nebo koncové body</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Styl linky</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Tloušťka linky</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Typy spojení linek</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Typy ukončení linek</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Styl linky aktuálního objektu</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Vyberte tvar rámce...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Upravit tvar rámce...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Nastavní poloměru zakulacení rohů</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Počet sloupců v textovém rámci</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Vzdálenost mezi sloupci</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Vzdálenost textu od horní části rámce</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Vzdálenost textu od spodní části rámce</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Vzdálenost textu od levé části rámce</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Vzdálenost textu od pravé části rámce</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Nastavit tabelátory v textového rámci...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Umožní nastavit jiné rozměry obrázku než rámce</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodorovné posunutí obrázku uvnitř rámce</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Svislé posunutí obrázku uvnitř rámce</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Změnit šířku obrázku</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Změnit výšku obrázku</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Použít stejnou změnu velikosti pro oba rozměry (X a Y)</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Obrázek změní velikost podle rozměru rámce</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Obrázek si zachová své proporce</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Linky buňky v tabulce</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Horní linka</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Levá linka</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Pravá linka </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Dolní linka</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Dodržet poměr</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Zdrojový profil obrázku</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Účel reprodukce obrázku</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Vlastnosti textu na křivky</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Ukazuje hladinu zvoleného objektu. 0 znamená, že je objekt nejníž</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Přepíná mezi mezisloupcovou mezerou a šířkou sloupce</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Šířka sloupce</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Obrázek</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linka</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Barvy</translation>
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
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Upravit &amp;tvar...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Zakulatit
&amp;rohy:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mezera:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabelátory...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Te&amp;xt obtéká okolo rámce</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Použít &amp;celostránkový rámec</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Použít &amp;obrysovou linku</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;yl:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-Měří&amp;tko:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-Měřít&amp;ko:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Změni&amp;t velikost na rozměr rámce</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proporcionálně</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Střed otáčení:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Typ linky:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Šíř&amp;ka linky:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Hrany:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Ukončení:</translation>
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
        <source>Hyphenation language of frame</source>
        <translation>Jazyk pro dělení slov v rámci</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Psaní zprava doleva</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Pevné řádkování</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatické řádkování</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnat k pomocné mřížce</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Aktuální X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Aktuální Y-DPI:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Počáteční šipka:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Koncová šipka:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Offset k účaří znaků</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Měřená výška znaků</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Jméno &quot;%1&quot; není jedinečné.&lt;br/&gt;Zvolte prosím jiné.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Pravidla pro vyplňování</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Lichý-sudý</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Nenulový</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Barva tahu textu nebo stínu, podle výběru. Pokud jsou vybrána obě formátování, potom jsou stejnou barvou.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Barva vybraného textu. Pokud je povolen obrys textu, tato barva bude výplňová. Pokud je povoleno stínování textu, tato barva bude nejvýše.</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation>Nechat text ve vzdálenějších rámcích plavat kolem objektu. Možnosti níže chování upřesňují.</translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Pro obtékání textových rámců pod objektem použijte místo tvaru rámce raději bounding box, který je vždy pravoúhlý. </translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Pro obtékání textových rámců pod objektem použijte druhou linku založenou na tvaru rámce. </translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Stiskněte a držte tlačítko pro výběr řádkování.</translation>
    </message>
    <message>
        <source>Enable or disable exporting of the object</source>
        <translation>Povolit nebo zakázat exportování objektu</translation>
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
        <translation>Upravit styl</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
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
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Šířka linky:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Plná linka</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Přerušovaná linka</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Tečkovaná linka</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Čerchovaná linka</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Dvojitě čerchovaná linka</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Jméno %1 není jedinečné.&lt;br/&gt;Zvolte prosím jiné.</translation>
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
        <translation>Průběh</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Celkový průběh:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Můj &amp;Modul</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Můj modul</translation>
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
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Okrajová vodítka</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Volby</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Velikost stránky dokumentu - buď standardní, nebo volitelný rozměr</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientace stránek dokumentu</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stránek dokumentu - upravitelná, je-li vybrán volitelný rozměr stránky</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Výška stránek dokumentu - upravitelná, je-li vybrán volitelný rozměr stránky</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Implicitní měrná jednotka dokumentu</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Vytvářet automaticky textové rámce, jsou-li přidány nové stránky</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Počet sloupců v automaticky vytvořených textových rámcích</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Vzdálenost mezi automaticky vytvořenými sloupci</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Implicitní jednotka:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Automatické textové rámce</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mezera:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Tento dialog znovu nezobrazovat</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Počáteční počet stránek dokumentu</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Počet stránek:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nový dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Otevřít &amp;existující dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Otevřít ne&amp;dávný dokument</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>N&amp;ový ze šablony...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Načíst dokumenty s předdefinovaným vzhledem</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Vytvořit dokument ze šablony připravené jinými uživateli nebo vámi (vhodné pro dokumenty s jednotným vzhledem).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Uzly</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Přesunout uzly</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Přesunout řídicí body</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Přidat uzly</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Smazat uzly</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Vynulovat řídicí body</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Vynulovat tento řídicí bod</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutní souřadnice</translation>
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
        <translation>Upravit &amp;obrysovou linku</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Zr&amp;ušit obrysovou linku</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Konec úprav</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Nezávisle přesouvat řídicí body</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Symetricky přesouvat řídicí body</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Otevře polygon nebo ořeže Bézierovou křivku</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Zavřít tuto Beziérovu křivku</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Zrcadlit vodorovně</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Zrcadlit svisle</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Uvolnit horizontálu doleva</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Uvolnit vertikálu nahoru</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Uvolnit vertikálu dolů</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotace proti směru hodinových ručiček</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotace po směru hodinových ručiček</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Zvětšit o uvedené %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Úhel rotace</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivovat režim pro úpravy obrysové linky</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Nastavit obrysovou linku na původní tvar rámce</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Uvolnit horizontálu doprava</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Je-li zatrženo, používají se souřadnice relativní ke stránce, jinak jsou relativní k objektu.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Zmenšit o uvedené %</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Zmenšit o zobrazenou hodnotu</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Zvětšit o zobrazenou hodnotu</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>% zvětšení či zmenšení</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Hodnota zvětšení či zmenšení</translation>
    </message>
    <message>
        <source>to Canvas</source>
        <translation>na plochu</translation>
    </message>
    <message>
        <source>to Page</source>
        <translation>na stránku</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Tento dokument nevypadá jako OpenOffice Draw soubor.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Import OpenOffice.org &amp;Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Import souborů OpenOffice Draw</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importuje do aktuálního dokumentu soubory OpenOffice.org Draw a převede vektory na objekty Scribusu.</translation>
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
        <translation>Tento soubor obsahuje nějaké nepodporované vlastnosti</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použít jméno dokumentu jako předponu stylů odstavce</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Neptat se znovu</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Volby importu OpenDocument</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Přepsat existující styly novými</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sloučit styly odstavce</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sloučí styly podle jejich vlastností. Výsledkem bude několik stylů se specifickými vlastnostmi, přestože původní dokument obsahoval styly pojmenované jinak.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Přidat název dokumentu do jména stylu.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Nastavit tyto vlastnosti jako implicitní a neptat se na ně při každém importu OASIS OpenDocument formátu.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Přepsat styly odstavce</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
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
        <translation>Šířka linky</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Uložit jako PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Výstup do souboru:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>&amp;Každou stránku do souboru</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF soubory (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Umožňuje exportovat individuálně pojmenované PDF soubory pro každou stránku v dokumentu. Čísla stránek jsou přidána automaticky. Vhodné zejména pro efektní komerční tisky.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Tlačítko pro uložení bude zakázané, pokud se pokusíte exportovat PDF/X-3 a bude chybět informační řetězec z karty PDF/X-3.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Ukládám PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Export vzorových stránek:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Export stránek:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Export objektů aktuální stránky:</translation>
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
        <translation>Náhled před tiskem</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Zobrazí průhlednost a průhledné objekty v dokumentu. Vyžaduje Ghostscript 7.07 nebo novější</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Vytvoří náhled tisku simulací CMYK inkoustů místo RGB barev</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Povolí/zakáže C (Cyan) složku</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Povolí/zakáže M (Magenta) složku</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Povolí/zakáže Y (Yellow) složku</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Povolí/zakáže B (Black) složku</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Zobrait &amp;průhlednost</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Zobrazit CMYK</translation>
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
        <translation>&amp;Under Color Removal</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Jméno separace</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tyrkysová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Zvětšení:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Tisk...</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Způsob, jak odstranit některé odstíny šedé, které jsou tvořeny tyrkysovou, žlutou a purpurovou, a použít místo nich černou. UCR ovlivní části obrázku, které jsou neutrální a/nebo obsahují tmavé tóny blízké šedé. Můžete tak vylepšit tisk některých obrázků, je ale nutné to vyzkoušet v praxi a trochu experimentovat. UCR snižuje riziko přesycení v případě CMY inkoustů.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Změnit měřítko stránky.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation>Povolit &amp;vyhlazování</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Umožňuje hezčí náhled fontů Type 1, TrueType, OpenType, EPS, PDF a vektorové grafiky, ovšem za cenu zpomalení</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Zpracování vzorových stránek:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Export stránek:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Obrázek</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linka</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnohoúhelník</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomená linka</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Text na křivky</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopie</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Vztahuje se k</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je rodičem</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je dítětem</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Vlastnosti objektu stránky</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametr</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Vztah</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Vztah k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>First Page is:</source>
        <translation>První stránka je:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Vzhled dokumentu</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Dvojstrany</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Střední pravá</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Přetáhnutím stránek nebo vzorových stránek na koš je smažete</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Všechny vaše vzorové stránky. Přetažením vzorové stránky na plochu vytvoříte stránku novou</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Uspořádat stránky</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Dostupné vzorové stránky:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Stránky dokumentu:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 of %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 of %1</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation>Quarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation>Kancelářský papír</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Letter</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation>Government Letter</translation>
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
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation>Výsledek</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Hledat výsledek pro:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Náhled</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Cesta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stránka</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Tisk</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Stav</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation type="obsolete">Jít na</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Chybí</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Hledat</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Zrušit hledání</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Správa obrázků</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Hledání obrázků</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Chyba hledání: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>6ádný soubor jménem &quot;%1&quot; nenalezen.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Zvolit základní adresář</translation>
    </message>
    <message>
        <source>Manage Images</source>
        <translation>Správa obrázků</translation>
    </message>
    <message>
        <source>Go to</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Uložit jako &amp;obrázek...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportovat jako obrázek</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exportuje vybrané stránky jako bitmapové obrázky.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Nelze najít modul</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>neznámá chyba</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Nelze najít symbol (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Modul: nahrávám %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>chyba inicializace</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>neznámý typ modulu</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 načten</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 chyba nahrávání: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Správce modulů</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Modul</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Jak spustit</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Nahrát?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>ID modulu</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Aby se změny projevily, musíte znovu spustit program.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Vlastnosti mnohoúhelníků</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ro&amp;hy:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Použít &amp;faktor</translation>
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
        <translation>Počet rohů mnohoúhelníků</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Stupně rotace mnohoúhelníků</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplikovat konvexnost/konkávnost pro změnu tvaru mnohoúhelníku</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Ukázkový mnohoúhelník</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Záporná hodnota nastaví mnohoúhelník jako konkávní (nebo s tvarem hvězdy), kladná jako konvexní</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Nastavení</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Všeobecné</translation>
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
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Okrajová vodítka</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatické uložení</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externí nástroje</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Nástroj na úpravu obrázků</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Velikost písma v menu a oknech</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Implicitní měrná jednotka dokumentu</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Počet řádek, o které Scribus posune text při pohybu kolečka myši</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Počet naposledy otevřených dokumentů zobrazených v nabídce Soubor</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Implicitní adresář na dokumenty</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Implicitní adresář na skripty</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Implicitní velikost stránky, standardní nebo vlastní rozměr</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Implicitní orientace stránek dokumentu</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stránek dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stránek dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Časový interval automatického ukládání</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papíru</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplnit plochu za hranicemi stránky barvou okrajů</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Nastavení implicitní úrovně zvětšení</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat text při vykreslování EPS a PDF na obrazovce</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat grafiku při vykreslování EPS a PDF na obrazovce</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Téma:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Skok kolečka myši:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nedávné dokumenty:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenty:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>Z&amp;měnit...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profily:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Změn&amp;it...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>S&amp;kripty:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Změni&amp;t...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Interval:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Přizpůsobení &amp;velikosti obrazovky</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>Jmé&amp;no spustitelného souboru (programu):</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Vyhlazený &amp;text</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Vyhlazená &amp;grafika</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Jméno &amp;spustitelného souboru (programu):</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Šablony &amp;dokumentů:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Jednotky:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Undo/Redo</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Délka historie akcí</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Dělení slov</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Správa barev</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položek dokumentu</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstříky</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Klávesové zkratky</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Zobrazení stránky</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Zobrazit obrázky</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Zobrazit řetězení textu</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Zobrazit řídicí znaky textu</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Zobrazit rámce</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke stránce</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Pracovní prostor</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Mezistránková mezera</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodorovná:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Svislá:</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Zobrazení přizpůsobíte posunem jezdce na spodním pravítku.</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Rozlišení:</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Vždy se ptát před nahrazením písem při načítání dokumentu</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Při úpravě stylů je náhled na aktuální styl odstavce viditelný</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Zobrazovat uvítací dialog</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Vždy použít klasické Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Počet odstavců:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Různé</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Moduly</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobrazit netisknutelné znaky, např. značky pro odstavec v textových rámcích</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Přepíná zobrazení rámců</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Přepíná zobrazení obrázků</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Doplňkový adresář pro šablony dokumentů</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Umístěte na obrazovku pravítko a posuňte jezdce, abyste nastavili požadované přiblížení. Scribus pak zobrazí stránky a jejich objekty ve správné velikosti</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa vlevo od dokumentu, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa vpravo od dokumentu, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa nad dokumentem, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa pod dokumentem, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Umístění Ghostscriptu</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Umístění editoru obrázků</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Interpret PostScriptu</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Povolit nebo zakázat zobrazení propojených rámců.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Vyberte jazyk, ve kterém se má Scribus spustit. Pokud ho nezvolíte, zvolí se na základě proměnných prostředí. Stále jej však budete moci změnit při spouštění Scribusu zadáním parametru na příkazové řádce</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>&amp;Velikost písma (nabídky):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Velikost &amp;písma (palety):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Implicitní dekorace oken a vzhled. Scribus přejímá dostupná témata KDE nebo Qt, pokud je Qt nastaveno pro vyhledávání pluginů KDE.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Implicitní velikost písma pro okna nástrojů</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Defaultní adresář ICC profilů. Nelze měnit, pokud je dokument otevřený. Normálně Scribus hledá v systémových adresářích (Mac OS X, Windows). V Linuxu a Unixu se prohledává adresář $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Pokud je povoleno, Scribus uloží záložní kopii souboru s příponou .bak pokaždé, když uplyne zadaný čas</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Délka historie jednotlivých akcí po krocích. Pokud se rovná nule, ukládá se neomezené množství akcí.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Cesta ke grafickému editoru. Pokud používáte GIMP a váš systém ho obsahuje, doporučujeme použít &apos;gimp-remote&apos;, protože se obrázek načte v instanci, která je už spuštěná.</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation>Cesta pro interpreter Ghostscript. Nezapoměňte, že ve Windows je nutné použít program gswin32c.exe, NE gswin32.exe, což by mohlo vést k zamrznutí Scribusu.</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation>Zobrazit obrázky</translation>
    </message>
    <message>
        <source>Turns the display of images on or off</source>
        <translation>Přepne zobrazení obrázků</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Výchozí</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Uložit...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Uložit nastavení</translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Postscript</source>
        <translation>Postsript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Převést stará nastavení Scribusu?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus nalezl soubory s nastavením pro Scribus 1.2.
Chcete je převést na novou verzi Scribusu?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Nelze otevřít soubor s nastavením &quot;%1&quot; pro zápis: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Zápis do souboru s nastavením &quot;%1&quot; selhal:  chybový kód QIODevice %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Nelze otevřít soubor s nastavením &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Nelze číst XML nastavení z &quot;%1&quot;: %2, řádek %3, sloupec %4</translation>
    </message>
    <message>
        <source>PDF 1.3</source>
        <translation type="obsolete">PDF 1.3</translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="obsolete">PDF 1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>Chyba zápisu nastavení</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Nelze uložit nastavení: &lt;br&gt;%1&lt;br&gt; Zkontrolujte prosím oprávnění k souboru a adresáři a také volné místo dostupné na disku.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Chyba při načítání nastavení</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Nelze načíst nastavení:&lt;br&gt;%1&lt;br&gt;Použije se implicitní nastavení.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>Magazine</source>
        <translation>Časopis</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Zlatý řez</translation>
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
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Můžete si vybrat předdefinovaný vzhled stránky. &apos;Žádný&apos; ponechá okraje, jak jsou, &apos;Gutenberg&apos; nastaví okraje klasicky, &apos;časopis&apos; nastaví okraje na stejnou hodnotu. Nejdůležitější je hodnota vlevo/uvnitř.</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>Spustit sk&amp;ript</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Spustit jako &amp;konzole</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>Uložit &amp;výstup...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Python konzole</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation type="obsolete">Odvozeno z běžné konzole Pythonu, takže obsahuje jistá omezení, např. co se mezer týče. Více najdete v manuálu Scribusu.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Konzole skriptů</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Sem napište příkazy. Výběr je zpracován jako skript</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Výstup vašeho skriptu</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python skripty (*.py);</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Uložit příkazy Pythonu do souboru</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Textové soubory (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Uložit aktuální výstup</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Otevřít soubor se skriptem v Pythonu</translation>
    </message>
    <message>
        <source>This is a standard Python console with some 
known limitations. Please consult the Scribus 
Scripter documentation for futher information. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Odstín:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sytost:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Hod:</translation>
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
        <translation>&amp;Alfa kanál:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Základní barvy</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Vlastní barvy</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definovat vlastní barvy &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Přidat k vlastním barvám</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Výběr barvy</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopírovat nebo přesunout soubor</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Číst: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Zapsat: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Jmé&amp;no souboru:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Typ souboru:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>O adresář výš</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
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
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Nahlédn&amp;i do:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Zpět</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Vytvořit nový adresář</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Pohled seznam</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Podrobný pohled</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Zobrazit informace o souboru</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Náhled na obsah souboru</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Číst-psát</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Jen číst</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Jen psát</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Nepřístupný</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symbolický odkaz na soubor</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symbolický odkaz na adresář</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symbolický odkaz na speciální</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Adresář</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Speciální</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Otevřít</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Přejmenovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Seřadit podle &amp;jména</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Seřadit podle &amp;velikosti</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Seřadit podle &amp;data</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nesetříděno</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Řazení</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Zobrazit s&amp;kryté soubory</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>soubor</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>adresář</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>symbolický odkaz</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Smazat %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Opravdu chcete smazat %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nový adresář 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nový adresář</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nový adresář %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Najít adresář</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Adresáře</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Uložit</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Chyba</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1(new line)
Soubor nenalezen.(new line)
Zkontrolujte cestu a název souboru.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Včechny soubory (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Adresář:</translation>
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
        <translation>S&amp;tyl písma</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Přešk&amp;rtnuté</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>Po&amp;dtržení</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Příklad</translation>
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
        <translation>Použít</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Vybrat písmo</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Vymazat</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vybrat vše</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Vyrovnat</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Upravit...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;O Qt&lt;/h3&gt;&lt;p&gt;Tento program používá Qt verze %1.&lt;/p&gt;&lt;p&gt;Qt je C++ knihovna určená pro vývoj multiplatformních nejen GUI aplikací.&lt;/p&gt;&lt;p&gt;Qt zajišťuje přenositelnost jediného zdrojového kódu na MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, a všechny hlavní varianty komerčních Unixů.&lt;br&gt;Qt je také k dispozici pro jednoúčelová zařízení (embedded devices).&lt;/p&gt;&lt;p&gt;Qt je produktem firmy Trolltech. Více informací získáte na &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Opravdu chcete přepsat soubor:
%1 ?</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Inicializace...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG obrázky (*.svg *.svgz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG obrázky (*.svg);;Všechny obrázky (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Uložit jako obrázek</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Chyba při zápisu souboru.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Úspěšný export.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Soubor existuje. Přepsat?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>už existuje. Přepsat?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ano všem</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Věstníky</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brožury</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogy</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Letáky</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Pokyny</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Karty</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Dopisy</translation>
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
        <translation>Kalendáře</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Inzeráty</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Štítky</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Nabídky (Menu)</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programy</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF formuláře</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF prezentace</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Časopisy</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakáty</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Oznámení</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Textové dokumenty</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Složky</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Vlastní šablony</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Všechny podporované formáty (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Všechny soubory (*)</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus skripty</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Vykonat skript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Použité skripty</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Zobrazit &amp;konzoli</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importovat text</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Všechny podporované formáty</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML soubory</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Textové soubory</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Soubory CSV (čárkou oddělené hodnoty)</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_data</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_hlavička</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Písmo %1 je poškozené. Bude vyřazeno</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Odkazy ven
</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Textové filtry</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Obaly médií</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>albánština</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>baskitština</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>bulharština</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>katalánština</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>čínština</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>čeština</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>dánština</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>holandština</translation>
    </message>
    <message>
        <source>English</source>
        <translation>angličtina</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>britská angličtina</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>němčina</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>finština</translation>
    </message>
    <message>
        <source>French</source>
        <translation>francouzština</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>galicijština</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>řečtina</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>maďarština</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>indonéština</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>italština</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>korejština</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>litevština</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>norština (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>norština (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>norština</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>polština</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>ruština</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>švédština</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>španělština</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>španělština (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>slovenština</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>slovinština</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>srbština</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;O skriptu...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>O skriptu</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat velikost písma z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat písmo z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat velikost textu z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat počet sloupců z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat řádkování z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat mezisloupcovou mezeru z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat text z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámce.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Hodnota zarovnání je mimo povolený rozsah. Použijte jednu z předdefinovaných konstant scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Pozice výběru mimo povolené hranice</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt není propojený textový rámec, proto nelze propojení zrušit.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektu na posledním místě nelze zrušit propojení. Použijte místo něj předchozí rámec.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Jednotka mimo povolený rozsah. Použijte jednu z předdefinovanýcj konstant scribus.UNIT*.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Cíl není obrázkový rámec.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Úhel rohu nesmí být negativní číslo.</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze získat barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze změnit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze vytvořit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze smazat barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze nahradit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.o Writer dokumenty</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Barva nenalezena - Python chyba</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Volitelná uživatelská konfigurace:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardní konfigurace:</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky. Čekejte prosím...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky. Hotovo.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikánština</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turečtina</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrajinština</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Welština</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Jméno souboru musí být řetězec znaků.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Nelze smazat nastavení typu obrázku.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Typ obrázku musí být řetězec.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Atribut allTypes je pouze ke čtení</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Chyba při exportu obrázku</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena v dokumentu.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena v implicitních barvách.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Nelze změnit velikost o 0%.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Specifikovaný objekt není obrázkový rámec.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Písmo nenalezeno.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Nelze vykreslit prázdný náhled.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Nelze zadat prázdné jméno vrstvy.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Vrstva nenalezena.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Nelze odstranit poslední vrstvu.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Nelze vytvořit vrstvu beze jména.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Index mimo povolené hranice.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit zarovnání textu netextovému rámci.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Velikost písma je mimo povolený interval - &lt;1, 512&gt;.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit velikost písma netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit písmo netextového rámce.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Velikost řádkování mimo hranice, musí být větší než 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit hodnotu řádkování netextovému rámci.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Velikost mezisloupcové mezery mimo povolený rozsah. Musí být kladné číslo.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit mezisloupcovou mezeru netextovému rámci.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Počet sloupců mimo povolený rozsah. Musí být &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit počet sloupců netextovému rámci.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Nelze vybrat text v netextovém rámci</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze smazat text z netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit výplň textu netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit tah textu u netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit odstín písma netextového rámce.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Propojit lze pouze textové rámce.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Cílový rámec musí být prázdný.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je propojený.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je propojený.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Zdroj a cíl jsou stejný objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze zrušit propojení netextového rámce.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Netextový rámec nelze převést na obrysy.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rámec nemůže být poznámkou</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rámec neobsahuje požadované informace</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument dokumenty (OpenOffice 2)</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Chorvatsky</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugalsky</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugalština (Braz.)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Zhroucení Scribusu</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus spadl díky signálu č. %1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stránka</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Vzorová stránka </translation>
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
        <translation>Nelze otevřít výstupní soubor %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Do výstupního proudu nelze zapisovat</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Selhala kontrola nastavení: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Nelze otevřít vstupní soubor %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Nelze načíst nastavení XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (řádek %2, sloupec %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Nelze načíst nastavení XML:. %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>nulový kořenový uzel

Načíst nastavení PDF</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; neplatné

Načíst nastavení PDF</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>nalezeno %1 &lt;%2&gt; uzlů, potřeba 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>neočekávaný nulový &lt;%2&gt; uzel</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>uzel &lt;%1&gt; není element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; neobsahuje vlastnost `value&apos;</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>hodnota elementu &lt;%1&gt; musí být `true&apos; nebo `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;lpiSettingsEntry&gt; neobsahuje vlastnost `name&apos;</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Knihovna Freetype2 není k dispozici</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Font %1 je poškozený, nebude vložen</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Font %1 je poškozený (chyba při čtení), nebude vložen</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Font %1 je poškozený (FreeType2), ignoruje se</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Font %1 je poškozený (není Face), ignoruje se</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Font %1 má poškozený glyf %2 (znak %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Font %1 je poškozený a bude ignorován</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Font %1 nelze načíst, nebude vložen</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Nelze načíst font %1 - neznámý typ fontu</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Font %1 načten z %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Font %1(%2) je duplicitou %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Načítám font %1 (nalezen pomocí fontconfigu)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Nelze načíst font FreeType2 - nelze nalézt soubor s fontem</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Font %1 je poškozený (FreeType), bude ignorován</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Font %1 obsahuje neplatný glyf %2 (znak %3), bude ignorován</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>rozbaluji face %1 z fontu %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>hlavička memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabulka &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>tabulka memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Vývojová verze Scribus</translation>
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
        <translation>Body (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Milimetry (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Palce (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pika (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimetry (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Soubor existuje</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Nahradit</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>stránka</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Šablona dokumentu: </translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Dokument nelze otevřít.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Dokument nelze uložit.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argumentem musí být název objektu stránky, nebo instance PyCObject</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Vlastnost nenalezena</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Potomek nenalezen</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Nelze konvertovat výsledek typu &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Vlastnost typu &apos;%1&apos; není podporována</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Nelze konvertovat &apos;%1&apos; na vlastnost typu &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Typy odpovídají, ale nastavení vlastnosti selhalo.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Nelze seskupit méně než dvě položky</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Nelze seskupit méně než dvě položky</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>K seskupení je nutný výběr nebo seznam argumentů pro položky</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Nelze uložit pixmapu</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Objekt s požadovaným jménem již existuje.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň dva body (čtyři hodnoty).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat sudý počet hodnot.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň tři body (šest hodnot).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň čtyři body (osm hodnot).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat násobky šesti hodnot.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekt nenalezen.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Styl nenalezen.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit styl netextovému rámci.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Nelze uložit EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Číslo stránky mimo rozsah.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument není v seznamu: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nečíselné hodnoty: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nečíselné hodnoty: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Šířka linky mimo rozsah, musí být 0 &lt;= line_width &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Odstín linky mimo rozsah, musí být 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Odstín výplně mimo rozsah, musí být 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Styl linky nenalezen.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Jen textové rámce mohou být testovány na přetečení</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Název souboru nesmí být prázdný řetězec.</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
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
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Tyrkysová</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Kruhová paleta</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Náhled písem</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Můj modul</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nový ze šablony</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Exportovat jako obrázek</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS Import</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Uložit jako šablonu</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skripter</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
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
        <translation>Import OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus se zhroutil kvůli následující výjimce: %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Vytvářím cache písem</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Nalezena nová písma, ověřuji...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Nalezena změněná písma, ověřuji...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Načítám cache písem</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Zapisuji aktualizovanou cache písem</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Hledám písma</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Používáte vývojovou verzi Scribusu 1.3.x. Dokument, se kterým pracujete, byl vytvořen ve Scribusu 1.2.3 nebo starším. Pokud jej teď uložíte, v těchto starších verzích jej nebude možné načíst, pokud ovšem nepoužijete Soubor-Uložit jako. Opravdu chcete pokračovat?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Změny ve vašem dokumentu nebyly uloženy, ovšem vy požadujete jejich vrácení. Opravdu chcete pokračovat?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Soubor jménem &apos;%1&apos; již existuje.&lt;br/&gt;Chcete jej nahradit souborem, který právě ukládáte?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder je větší, než je dovoleno.</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Podpora starého formátu .sla</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Německy</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Export PostScript souboru</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Tisk souboru</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Snažíte se importovat více stránek, než je počet stránek dostupných v aktuálním dokumentu, počítáno od aktivní stránky.&lt;/p&gt;Vyberte některou možnost:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Vytvořit&lt;/b&gt; chybějící stránky&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importovat&lt;/b&gt; stránky do poslední stránky&lt;/li&gt;&lt;b&gt;Zrušit&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Vytvořit</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>thajština</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Generátor čárových kódů</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word dokumenty</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB dokumenty</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_data</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Import PDB</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Nelze otevřít soubor %1</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>lucemburština</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>arabština</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation>estonština</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>japonština</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Zadané jméno vzorové stránky nesouhlasí s žádnou existující.</translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>islandština</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 může být poškozený: chybí značky pro rozlišení</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Tento soubor není PDB dokument. Pokud si myslíte, že je, nahlaste nám, prosím, chybu.</translation>
    </message>
    <message>
        <source>Breton</source>
        <translation>bretonština</translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation>americká angličtina</translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation>australská angličtina</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation>%1 může být poškozený: chybějící nebo poškozené značky</translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation>švýcarská němčina</translation>
    </message>
    <message>
        <source>Chinese (Trad.)</source>
        <translation>čínština (trad.)</translation>
    </message>
    <message>
        <source>Insufficient memory for this image size.</source>
        <translation>Nedostatek paměti pro tak velký obrázek.</translation>
    </message>
    <message>
        <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
        <translation>Písmo %1 má poškozenou metriku v souboru %2, metriku ignoruji</translation>
    </message>
    <message>
        <source>Valid metrics were found for font %1, using metrics in file %2</source>
        <translation>Nalezeny platné metriky pro písmo %1, používám metriku ze souboru %2</translation>
    </message>
    <message>
        <source>Fill opacity out of bounds, must be 0.0 &lt;= opacity &lt;= 1.0</source>
        <comment>python error</comment>
        <translation>Neprůhlednost výplně mimo rozmezí, musí být mezi 0,0 a 1,0</translation>
    </message>
    <message>
        <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
        <comment>python error</comment>
        <translation>Průhlednost mimo rozsah, musí být mezi 0 a 1.</translation>
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
        <translation>Vymazat</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Vybrat vše</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Systémová nabídka</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Skrýt</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizovat</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimalizovat</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximalizovat</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Přesunout</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimalizovat</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximalizovat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Zůs&amp;tat navrchu</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimalizovat</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Obnovit původní</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Schovat</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Z&amp;obrazit</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Nastavení dokumentu</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Okrajová vodítka</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Jednotka:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatické uložení</translation>
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
        <translation>Informace o dokumentu</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Zobrazení stránky</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Zobrazit obrázky</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Zobrazit řetězení textu</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Zobrazit řídicí znaky textu</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Zobrazit rámce</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke stránce</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Minimální pracovní prostor</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Mezistránková mezera</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodorovná:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Svislá:</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Dělení slov</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položky v dokumentu</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstříky</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Správa barev</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobrazit v textových rámcích netisknutelné znaky jako např. znaky konce odstavců</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Přepíná zobrazení rámců</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Vypne nebo zapne zobrazení obrázků</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papíru</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplnit plochu za hranicemi stránky barvou okrajů</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Přizpůsobení barev</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Přepíná zobrazení propojených textových rámců.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Použít nastavení velikosti pro všechny stránky</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Sekce</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Použít změny ve velikosti stránky na všechny existující stránky dokumentu</translation>
    </message>
    <message>
        <source>Show Images</source>
        <translation>Zobrazit obrázky</translation>
    </message>
    <message>
        <source>Turns the display of images on or off</source>
        <translation>Přepne zobrazení obrázků</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python skripty (*.py);; All Files (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Spustit jako Python rozšíření</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Správce stylů</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation>Sloupec 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>O&amp;K</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>&amp;Použít</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>Linky</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Styl aktuálního odstavce</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Nastavení stylu</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Color of text fill</source>
        <translation>Barva výplně písma</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Sytost barvy výplně písma</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Nastavení výplně</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva tahu textu</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Barevná sytost tahu textu</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Nastavení barvy tahu</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
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
        <translation>Písmo vybraného textu</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Změna šířky znaků písma</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Nastavení písma</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Upravuji výšku znaků</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Nastavení znaků</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
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
        <translation>Uložit stránku jako &amp;SVG...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Export SVG souborů</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Export aktuální stránky do SVG souboru.</translation>
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
        <translation>Import SVG souborů</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje většinu SVG souborů do aktuálního dokumentu,
přičemž konvertuje vektorová data na objekty Scribusu.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG obsahuje některé nepodporované vlastnosti</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Aplikovat nezlomitelnou mezeru na:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Vybrané rámce</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Aktuální stránka</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>Všechny &amp;objekty</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Upraveny pouze vybrané rámce.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Upravena pouze aktuální stránka.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Upraven celý dokument.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Uživatelské nastavení</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Systémová konfigurace</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Uložit uživatelskou konfiguraci</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Načíst systémové nastavení a odstranit uživatelské</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Upravit vlastní nastavení. Pokud je uložíte, má přednost před systémovým nastavením</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Uživatelské nastavení již existuje. Opravdu je chcete přepsat?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Nelze uložit soubor %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Uživatelské nastavení uloženo</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Systémové nastavení načteno</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Nelze otevřít soubor %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Uložit jako ša&amp;blonu...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Uložit dokument jako šablonu</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Uložit dokument jako šablonu. Dobrá cesta, jak si usnadnit práci s dokumenty, které mají mít jednotný vzhled</translation>
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
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Akce</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Načíst/Uložit/Import/Export</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznámý</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Nahoře</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Dole</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Umožnit dokování...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodorovně</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Svisle</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Prostorová orientace...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Tisknu...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>Barvy v dokumentu</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nová vrstva</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Opravdu chcete vyčistit celý text?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Nelze smazat používaný objekt</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku, nelze tedy nic smazat</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Při otevírání ICC profilů došlo k chybě, správa barev není aktivní.</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializace zásuvných modulů</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Inicializace klávesových skratek</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Načítám nastavení</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Inicializace zápisníku</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Inicializece ICC profilů</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Inicializace dělení slov</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Načítám zápisník</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Nastavení klávesových zkratek</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Upravit</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Hledám písma</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Nenalezena zádná písma.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Ukončuji.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Inicializován systém písem</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Otevřít &amp;předchozí</translation>
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
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;yl</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Od&amp;stín</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Písmo</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efekty</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>O&amp;bjekt</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Nastavení náhledu</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Hladina</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Volby PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Převést na</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mezery</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Stránka</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>Ná&amp;hled</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Okna</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>Nápo&amp;věda</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Zarovnání</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Připravený</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importuji stránky...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importovat stránku(y)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import ukončen</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nenalezeno nic k importu</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Soubor %1 není v přijatelném formátu</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Načítám...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Některé ICC profily použité tímto dokumentem nejsou nainstalované:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> zaměněn za: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(konvertováno)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Všechny podporované formáty</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Nelze uložit soubor: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Ukládám...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus detekoval nějaké chyby. Zvažte použití předtiskové kontroly pro jejich odstranění</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorovat</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Přerušit</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Tisknu...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Tisk se nepodařil!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Nelze odstranit používaný objekt</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku. proto nelze nic vyříznout</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus manuál</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové soubory (*.txt);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Převést stránku na vzorovou stránku</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Od&amp;stín:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Odstín</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Chybí následující program(y):</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: nelze použít EPS obrázky nebo Náhled před tiskem</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus detekoval nějaké chyby.
Zvažte použití předtiskové kontrol pro jejich odstranění.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS soubory (*.eps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Detekovány chyby.
Zvažte použití předtiskové kontroly pro jejich odstranění</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Stránka%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Některé objekty jsou zamčeny.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Zamknout vše</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odemknout vše</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informace</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Program %1 už běží!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Program %1 nenalezen!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Vybraná barva v barevné paletě dokumentu neexistuje. Zadejte prosím její nové jméno.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Barva nenalezena</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Barva, kterou jste vybrali, již existuje. Pojmenujte prosím novou barvu jinak.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Hladina</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Přesunout do vrstvy</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Nas&amp;tavení náhledu</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Nástroje</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Poz:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mezery &amp;&amp; Zalomení</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nová vzorová stránka %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Počet kopií: %1
Vodorovné posunutí: %2
Svislé posunutí: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: nelze použít EPS obrázky nebo PS Náhled před tiskem</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript chybí: PS Náhled před tiskem nelze použít</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Opravdu chcete zaměnit existující obrázek?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;tura</translation>
    </message>
    <message>
        <source>Scribus </source>
        <translation type="obsolete">Scribus </translation>
    </message>
    <message>
        <source>Your document was saved to a temporary file and could not be moved: 
%1</source>
        <translation>Váš dokument byl uložen jako dočasný soubor, ale nelze jej přesunout: 
%1</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Nesprávný argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Soubor %1 neexistuje, končím.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Použití: scribus [volba ...] [soubor]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Volby:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Zobrazit nápovědu (tuto zprávu) a skončit</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Používá xx jako zkratku pro jazyk, např. &apos;en&apos; nebo &apos;de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Zobrazit dostupné jazykové verze prostředí</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Zobrazit na konzoli informace o načítání písem</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Nezobrazovat při startu uvítací obrazovku</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Zobrazit informace o verzi a skončit</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Použít pořadí tlačítek zprava doleva (tedy Zrušit/Ne/Ano místo Ano/Ne/Zrušit)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>jméno souboru</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Jméno souboru pro uživatelské nastavení</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Nainstalované jazykové verze v prostředí Scribusu:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Změna nastavení jazyka:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx nebo scribus --lang xx, kde xx je kód jazyka.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Verze Scribusu</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Webová stránka</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentace</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Vydání</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>Zobrazit okno konzole</translation>
    </message>
    <message>
        <source>Show location ICC profile information on console while starting</source>
        <translation>Zobrazit při startu konzole informace o ICC profilu</translation>
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
        <translation>Vrstva</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopírovat sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Přesunout sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Obrázek</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Soubor:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Původní PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Aktuální PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Řeťezený text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Textový rámec</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text na křivce</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavce:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Slova:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaky:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Tisk:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Povoleno</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Zakázáno</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Nastavení náhledu</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Volby PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;Hladina</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>K&amp;onverze na</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Propojení textových rámců</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Pokoušíte se odkazovat na vyplněný rámec, nebo na rámec samotný.</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Nelze konvertovat používaný objekt</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku, proto bude přeskočeno jeho konvertování na obrysy</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Stránka %1 až %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Barevný prostor: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznámý</translation>
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
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <source>Export: </source>
        <translation>Export: </translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Obrázek</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>od posledního uložení byl dokument změněný.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Zabudovaná podpora pro skripty v Pythonu.</translation>
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
        <translation>Chyba ve skriptu</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Jestliže běžel skript distribuovaný s programem, informujte nás o chybě na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; - děkujeme vám.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Tato chybová zpráva je ve vaší systémové schránce. Použijte Ctrl+V, tím ji
můžete zkopírovat do chybového hlášení.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Došlo k vnitřní chybě při provádění příkazu, který jste zadali. Detaily vypsány na stderr. </translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Prozkoumat skript</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Chybná inicializace Python modulu. Detaily chyby byly vypsány na standardní chybový výstup. </translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Python skripty (*.py);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentace:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Skript</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> neobsahuje žádný docstring!</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Nastavení skripteru</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Povolit Python rozšíření</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Rozšíření</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konzole</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Skript při startu:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Chyba:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Komentáře:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Klíčová slova:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Operátory:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Čísla:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Řetězce:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Základní texty:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Vyberte barvu</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Změnit...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Umístění start skriptu</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Zobrazit náhledy stránek</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Hledat/Nahradit</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Hledat:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Styl odstavce</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Písmo</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efekty písma</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Barva výplně</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Odstín výplně</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Barva tahu</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Odstín tahu</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Na střed</translation>
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
        <translation>Vynucené</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Nahradit čím:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Hledání hotovo</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Celé slovo</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorovat velikost znaků</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Nahradit</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Nahradit &amp;vše</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Hledání dokončeno, nalezeno %1</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Zvolit pole</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Dostupná pole</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Zvolená pole</translation>
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
        <translation>Jiný...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Odstín</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Od&amp;stín:</translation>
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
        <translation>&amp;Předložky a zkratky (nezlomitelná mezera)...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Speciální modul, který doplní nezlomitelné mezery před nebo za předložky a zkratky a další &quot;krátká slova&quot;. Dostupný v následujících jazycích: </translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Úprava stylů...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Upravit styly</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nový styl</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktuální odstavec:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Slov:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaků:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Celkem:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavců:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové soubory (*.txt);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Opravdu chcete uložit změny?</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Znovu načíst text z rámce</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>Uložit do &amp;souboru...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>Nahrá&amp;t ze souboru...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Uložit &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Aktualizovat textový rámec a skončit</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Skončit &amp;bez aktualizace textového rámce</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>Ak&amp;tualizovat textový rámec</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Vybr&amp;at vše</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Upravit &amp;styly...</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hledat/nahradit...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Náhled písem...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Pozadí...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Zobrazit písmem...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Nastavení</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Chytrý výběr textu</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Vložit znak...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Vymazat text</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Zápisník - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Opravdu chcete zrušit všechny své změny?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Opravdu chcete smazat celý text?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mezery &amp;&amp; Zalomení</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mezera</translation>
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
        <translation>Posunití</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Šířka linky</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation>Zvoleno více než jedna položka</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Small Caps</source>
        <translation>Kapitálky</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Dolní index</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Horní index</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Verzálky</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podtrhnout text. Stiskněte na chvíli tlačítko, abyste nastavili šířku linky a možnosti posunutí.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podtrhnout pouze slova. Stiskněte na chvíli tlačítko, abyste nastavili šířku linky a možnosti posunutí.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Proškrtnutí. Stiskněte na chvíli tlačítko, abyste nastavili šířku linky a možnosti posunutí.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation>Obrys. Stiskněte na chvíli tlačítko, abyste změnili šířku obrysového tahu.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Stínovaný text. Stiskněte na chvíli tlačítko, abyste nastavili odstup stínu od písma.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použít jméno dokumentu jako předponu stylů odstavce</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Neptat se znovu</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Volby importu OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Přepsat existující styly novými</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sloučit styly odstavce</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sloučí styly podle jejich vlastností. Výsledkem bude několik stylů se specifickými vlastnostmi, přestože původní dokument obsahoval styly pojmenované jinak.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Přidat název dokumentu do jména stylu.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Nastavit tyto vlastnosti jako implicitní a neptat se na ně při každém importu OASIS OpenDocument formátu.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Přepsat styly odstavců</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Na začátku</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Na konci</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Nezobrazeno</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstřík</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rámec pro tabulku s obsahem</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Čísla stránek umístěna:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Název vlastnosti objektu:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Vlastnost objektu, která bude u rámců použita jako základ pro vytváření záznamů</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Umístit čísla stránek na začátek řádků, nebo na konec, nebo vůbec</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Vypsat netisknuté položky</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Začlenit také rámce, které se nemají tisknout</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Odstavcový styl použitý pro vstupní řádky</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Styl odstavce:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Cílový rámec:</translation>
    </message>
    <message>
        <source>Table of Contents %1</source>
        <translation>Obsah %1</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstřík</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rámec pro tabulku s obsahem</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Čísla stránek umístěna:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Název vlastnosti objektu:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Vlastnost objektu, která bude nastavena u rámců jako základ při vytváření nových položek</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Umístit čísla stránek na začátek řádku, nebo na konec, nebo vůbec</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Vypsat netisknuté položky</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Začlenit také rámce, které se nemají tisknout</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Odstavcový styl použitý pro vstupní řádky</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Styl odstavce:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Cílový rámec:</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ignorovat všechny chyby</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Automaticky kontrolovat před tiskem nebo exportem</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Kontrolovat chybějící znaky</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Zjišṫovat objekty mimo stránky</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Kontrolovat přetečení textových rámců</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Kontrolovat transparentnost</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Kontrolovat chybějící obrázky</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Kontrolovat rozlišení obrázků</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Nejnižší povolené rozlišení</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Kontrolovat umístěné PDF soubory</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrolovat PDF anotace a formuláře</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Přidat profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Odstranit profil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Běžná nastavení</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Umístění v dokumentech</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>V pozadí</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>V popředí</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Jako magnet</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Vzdálenost pro magnet:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Poloměr uchopení:</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Zobrazit vodítka</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Zobrazit okraje</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Zobrazit mřížku stránky</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Hlavní mřížka</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Vzdálenost:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Vedlejší mřížka</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Nastavení účaří</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Pomocná &amp;mřížka:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Vzdálenost pomocné mřížky:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Vodítka nejsou viditelná přes objekty na stránce</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Vodítka jsou viditelná nad všemi objekty na stránce</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Vzdálenost mezi linkami vedlejší mřížky</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Vzdálenost mezi linkami hlavní mřížky</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Vzdálenost, od které se objekt přitáhne k vodítkům</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Poloměr oblasti, kterou Scribus považuje za oblast daného objektu</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Barva linek vedlejší mřížky</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Barva linek hlavní mřížky</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Barva vkládáných vodítek</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Barva pro linky okrajů</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Barva pomocné mřížky</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Přepne zobrazení pomocné mřížky</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Vzdálenost mezi linkami pomocné mřížky</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Vzdálenost prvního účaří od horního okraje stránky</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Přepne zobrazení linek mřížky</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Přepne zobrazení vodítek</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Přepne zobrazení okrajů</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides. After setting this you will need to restart Scribus to set this setting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles.After setting this you will need to restart Scribus to set this setting.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Správa tabelátorů</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Exportovat rozsah</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Všechny str&amp;ánky</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Vybrat stránky</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Volby souboru</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilita:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>Vaz&amp;ba:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Levý okraj</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Pravý okraj</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Vytvořit &amp;náhledy</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Uložit &amp;propojené textové rámce jako PDF články</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Vče&amp;tně záložek</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Rozlišení pro EPS grafiku:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primovat textovou a vektorovou grafiku</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maximální</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Vysoká</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Střední</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Nízká</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimální</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Všeobecné</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Vkládání</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Dostupná písma:</translation>
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
        <translation>Písma k vložení:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Písma</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Povolit efekty &amp;prezentace</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stránka</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Zobrazit &amp;náhledy stránek</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Doba zobrazení:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Tr&amp;vání efektu:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Typ efektu:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Přesouvání řádků:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>O&amp;d:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Směr:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation>s</translation>
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
        <translation>Rám</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Rozpuštění</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Lesk</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Rozdělit</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Setřít</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodorovně</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Svisle</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Zevnitř</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Zvenku</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Zleva doprava</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Shora dolů</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Zdola nahoru</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Zprava doleva</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Zleva nahoře na doprava dolů</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">Po&amp;užít efekt na všechny strany</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Použít ši&amp;frování</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Hesla</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Uživatel:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Vlastník:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Nastavení</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>&amp;Povolit tisk dokumentu</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Povolit z&amp;měny dokumentu</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Povolit &amp;kopírování textu a grafiky</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Povolit přidávání &amp;anotací a polí formulářů</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Bezpečnost</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Všeobecné</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>&amp;Plánovaný výstup na:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Obrazovka/web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Tiskárna</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Použít vlastní nastavení reprodukce</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Nastavení reprodukce</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Frekvence:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>Úhe&amp;l:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Funkce &amp;bodu:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Prostá tečka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linka</translation>
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
        <translation>Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Použít ICC profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Účel generování:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Obrázky:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Nepoužívat vložené ICC profily</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Výstup do PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Info text:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Výstupní profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Vlastní formát stránky</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Zobrazit náhled každé stránky uvedené v seznamu nahoře.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Typ efektu.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Směr efektu Přesouvání řádků nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Startovní pozice efektu Rám nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Směr efektu Lesk nebo Setřít.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Použít vybraný efekt na všechny stránky.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Export všech stránek do PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportovat rozsah stránek do PDF</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Vytvořit PDF články, což umožňuje navigaci odkazy v PDF.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (body na palec) pro export obrázků.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Zvolit heslo, které musí použít čtenář PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Povolit tisk PDF. Jestliže není zatrženo, tisk není povolen.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Povolit modifikaci PDF. Jestliže není zatrženo, modifikace jsou zakázány.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Vložit barevný profil plných barev</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Barevný profil plných barev</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Účel reprodukce plných barev</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Vložit barevný profil obrázků</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Nepoužívat barevný profil vložený ve zdrojových obrázcích</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Barevný profil obrázků</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Účel reprodukce obrázků</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Výstupní profil tisku. Je-li to možné, snažte se získat z tiskárny informace ohledně profilů.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Vzdálenost ořezu od horního okraje fyzické stránky</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Vzdálenost ořezu od dolního okraje fyzické stránky</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Vzdálenost ořezu od levého okraje fyzické stránky</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Vzdálenost ořezu od pravého okraje fyzické stránky</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Zrcadlit stránky vodorovně</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Zrcadlit stránky svisle</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertovat přímé barvy na procesní barvy</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Komprese a kvalita:</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Umožní kopírování textu nebo grafiky z PDF. Není-li zatrženo, text a grafiku nelze kopírovat.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Povolit přidávání anotací a polí do PDF. Pokud není zatrženo, úprava anotací a polí není možná.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Umožní převod přímých barev na kompozitní. Pokud neplánujete tisk přímých barev na komeční tiskárně, je zřejmě lepší nechat tuto volbu povolenou.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Vložit &amp;vrstvy</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>&amp;Metoda komprese:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Změnit ve&amp;likost obrázků na:</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Doba, po kterou efekt běží. Kratší doba efekt zrychlí, delší doba jej zpomalí.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vložte seznam identifikátorů oddělený čárkami. Identifikátor může být * pro všechny stránky, 1-5 pro rozmezí stránek nebo číslo konkrétní stránky.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Určuje způsob vazby stránek v PDF. Pokud ji nepotřebujete měnit, ponechte předvolenou hodnotu - vlevo.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Vytvoří náhledy každé stránky v PDF. Některé prohlížeče je pak používají pro navigaci.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Vložit do dokumentu vaše záložky. Je to praktické při orientaci v dlouhých PDF dokumentech.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Exportovat rozlišení textu a vektorové grafiky. Neovlivní to rozlišení bitmapových obrázků jako třeba fotografií.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Povolí bezztrátovou kompresi textu a grafiky. Pokud nemáte důvod to měnit, nechte zatržené. Ovlivníte tak velikost PDF souboru.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Povolit bezpečnostní vlastnosti v exportovaném PDF. Pokud vyberete PDF 1.3, výsledné PDF bude chráněno 40bitovým šifrováním. Pokud vyberete PDF 1.4, PDF bude chráněno 128bitovým šifrováním. Upozornění: PDF šifrování není tak věrohodné jako GPG nebo PGP šifrování a má svá omezení.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Vyberte hlavní heslo, které povolí nebo zakáže všechny bezpečnostní vlastnosti v exportovaném PDF</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Jedná se o pokročilé nastavení, které není běžně povoleno. Povolte jej pouze v případě, že jej vyžaduje vaše tiskárna a máte přesné instrukce, jak to udělat. Jinak hrozí, že vytvořené PDF nebude možné korektně tisknout a rozhodně jej nebude možné používat na různých systémech.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Povinný řetězec pro PDF/X-3 - jinak se PDF nebude shodovat s formátem PDF/X-3. Doporučujeme, abyste použili název dokumentu.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Nastavení displeje</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Jedna stránka</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Pruběžně</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Dvojitá stránka vlevo</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Dvojitá stránka vpravo</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Vzhled</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Použít nastavení prohlížeče</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Použít celoobrazovkový režim</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Zobrazit kartu Záložky</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Zobrazit náhledy</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Zobrazit kartu Vrstvy</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Skrýt nástrojový panel prohlížeče</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Skrýt hlavní nabídku prohlížeče</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Přizpůsobit velikost stránek oknu prohlížeče</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Zvláštní akce</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Bez skriptu</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Prohlížeč</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Zmenšit na okraje stránky</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Ztrátový - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Bezztrátový - ZIP</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Metoda komprese obrázku</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript, který se vykoná
při otevření PDF dokumentu:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Povolí prezentační efekty, pokud se použije Adobe&amp;#174; Reader&amp;#174; nebo jiný PDF prohlížeč, který je v celoobrazovkovém režimu podporuje.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Upresňuje kompatibilitu PDF. Běžné je PDF 1.3 s nejširší kompatibilitou. Pokud váš dokument používá průhlednost nebo požadujete 128bitové šifrování, pak použijte PDF 1.4. PDF 1.5 je nutné, pokud si přejete podporu vrstev.  PDF/X-3 je určen pro exportování dokumentů se správou barev v prostoru RGB, hodí se pro komerční tisky a lze jej vybrat pouze tehdy, máte-li aktivní správu barev. Použijte v případě, kdy je to nutné kvůli tiskárně nebo při tisku na čtyřbarevnou barevnou laserovou tiskárnu.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Vrstvy ve vašem dokumentu jsou do PDF exportovány pouze tehdy, je-li jako výstup zvolena PDF verze 1.5.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Změna rozlišení bitmapových obrázků na zvolené DPI. Necháte-li nezatržené, budou se obrázky vykreslovat ve svém přirozeném rozlišení. Pokud volbu zatrhnete, zvýší se paměťová náročnost a zpomalí export.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Barevný model pro výstupní PDF. Vyberte Obrazovka/web pro PDF soubory, které se budou zobrazovat na monitoru a tisknout na běžných inkoustových tiskárnách. Pokud se budou tisknout na CMYK tiskárně, zvolte Tiskárna. Chcete-li PDF v odstínech šedé, zatrhněte Odstíny šedé.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Nezobrazovat v exportovaném souboru objekty, které přesahují okraje</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Doba, po kterou se zobrazí snímek, než se prezentace spustí na zvolené stránce. Nastavení na nulu automatickou změnu stránek zakáže.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Metoda komprese pro obrázky. Automatická volba umožní vybrat nejlepší metodu. ZIP je bezztrátový způsob a je dobrý pro obrázky v plných barvách. JPEG je lepší pro vytváření malých PDF souborů s mnoha fotografiemi (s mírnou ztrátou kvality). Ponechejte Automaticky, pokud nemáte na kompresi speciální požadavky.</translation>
    </message>
    <message>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Úrovně kvality pro metody ztrátové komprese: Minimum (25 %), Nízká (50 %), Střední (75 %), Vysoká (85 %), Maximum (95 %). Uvědomte si, prosím, že úroveň kvality nemá jednoznačný vliv na datovou velikost výsledného obrázku - výsledná velikost a ztráta kvality se liší obrázek od obrázku, a to u každé úrovně komprese. I když vyberete Maximum, u JPEG vždy ke ztrátě dochází.</translation>
    </message>
    <message>
        <source>&amp;Embed All</source>
        <translation>&amp;Vložit vše</translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation>Písma pro obrys:</translation>
    </message>
    <message>
        <source>Outline &amp;All</source>
        <translation>&amp;Obrys pro vše</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Vzhled dokumentu</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>Vložit do PDF dokumentu písma. Vložení písem zachová vzhled dokumentu. Některá písma jako OpenType mohou být pouze podmnožinou, protože je nelze vložit do dokumentů před PDF verzí 1.6.</translation>
    </message>
    <message>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation>Vloží do PDF všechna písma jako podmnožinu (subset). To znamená, že se vkládají jen použité glyfy, ne celé písmo. Některá písma jako např. OpenType mohou být vložena pouze jako podmnožina, protože je nelze vložit do dokumentů před verzí PDF 1.6.</translation>
    </message>
    <message>
        <source>&amp;Apply Effect to all Pages</source>
        <translation>&amp;Použít pro všechny stránky</translation>
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
        <translation>Velikost:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Výplňová barva:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Barva tahu:</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Výplňový znak pro tabulátor:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Šířka tabulátoru:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Mezera:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Ba&amp;rva linky:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Stín:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Stí&amp;n:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Styl linky:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Šíř&amp;ka linky:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>S&amp;tyl linky:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Šipky:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Začátek:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Konec:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vodorovné zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>S&amp;vislé zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">Změnit velikost &amp;obrázku na rozměr rámu</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Dod&amp;ržet poměr stránek</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation></translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Náhled na obrazovce</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Náhled v plném rozlišení</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Náhled v normálním rozlišení</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Náhled v nízkém rozlišení</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>&amp;Maximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Krokování:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Vlastnosti textového rámce</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Vlastnosti obrázkových rámců</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Vlastnosti kreslení tvarů</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Vlastnosti úrovně zvětšení</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Vlastnosti linek</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Vlastnosti mnohoúhelníků</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Písmo nových textových rámců</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Velikost písma nových textových rámců</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Barva písma</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Počet sloupců v textovém rámci</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Mezera mezi sloupci textového rámce</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Ukázka písma</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Obrázkové rámce mohou libovolně měnit rozměry obrázku</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Vodorovné zvětšení obrázků</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Svislé zvětšení obrázků</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Dodržet stejné vodorovné a svislé zvětšení</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Obrázky budou deformovány podle rozměrů rámce</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Automaticky nastavovaná velikost obrázků dodržuje originální rozměry</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Barva výplně obrázkových rámců</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Sytost barvy výplně</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Barva linek tvarů</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Sytost barvy linek</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Výplňová barva tvarů</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Styl linek tvarů</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Tloušťka linek tvarů</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Minimální povolené zvětšení (zmenšení)</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Maximální povolené zvětšení</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Změna zvětšení - krok operace lupou</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Barva linek</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Sytost barvy</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Styl linek</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Šířka linek</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Vlastní:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Vlastní: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Barva textu:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Stín:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Tah textu:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Tečka</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Pomlčka/spojovník</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podtržení</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>&amp;Scale Image to Frame Size</source>
        <translation>Změni&amp;t velikost obrázku podle velikosti rámce</translation>
    </message>
    <message>
        <source>Image Frame Properties</source>
        <translation>Vlastnosti obrázkových rámců</translation>
    </message>
    <message>
        <source>Image frames allow images to scale to any size</source>
        <translation>Velikost obrázků v obrázkových rámcích lze libovolně měnit</translation>
    </message>
    <message>
        <source>Images in image frames are scaled to the size of the frame</source>
        <translation>Velikost obrázků v obrázkových rámcích je upravena podle velikosti rámce</translation>
    </message>
    <message>
        <source>Automatically scaled images keep their original proportions</source>
        <translation>Automaticky škálované obrázky si ponechávají původní proporce</translation>
    </message>
    <message>
        <source>Fill color of image frames</source>
        <translation>Výplňová barva obrázkových rámců</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Dolní index</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Posunutí:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>Z&amp;většení:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Horní index</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Po&amp;sunutí:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Zvě&amp;tšení:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podtržené</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Posunutí:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Šířka linky:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Přeškrtnuté výš</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitálky</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Zvětše&amp;ní:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatic&amp;ké řádkování</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Řádkování:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Posunutí nad účaří písma</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativní velikost horního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Posunutí pod účaří písma</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativní velikost dolního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativní velikost kapitálek vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Procentuální zvětšení řádkování podle velikosti písma</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Posunutí pod běžné účaří vyjádřené jako procento spodního dotahu znaku</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Šířka řádku vyjádřená procentem velikosti písma</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Posunutí nad běžné účaří vyjádřené jako procento horního dotahu znaku</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation type="obsolete">Tečka</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Čárka</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Na střed</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Smazat vše</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Odsazení prvního řádku odstavce</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Odsazení celého odstavce zleva</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Smazat všechny tabelátory</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Pozice:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Tečka</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Spojovník</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podtržení</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Výplňový znak:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Vlastní:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Vlastní: </translation>
    </message>
    <message>
        <source>Period</source>
        <translation>Perioda</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Žádné</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Obrys</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Prvek</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Seskupit</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Volné objekty</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Stránka</translation>
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
        <translation>Posunití</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Šířka linky</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Přidat svislé vodítko</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Přidat vodorovné vodítko</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Odebrat svislé vodítko</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Odebrat vodorovné vodítko</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Přesunout svislé vodítko</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Přesunout vorodovné vodítko</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Zamknout vodítka</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Odemknout vodítka</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Přesunout</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Změnit velikost</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Rotovat</translation>
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
        <translation>Výběr</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Seskupit</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Výběr/seskupení</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Vytvořit</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
Š: %3, V: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Zarovnat/rozmístit</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Zahrnuté objekty</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Nastavit barvu výplně</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Barva1: %1, Barva2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Nastavit odstín výplňové barvy</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Nastavit barvu linky</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Nastavit barevný odstín linky</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Překlopit vodorovně</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Překlopit svisle</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Zamknout</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Odemknout</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Zamknout velikost</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Odemknout velikost</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Zrušit seskupení</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Smazat</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Přejmenovat</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Od %1
po %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Použít vzorovou stránku</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Vložit</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Vyjmout</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Nastavit průhlednost barvy výplně</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Nastavit průhlednost barvy linky</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Nastavit styl linky</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Nastavit styl konce linky</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Nastavit styl spoje linky</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Nastavit šířku linky</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Nastavit uživatelský styl linky</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Nepoužívat uživatelský styl linky</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Nastavit šipku pro začátek</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Nastavit šipku pro konec</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Vytvořit tabulku</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Řádky: %1, Sloupce: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Nastavit písmo</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Nastavit velikost písma</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Nastavit šířku písma</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Nastavit výšku písma</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Nastavit barvu výplně písma</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Nastavit barvu tahu písma</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Nastavit barevný odstín výplně písma</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Nastavit barevný odstín tahu písma</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Nastavit kerning</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Nastavit řádkování</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Nastavit styl odstavce</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Nastavit jazyk</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Zarovnání textu</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Nastavit efekt písma</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Obrázkový rámec</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Textový rámec</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnohoúhelník</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Beziérova křivka</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomená linka</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Konverze na</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Import SVG obrázku</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Import EPS obrázku</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Import souboru OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Pracovní prostor</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Text obtéká okolo rámce</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Text obtéká kolem bounding boxu</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Text obtéká kolem obrysové linky</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Text neobtéká</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Bez bounding boxu</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Bez obrysové linky</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Stránka %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Nastavit škálování obrázku</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Velikost rámce</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Volná změna velikosti</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Dodržet poměr stránek</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Porušit poměr stránek</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Upravit obrysovou linku</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Upravit tvar</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Znovu nastavit obrysovou linku</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Přidat stránku</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Přidat stránky</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Smazat stránku</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Smazat stránky</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Přidat vrstvu</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Přejmenovat vrstvu</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Vrstvu nahoru</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Vrstvu dolů</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Přesunout do vrstvy</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Povolit tisk vrstvy</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Zakázat tisk vrstvy</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Změnit název vrstvy</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Vložit obrázek</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text na křivky</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Tisk objektu povolen</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Tisk objektu zakázán</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Změnit offset obrázku</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Změnit měřítko obrázku</translation>
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
        <source>Reset control point</source>
        <translation>Vynulovat řídicí bod</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>Vynulovat řídicí body</translation>
    </message>
    <message>
        <source>Modify image effects</source>
        <translation>Upravit obrázkové efekty</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Původní stav</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Historie akcí</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Zobrazit pouze vybrané objekty</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
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
        <translation>Pokouším se získat aktualizační soubor Scribusu</translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(Z vašeho počítače nebudou odeslána žádná data)</translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation>Při pokusu o získání aktualizačního souboru vypršel čas.</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>Chyba při pokusu o stažení souboru s aktualizací %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>Soubor nebyl na serveru nalezen</translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>Nepodařilo se otevřít soubor s verzí %1 
Chyba %2 na řádku %3, sloupci %4</translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>Při hledání aktualizací Scribusu došlo k chybě. Zkontrolujte prosím své připojení k Internetu.</translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>Pro vaši verzi Scribusu (%1) nejsou k dispozici žádné aktualizace:</translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>Pro vaši verzi Scribusu (%1) je k dispozici jedna nebo více aktualizací:</translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation>Tento seznam může obsahovat i vývojové verze.</translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Podrobnosti najdete na www.scribus.net.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Minimální okraje pro velikost stránky %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Použít okraje tiskárny</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Vybrat &amp;tiskárnu:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Okraje</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Pravý:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
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
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Vložte hodnotu</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Vložte hodnotu a potom stiskněte OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Vložte hodnotu a potom stiskněte OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Předá vaši hodnotu skriptu</translation>
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
        <source>Button</source>
        <translation>Tlačítko</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textové pole</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Políčko k zaškrtnuti</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznam</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Spojení</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF nástroje</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Vložit PDF pole</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Vložit PDF anotace</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vyberte požadovaný import</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importovat text bez jakéhokoli formátování</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importer:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kódování:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importovat pouze text</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vyberte požadovaný import</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Zapamatovat si asociaci</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Zapamatovat si přířazení přípony souboru k importní aplikaci a příště se již nedotazovat, který program použít pro soubor tohoto typu.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nový ze šablony</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Použití</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Vytvořeno v</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Otevřít</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Získat nové šablony</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Instalace šablon</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Rozbalte archiv do adresáře šablon &lt;pre&gt;~/.scribus/templates&lt;/pre&gt; odkud budou přístupné pouze vám nebo do &lt;pre&gt;PREFIX/share/scribus/templates&lt;/pre&gt; odkud je uvidí všichni uživatelé.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Příprava šablony</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Odstranění šablony</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Překlad template.xml</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Šablony získáte na &lt;a href=&quot;http://www.scribus.net/&quot;&gt;www.scribus.net&lt;/a&gt; v sekci Download.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Ujistěte se, že použité obrázky mohou být použity všude. Také písma musí být zkontrolována. Jestliže písma nesmíte distribuovat, nevkládejte je do šablony. </translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Autor šablony by se měl také ujistit, že se jeho šablona korektně nainstaluje, což mimo jiné znamená, že se správně zachová v cizím systému.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Jestliže odstraníte šablonu z dialogu Nový ze šablony, odstraníte pouze záznam z template.xml. Soubory zůstanou na disku. Menu s možností vymazání se zobrazí pouze tehdy, jestliže máte právo zápisu do souboru template.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Zkopírujte existující template.xml a přejmenujte kopii na template.lang.xml (použijte stejný kód, jaký je ve jménu QM souboru jazyka). Např. template.cs.xml bude použito v Českém prostředí. Soubor musí být ve stejném adresáři jako původní.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Uložit jako šablonu</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategorie</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost stránky</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Použití</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Více detailů</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Méně detailů</translation>
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
        <translation>na šířku</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>na výšku</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>vlastní</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Vytvořit filtr</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Vyčistit</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Zvolit předchozí uložený filtr</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Zadejte jméno filtru</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Zadejte jméno filtru</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Povolit nebo zakázat tuto část filtru</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Odstranit tuto část filtru</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Přidat novou část filtru</translation>
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
        <translation>odstranit vzor</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>neodstraňovat vzor</translation>
    </message>
    <message>
        <source>words</source>
        <translation>slovy</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Nahradit</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Použít</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Hodnota vlevo je regulární výraz</translation>
    </message>
    <message>
        <source>with</source>
        <translation>čím</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>styl odstavce</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>všechny výskyty</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>všechny odstavce</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>odstavec začíná</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>odstavec s méně než</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>odstavec s více než</translation>
    </message>
</context>
</TS>
