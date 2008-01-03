<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS><TS version="1.1" language="cs_CZ">
<defaultcodec></defaultcodec>
<context>
    <name></name>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="23"/>
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
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="21"/>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Vrátí seznam se jmény všech barev v dokumentu. Jestliže není žádný dokument
otevřen, vrátí seznam implicitních barev.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddialog.h" line="22"/>
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
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="21"/>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Vrátí jméno výplňové barvy objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="23"/>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Posune objekt &quot;name&quot; relativně o dx a dy vůči aktuální pozici. Vzdálenosti
jsou vyjádřeny v délkových jednotkách dokumentu (viz. konstanty UNIT).
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt. Jestliže &quot;name&quot;
patří do nějaké skupiny, je posunuta celá skupina.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="22"/>
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
        <location filename="../plugins/scriptplugin/cmdobj.h" line="26"/>
        <source>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Creates a new rectangle on the current page and returns its name. The
coordinates are given in the current measurement units of the document
(see UNIT constants). &quot;name&quot; should be a unique identifier for the object
because you need this name to reference that object in future. If &quot;name&quot;
is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Vytvoří nový čtyřúhelník na aktuální stránce a vráti jeho jméno. X, Y, W a H koordináty
jsou dány ve zvolených měrných jednotkách dokumentu (viz. konstanty UNIT*).
&quot;name&quot; musí být unikátní řetězec, protože slouží jako identifikátor. Jestliže není
&quot;name&quot; zadáno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError, když se pokusíte zduplikovat jméno.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="22"/>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Nastaví gradient výplně objektu &quot;name&quot; na specifikovaný typ. Specifikace barev
je stejná jako v setFillColor() a setFillShade(). Dostupné gradienty viz.
konstanty FILL_&lt;typ&gt;.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="21"/>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Zapíše řetězec &quot;string&quot; do stavového řádku Scribusu. Text musí být kódován
UTF8 nebo unicode (např. unicode(&quot;text&quot;, &quot;iso-8859-2&quot;)).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="26"/>
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
        <location filename="../plugins/scriptplugin/svgimport.h" line="18"/>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="55"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="35"/>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;name&quot;]) -&gt; string

Vrátí jméno písma texttového rámce &quot;name&quot;. Jestliže je v rámu vybraný
nějaký text, pak vrátí písmo prvního znaku výběru.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="45"/>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

Vrátí délku textu textového rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="57"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="69"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="79"/>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Vrátí velikost řádkování textového rámce &quot;name&quot; vyjádřené v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="89"/>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Vrátí velikost mezisloupcové mezery textového rámce &quot;name&quot; vyjádřenou v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="99"/>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

Vrátí počet sloupců textového rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="111"/>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Vyplní obsah textového rámce &quot;name&quot; textem &quot;text&quot;. Text musí být v UTF8
kódování - použijte např. unicode(text, &apos;iso-8859-2&apos;). Viz. FAQ.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="138"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="152"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="165"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="177"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="189"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="202"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="216"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="227"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="238"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="248"/>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Nastaví obrys textu na barvu &quot;color&quot; v textovém rámci &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.h" line="261"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="274"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="291"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="304"/>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;])

Převede textový rámec &quot;name&quot; na křivky.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="35"/>
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
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="64"/>
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
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="94"/>
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

Smaže barvu &quot;name&quot;. každý výskyt této barvy je nahrazen  barvou &quot;replace&quot;.
Jestliže není &quot;replace&quot; uvedena, zamění mazanou barvu transparentní 
průhlednou &quot;None&quot;.
Jestliže není otevřený žádný dokument pracuje deleteColor s imlicitní
barevnou množinou. &quot;replace&quot; potom nemá žádnou funkčnost.

Může vyvolat výjimky NotFoundError (barva neexistuje) a ValueError
(chybné parametry).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="106"/>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Každý výskyt barvy &quot;name&quot; je nahrazen barvou &quot;replace&quot;.
Může vyvolat výjimky NotFoundError (barva neexistuje) a ValueError
(chybné parametry).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddialog.h" line="88"/>
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
        <location filename="../plugins/scriptplugin/cmddialog.h" line="101"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="108"/>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Zavře aktuální dokument bez výzvy k uložení.

Může vyvolat výjimku NoDocOpenError, kdyý enní žádný dokument otevřen</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="117"/>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Vrátí true, když je otevřený jakýkoli dokument.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="128"/>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Otevře dokument &quot;name&quot;.

Může vyvolat výjimku ScribusError, když dokument nejde otevřít.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="141"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="152"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="173"/>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Nastaví okraje dokumentu - levý, pravý, horní a spodní okraj je zadáván
v aktuálních měrných jednotkách dokumentu. Viz. konstanty UNIT_typ.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="185"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="196"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="206"/>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;)

Nahraje styly odstavce z dokumentu &quot;filename&quot; do dokumentu aktuálního.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="218"/>
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
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="31"/>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Vrátí jméno barvy čar objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="41"/>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

Vrátí tloušťku čáry objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="51"/>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer

Vrátí stín barvy objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="62"/>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Vrátí typ spojení čar objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Typy spojení: JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="73"/>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Vrátí typ ukončení čáry objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Typy jsou: CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="84"/>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Vrátí styl čáry objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Styly jsou: LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="94"/>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Vrátí stín výplně objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="115"/>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Vrátí tuple s velikostmi obrázku rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="125"/>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string

Vrátí jméno souboru (obrázku) daného rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="149"/>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (width,height)

Vrátí velikost (tuple) s velikostí rámce &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Velikost je vyjádřena v aktuálních měrných jednotkách (viz. UNIT_typ).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="160"/>
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
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="169"/>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Vrátí seznam, který obsahuje jména všech objektů na aktuální stránce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="35"/>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;name&quot;])

Přesune objekt &quot;name&quot; na nové místo. Paramety x a y jsou vyjádřeny v aktuálních
měrných jednotkách dokumentu (viz. konstanty UNIT).
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Jestliže onjekt &quot;name&quot; patří do skupiny onjektů, je posunuta celá skupina.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="48"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="69"/>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;])

Změní velikost objektu &quot;name&quot; na danou šířku &quot;width&quot; a výšku &quot;height&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="80"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="89"/>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Vrátí počet objektů ve výběru.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="98"/>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;)

Zařadí objekt &quot;name&quot; do výběru.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="107"/>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Zruší výběr všech objektů v celém dokumentu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="118"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="126"/>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;)

Zruší seskupení objektů, do kterého patří objekt &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="141"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="153"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="166"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="177"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="187"/>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

Vrátí true kdyý je objekt &quot;name&quot; zamčený.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="31"/>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Vrátí seznam se jmény dostupných písem.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="41"/>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list of tuples

Vrátí více informací o dostupných písmech. Seznam obsahuje tupple:
[(Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="69"/>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Vrátí seznam se jmény všech vrstev.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="81"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="90"/>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Vrátí jméno aktuální vrstvy.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="103"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="116"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="129"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="170"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="181"/>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer)

Vytvoří novou vrstvu se jménem &quot;layer&quot;.

Může vyvolat výjimku ValueError v případě chyby.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="190"/>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Vrátí řetězec s kódem jazyka, ve kterém Scribus běží (viz. přepínač --lang xx).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="43"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="60"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="75"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="90"/>
        <source>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

Creates a new line from the point(x1, y1) to the point(x2, y2) and returns
its name. The coordinates are given in the current measurement unit of the
document (see UNIT constants). &quot;name&quot; should be a unique identifier for the
object because you need this name for further access to that object. If
&quot;name&quot; is not given Scribus will create one for you.

May raise NameExistsError if you explicitly pass a name that&apos;s already used.
</source>
        <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

Vytvoří novou čáru na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="108"/>
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

Vytvoří novou lomenou čáru na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů má tvar: [x1, y1, x2, y2, ..., xn, yn].

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="128"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů objeku má tvar: [x1, y1, x2, y2, ..., xn, yn] a jsou nutné alespoň
tři body. Mnohoúhelník je automaticky uzavřen, takže není třeba zadávat poslední
bod jako první.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="149"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Seznam bodů objeku má tvar: [x1, y1, kx1, ky1, x2, y2, kx2, ky2, ..., xn, yn, kxn, kyn].
x a y jsou koordináty bodů, kx a ky jsou koordináty řídícího bodu křivky.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku ValueError v případě špatných koordinátů.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="165"/>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
Jméno &quot;name&quot; musí být unikátní řetězec.
Jestliže není &quot;name&quot; uvedeno, Scribus jméno vytvoří sám.
Text na křivce vyznikne ze dvou objektů - textového rámce &quot;textbox&quot; a Bezierovské
křivky &quot;beziercurve&quot;.

Může vyvolat výjimku NameExistsError když zadáte jméno, které již existuje.
Může vyvolat výjimku NotFoundError v případě neexistujících objektů.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="177"/>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;])

Smaže objekt &quot;name&quot;. Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="189"/>
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
        <location filename="../plugins/scriptplugin/cmdobj.h" line="206"/>
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
        <location filename="../plugins/scriptplugin/cmdobj.h" line="222"/>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Aplikuje styl &quot;style&quot; na objekt &quot;name&quot;. Jestliže není &quot;name&quot; uvedeno,
použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="235"/>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Vrátí seznam všech jmen stylů odstavce v dokumentu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="36"/>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Vrátí číslo aktuální stránky dokumentu. Stránky jsou číslovány od 1, přičemž nezáleží
na nastaveném čísle první stránky.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="45"/>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>Překreslí/obnoví všechny stránky.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="56"/>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>Uloží aktuální stránku jako EPS do souboru &quot;name&quot;.

Může vyvolat výjimu ScribusErro, dojde-li k chybě.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="69"/>
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
        <location filename="../plugins/scriptplugin/cmdpage.h" line="82"/>
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
        <location filename="../plugins/scriptplugin/cmdpage.h" line="91"/>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>Vrátí počet stránek dokumentu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="101"/>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Vrátí seznam s pozicemi horizontálních vodítek. Hodnoty jsou v aktuálních
měrných jednotkách. Viz. konstanty UNIT.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="114"/>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(list)

Nastaví horizontální vodítka. Vstupní parametr je seznam jejich pozicí
v aktuálních měrných jednotkách (viz. konstanty UNIT).
Např.:
     setHGuides(getHGuides()) + [200.0, 210.0] # prida voditka
     setHGuides([90, 250]) # smaze stara a nastavi nova voditka</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="123"/>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>Viz. getHGuides().</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="132"/>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>Viz. setHGuides().</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="142"/>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Vrátí tuple s rozměry stránky v aktuálních měrných jednotkách. Viz. konstanty UNIT
a getPageMargins()</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.h" line="158"/>
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
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="33"/>
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
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="43"/>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Nastaví barvu &quot;color&quot; čar objetu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="56"/>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(width, [&quot;name&quot;])

Nastaví šířku čáry objektu &quot;name&quot; na hodnotu &quot;width&quot;. &quot;width&quot; musí
být v intervalu &lt;0.0; 12.0&gt; a je v bodech.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolatvýjimku ValueError, když není hodnota v intervalu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="70"/>
        <source>setLineShade(shade, [&quot;name&quot;])

Sets the shading of the line color of the object &quot;name&quot; to &quot;shade&quot;.
&quot;shade&quot; must be an integer value in the range from 0 (lightest) to 100
(full color intensity). If &quot;name&quot; is not given the currently selected item
is used.

May raise ValueError if the line shade is out of bounds.
</source>
        <translation>setLineShade(shade, [&quot;name&quot;])

Nastaví stín čar objektu &quot;name&quot; na hodnotu &quot;shade&quot;. &quot;shade&quot; musí
být celé číslo z intervalu &lt;0; 100&gt;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku ValueError, když je hodnota mimo interval.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="81"/>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Nastaví typ spojení čar objektu &quot;name&quot; na styl &quot;join&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz. předdefinované konstanty JOIN_*.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="92"/>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;name&quot;])

Nastaví styl konce čar objektu &quot;name&quot; na styl &quot;cap&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz. předdefinované konstanty CAP_*.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="103"/>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;])

Nastaví styl čáry objektu &quot;name&quot; na styl &quot;style&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz. předdefinované konstanty LINE_*.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="117"/>
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
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="129"/>
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
        <location filename="../plugins/scriptplugin/cmdsetprop.h" line="141"/>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Nastaví styl čar objektu &quot;name&quot; na definovaný styl &quot;namedStyle&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku NotFoundError, jestliže styl neexistuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="34"/>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Zruší předchozí nastavení progress baru. Je to vhodné použít před novým použitím P.B.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="47"/>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Nastaví maximální možný počet kroků (zaplnění) progress baru.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="61"/>
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
Po použití P.B. je vhodné jej vynulovat, t.j. použít progressReset(). Viz. dokumentace
Qt</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="69"/>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>[UNSUPPORTED!] This might break things, so steer clear for now.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="83"/>
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
        <location filename="../plugins/scriptplugin/cmdmani.h" line="199"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="157"/>
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
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="60"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="332"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="344"/>
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
        <location filename="../plugins/scriptplugin/cmdpage.h" line="172"/>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="50"/>
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
        <location filename="../plugins/scriptplugin/cmdcolor.h" line="77"/>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddialog.h" line="46"/>
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
        <location filename="../plugins/scriptplugin/cmddialog.h" line="111"/>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="105"/>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.h" line="137"/>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="82"/>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="100"/>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="134"/>
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
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="164"/>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="183"/>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.h" line="200"/>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.h" line="59"/>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.h" line="143"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="125"/>
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
        <location filename="../plugins/scriptplugin/cmdtext.h" line="318"/>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="96"/>
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
        <location filename="../plugins/scriptplugin/cmddoc.h" line="226"/>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="233"/>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="241"/>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="249"/>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="256"/>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/guiapp.h" line="95"/>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.h" line="162"/>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.h" line="247"/>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <location filename="../about.cpp" line="157"/>
        <source>Contributions from:</source>
        <translation>Příspěvky od:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="138"/>
        <source>&amp;About</source>
        <translation>O &amp;Scribusu</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="206"/>
        <source>A&amp;uthors</source>
        <translation>&amp;Autoři</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="374"/>
        <source>&amp;Translations</source>
        <translation>&amp;Překlady</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="395"/>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="417"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="144"/>
        <source>Development Team:</source>
        <translation>Vývojový tým:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="187"/>
        <source>Official Documentation:</source>
        <translation>Oficiální dokumentace:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="196"/>
        <source>Other Documentation:</source>
        <translation>Ostatní dokumentace:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="380"/>
        <source>Homepage</source>
        <translation>Webová stránka</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="382"/>
        <source>Online Reference</source>
        <translation>Online odkazy</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="386"/>
        <source>Bugs and Feature Requests</source>
        <translation>Chyby a požadavky na změnu</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="388"/>
        <source>Mailing List</source>
        <translation>Mailová skupina</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="214"/>
        <source>Official Translations and Translators:</source>
        <translation>Oficiální překlady a překladatelé:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="333"/>
        <source>Previous Translation Contributors:</source>
        <translation>Předchozí přispěvatelé:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="40"/>
        <source>About Scribus %1</source>
        <translation>O Scribusu %1</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="384"/>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="65"/>
        <source>%1 %2 %3</source>
        <translation>%1 %2 %3</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="73"/>
        <source>%3-%2-%1 %4 %5</source>
        <translation>%3-%2-%1 %4 %5</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="133"/>
        <source>Using Ghostscript version %1</source>
        <translation>Ghostscript verze %1</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="135"/>
        <source>No Ghostscript version available</source>
        <translation>Ghostscript není dostupný</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="136"/>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus verze %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="136"/>
        <source>Build ID:</source>
        <translation>Build ID:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="426"/>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Okno ukazuje verzi programu, datum kompilace a knihovny v něm použité. Symboly C-C-T-F znamenají C-littlecms C-CUPS T-TIFF F-Fontconfig podporu. Poslední písmeno uvádí způsob vykreslení C - Cairo A - Libart. Chybějící knihovny jsou zobrazeny jako *. Také je zde uvedena verze Ghostscriptu, kterou Scribus našel.</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="175"/>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Mac OS&amp;#174; X Aqua Port:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="183"/>
        <source>Windows&amp;#174; Port:</source>
        <translation>Windows&amp;#174; Port:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="201"/>
        <source>Tango Project Icons:</source>
        <translation>Ikony projektu Tango:</translation>
    </message>
    <message>
        <location filename="../about.cpp" line="179"/>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>OS/2&amp;#174;/eComStation&amp;#8482; Port:</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <location filename="../aboutplugins.cpp" line="67"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="67"/>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="64"/>
        <source>Filename:</source>
        <translation>Soubor:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="65"/>
        <source>Version:</source>
        <translation>Verze:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="68"/>
        <source>Enabled:</source>
        <translation>Povoleno:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="69"/>
        <source>Release Date:</source>
        <translation>Datum vydání:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="76"/>
        <source>Description:</source>
        <translation>Popis:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="77"/>
        <source>Author(s):</source>
        <translation>Autor/Autoři:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="78"/>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <location filename="../aboutplugins.cpp" line="79"/>
        <source>License:</source>
        <translation>Licence:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <location filename="../aboutpluginsbase.ui" line="16"/>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: O modulech</translation>
    </message>
    <message>
        <location filename="../aboutpluginsbase.ui" line="110"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../aboutpluginsbase.ui" line="113"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <location filename="../actionmanager.cpp" line="870"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="871"/>
        <source>&amp;Open...</source>
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="872"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="873"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="874"/>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="875"/>
        <source>Re&amp;vert to Saved</source>
        <translation>Návrat k u&amp;loženému</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="876"/>
        <source>Collect for O&amp;utput...</source>
        <translation>Ad&amp;resář pro výstup...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="877"/>
        <source>Get Text...</source>
        <translation>Vložit text...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="878"/>
        <source>Append &amp;Text...</source>
        <translation>&amp;Připojit text...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="879"/>
        <source>Get Image...</source>
        <translation>Vložit obrázek...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="880"/>
        <source>Save &amp;Text...</source>
        <translation>Uložit &amp;text...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="881"/>
        <source>Save Page as &amp;EPS...</source>
        <translation>Uložit stranu jako &amp;EPS...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="882"/>
        <source>Save as P&amp;DF...</source>
        <translation>Uložit jako P&amp;DF...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="883"/>
        <source>Document &amp;Setup...</source>
        <translation>Na&amp;stavení dokumentu...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="884"/>
        <source>&amp;Print...</source>
        <translation>&amp;Tisk...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="886"/>
        <source>&amp;Quit</source>
        <translation>&amp;Konec</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="888"/>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="889"/>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="890"/>
        <source>&amp;Item Action Mode</source>
        <translation>Reži&amp;m hromadných akcí</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="891"/>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="894"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="895"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="898"/>
        <source>Select &amp;All</source>
        <translation>Vybr&amp;at vše</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="899"/>
        <source>&amp;Deselect All</source>
        <translation>&amp;Zrušit výběr</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="900"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hledat/Nahradit...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="901"/>
        <source>Edit Image...</source>
        <translation>Upravit obrázek...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="902"/>
        <source>C&amp;olors...</source>
        <translation>&amp;Barvy...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="904"/>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Styly odstavce...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="905"/>
        <source>&amp;Line Styles...</source>
        <translation>Styly ča&amp;r...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="906"/>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Vzorové strany...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="908"/>
        <source>P&amp;references...</source>
        <translation>N&amp;astavení...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="915"/>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="930"/>
        <source>&amp;Other...</source>
        <translation>&amp;Jiný...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="918"/>
        <source>&amp;Left</source>
        <translation>V&amp;levo</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="919"/>
        <source>&amp;Center</source>
        <translation>Na &amp;střed</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="920"/>
        <source>&amp;Right</source>
        <translation>V&amp;pravo</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="921"/>
        <source>&amp;Block</source>
        <translation>Do &amp;bloku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="922"/>
        <source>&amp;Forced</source>
        <translation>&amp;Vynucené</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="927"/>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="931"/>
        <source>&amp;Normal</source>
        <translation>&amp;Normální</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="932"/>
        <source>&amp;Underline</source>
        <translation>Po&amp;dtržení</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="933"/>
        <source>Underline &amp;Words</source>
        <translation>Podtržená &amp;slova</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="934"/>
        <source>&amp;Strike Through</source>
        <translation>Př&amp;eškrtnuté</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="935"/>
        <source>&amp;All Caps</source>
        <translation>&amp;Verzálky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="936"/>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapitálky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="937"/>
        <source>Su&amp;perscript</source>
        <translation>&amp;Horní index</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="938"/>
        <source>Su&amp;bscript</source>
        <translation>&amp;Dolní index</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="940"/>
        <source>S&amp;hadow</source>
        <translation>&amp;Stín</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="942"/>
        <source>&amp;Image Effects</source>
        <translation>&amp;Efekty obrázku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="943"/>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabelátory...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="946"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="947"/>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Vícenásobné duplikování</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="948"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="949"/>
        <source>&amp;Group</source>
        <translation>&amp;Seskupit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="950"/>
        <source>&amp;Ungroup</source>
        <translation>&amp;Zrušit seskupení</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="951"/>
        <source>Is &amp;Locked</source>
        <translation>Je &amp;zamčeno</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="952"/>
        <source>Si&amp;ze is Locked</source>
        <translation>&amp;Velikost je zamčena</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="956"/>
        <source>Lower to &amp;Bottom</source>
        <translation>Přesunout do nej&amp;nižší vrstvy</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="957"/>
        <source>Raise to &amp;Top</source>
        <translation>Přesunout do nej&amp;vyšší vrstvy</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="958"/>
        <source>&amp;Lower</source>
        <translation>Přesunout o &amp;vrstvu níž</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="959"/>
        <source>&amp;Raise</source>
        <translation>Přesunout o v&amp;rstvu výš</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="960"/>
        <source>Send to S&amp;crapbook</source>
        <translation>&amp;Poslat do zápisníku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="961"/>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atributy...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="963"/>
        <source>I&amp;mage Visible</source>
        <translation>Obrázek je vi&amp;ditelný</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="964"/>
        <source>&amp;Update Image</source>
        <translation>&amp;Aktualizovat obrázek</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="965"/>
        <source>Adjust Frame to Image</source>
        <translation>Přizpůsobit rám obrázku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="966"/>
        <source>Extended Image Properties</source>
        <translation>Rozšířené vlastnosti obrázku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="967"/>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Nízké rozlišení</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="968"/>
        <source>&amp;Normal Resolution</source>
        <translation>N&amp;ormální rozlišení</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="969"/>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Plné rozlišení</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="970"/>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Je PDF &amp;záložkou</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="971"/>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Je PDF &amp;anotací</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="972"/>
        <source>Annotation P&amp;roperties</source>
        <translation>Vlast&amp;nosti anotace</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="973"/>
        <source>Field P&amp;roperties</source>
        <translation>Vlastnosti &amp;pole</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="974"/>
        <source>&amp;Edit Shape...</source>
        <translation>Upravit &amp;obrys...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="975"/>
        <source>&amp;Attach Text to Path</source>
        <translation>Připojit &amp;text ke křivce</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="976"/>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Odpojit text od křivky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="977"/>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Kombinovat mnohoúhelníky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="978"/>
        <source>Split &amp;Polygons</source>
        <translation>&amp;Rozdělit mnohoúhelníky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1045"/>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Beziérova křivka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1040"/>
        <source>&amp;Image Frame</source>
        <translation>&amp;Obrázkový rám</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1043"/>
        <source>&amp;Polygon</source>
        <translation>&amp;Mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1039"/>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textový rám</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="986"/>
        <source>&amp;Glyph...</source>
        <translation>&amp;Znak...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="987"/>
        <source>Sample Text</source>
        <translation>Výplňový text</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="990"/>
        <source>&amp;Insert...</source>
        <translation>&amp;Vložit...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="991"/>
        <source>Im&amp;port...</source>
        <translation>&amp;Import...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="992"/>
        <source>&amp;Delete...</source>
        <translation>&amp;Smazat...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="993"/>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopírovat...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="994"/>
        <source>&amp;Move...</source>
        <translation>&amp;Přesunout...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="995"/>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Použít vzorovou stranu...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="997"/>
        <source>Manage &amp;Guides...</source>
        <translation>O&amp;vládání vodítek...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="998"/>
        <source>Manage Page Properties...</source>
        <translation>Vlastnosti strany...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1001"/>
        <source>&amp;Fit in window</source>
        <translation>&amp;Přizpůsobit oknu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1002"/>
        <source>&amp;50%</source>
        <translation>&amp;50%</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1003"/>
        <source>&amp;75%</source>
        <translation>&amp;75%</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1004"/>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1005"/>
        <source>&amp;200%</source>
        <translation>&amp;200%</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1007"/>
        <source>&amp;Thumbnails</source>
        <translation>M&amp;iniatury</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1008"/>
        <source>Show &amp;Margins</source>
        <translation>Zobrazit o&amp;kraje</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1009"/>
        <source>Show &amp;Frames</source>
        <translation>Zobrazit &amp;rámy</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1010"/>
        <source>Show &amp;Images</source>
        <translation>Zobrazit &amp;obrázky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1011"/>
        <source>Show &amp;Grid</source>
        <translation>Zobrazit &amp;mřížku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1012"/>
        <source>Show G&amp;uides</source>
        <translation>Zobrazit &amp;vodítka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1014"/>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1015"/>
        <source>Show &amp;Text Chain</source>
        <translation>Zob&amp;razit řetězení textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1016"/>
        <source>Show Control Characters</source>
        <translation>Zobrazit řídicí znaky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1018"/>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke straně</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1019"/>
        <source>Sn&amp;ap to Grid</source>
        <translation>M&amp;agnetická mřížka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1020"/>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Magnetická vodítka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1024"/>
        <source>&amp;Properties</source>
        <translation>&amp;Vlastnosti</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1026"/>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Zápisník</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1027"/>
        <source>&amp;Layers</source>
        <translation>V&amp;rstvy</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1028"/>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Uspořádat strany</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1029"/>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1030"/>
        <source>&amp;Measurements</source>
        <translation>&amp;Vzdálenosti</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1031"/>
        <source>Action &amp;History</source>
        <translation>&amp;Historie akcí</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1032"/>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Předtisková kontrola</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1033"/>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Zarovnat a rozmístit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1034"/>
        <source>&amp;Tools</source>
        <translation>&amp;Nástroje</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1035"/>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF nástroje</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1038"/>
        <source>Select Item</source>
        <translation>Vybrat objekt</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1041"/>
        <source>T&amp;able</source>
        <translation>&amp;Tabulka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1042"/>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1044"/>
        <source>&amp;Line</source>
        <translation>Čá&amp;ra</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1046"/>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Kresba od ruky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1047"/>
        <source>Rotate Item</source>
        <translation>Otočit prvek</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1048"/>
        <source>Zoom in or out</source>
        <translation>Zvětšit nebo zmenšit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1049"/>
        <source>Zoom in</source>
        <translation>Zvětšit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1050"/>
        <source>Zoom out</source>
        <translation>Zmenšit</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1051"/>
        <source>Edit Contents of Frame</source>
        <translation>Upravit obsah rámu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1052"/>
        <source>Edit Text...</source>
        <translation>Upravit text...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1053"/>
        <source>Link Text Frames</source>
        <translation>Vytvořit řetězení textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1054"/>
        <source>Unlink Text Frames</source>
        <translation>Přerušit řetězení textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1055"/>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Barevná pipeta</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1056"/>
        <source>Copy Item Properties</source>
        <translation>Kopírovat vlastnosti objektu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1058"/>
        <source>Edit the text with the Story Editor</source>
        <translation>Upravit text v zápisníku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1060"/>
        <source>Insert Text Frame</source>
        <translation>Vložit textový rámec</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1061"/>
        <source>Insert Image Frame</source>
        <translation>Vložit obrázjový rámec</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1062"/>
        <source>Insert Table</source>
        <translation>Vložit tabulku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1063"/>
        <source>Insert Shape</source>
        <translation>Vložit tvar</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1064"/>
        <source>Insert Polygon</source>
        <translation>Vložit mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1065"/>
        <source>Insert Line</source>
        <translation>Vložit čáru</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1066"/>
        <source>Insert Bezier Curve</source>
        <translation>Vložit Beziérovu křivku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1067"/>
        <source>Insert Freehand Line</source>
        <translation>Vložit čáru kreslenou volnou rukou</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1070"/>
        <source>&amp;Manage Pictures</source>
        <translation>Správa &amp;obrázků</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1071"/>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Dělení slov v textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1072"/>
        <source>Dehyphenate Text</source>
        <translation>Zrušit dělení slov textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1073"/>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Generovat obsah</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1080"/>
        <source>&amp;About Scribus</source>
        <translation>&amp;O Scribusu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1082"/>
        <source>About &amp;Qt</source>
        <translation>O &amp;Qt</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1083"/>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Tipy pro nástroje</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1084"/>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;manuál...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1096"/>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Chytré dělení textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1097"/>
        <source>Non Breaking Dash</source>
        <translation>Nedělitelná pomlčka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1098"/>
        <source>Non Breaking &amp;Space</source>
        <translation>Nedělitelná &amp;mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1099"/>
        <source>Page &amp;Number</source>
        <translation>Číslo &amp;strany</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1148"/>
        <source>New Line</source>
        <translation>Nový řádek</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1149"/>
        <source>Frame Break</source>
        <translation>Zalomení rámce</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1150"/>
        <source>Column Break</source>
        <translation>Zalomení sloupce</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1103"/>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1104"/>
        <source>Registered Trademark</source>
        <translation>Registrovaná obchodní známka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1105"/>
        <source>Trademark</source>
        <translation>Obchodní známka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1107"/>
        <source>Bullet</source>
        <translation>Odrážka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1109"/>
        <source>Em Dash</source>
        <translation>En dash</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1110"/>
        <source>En Dash</source>
        <translation>En dash</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1111"/>
        <source>Figure Dash</source>
        <translation>Figure dash</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1112"/>
        <source>Quotation Dash</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1118"/>
        <source>Apostrophe</source>
        <translation>Apostrof</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1119"/>
        <source>Straight Double</source>
        <translation>Přímé dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1120"/>
        <source>Single Left</source>
        <translation>Levé jednoduché</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1121"/>
        <source>Single Right</source>
        <translation>Pravé jednoduché</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1122"/>
        <source>Double Left</source>
        <translation>Levé dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1123"/>
        <source>Double Right</source>
        <translation>Pravé dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1124"/>
        <source>Single Reversed</source>
        <translation>Jednoduché obrácené</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1125"/>
        <source>Double Reversed</source>
        <translation>Dvojité obrácené</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1126"/>
        <source>Single Left Guillemet</source>
        <translation>Jednoduché levé Guillemet (francouzské)</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1127"/>
        <source>Single Right Guillemet</source>
        <translation>Jednoduché pravé Guillemet (francouzské)</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1128"/>
        <source>Double Left Guillemet</source>
        <translation>Dvojité levé Guillemet (francouzské)</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1129"/>
        <source>Double Right Guillemet</source>
        <translation>Dvojité pravé Guillemet (framcouzské)</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1130"/>
        <source>Low Single Comma</source>
        <translation>Dolní jednoduché</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1131"/>
        <source>Low Double Comma</source>
        <translation>Dolní dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1132"/>
        <source>CJK Single Left</source>
        <translation>Asijské levé jednoduché</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1133"/>
        <source>CJK Single Right</source>
        <translation>Asijské pravé jednoduché</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1134"/>
        <source>CJK Double Left</source>
        <translation>Asijské levé dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1135"/>
        <source>CJK Double Right</source>
        <translation>Asijské pravé dvojité</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1087"/>
        <source>Toggle Palettes</source>
        <translation>Přepnout palety</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1088"/>
        <source>Toggle Guides</source>
        <translation>Přepnout vodítka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="885"/>
        <source>Print Previe&amp;w</source>
        <translation>Ukázka pře&amp;d tiskem</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="907"/>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;Javaskripty...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="996"/>
        <source>Convert to Master Page...</source>
        <translation>Převést na vzorovou stranu...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1076"/>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskáda</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1077"/>
        <source>&amp;Tile</source>
        <translation>&amp;Dlaždice</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1081"/>
        <source>&amp;About Plug-ins</source>
        <translation>O &amp;Modulech</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="962"/>
        <source>More Info...</source>
        <translation>Více informací...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="953"/>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Tisk povolen</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="954"/>
        <source>&amp;Flip Horizontally</source>
        <translation>Překlopit &amp;vodorovně</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="955"/>
        <source>&amp;Flip Vertically</source>
        <translation>Překlopit &amp;svisle</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1017"/>
        <source>Show Rulers</source>
        <translation>Zobrazit pravítka</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1025"/>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Objekty</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1106"/>
        <source>Solidus</source>
        <translation>Lomítko</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1108"/>
        <source>Middle Dot</source>
        <translation>Middle Dot</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1137"/>
        <source>En Space</source>
        <translation>En mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1138"/>
        <source>Em Space</source>
        <translation>Em mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1139"/>
        <source>Thin Space</source>
        <translation>Úzká mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1140"/>
        <source>Thick Space</source>
        <translation>Široká mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1141"/>
        <source>Mid Space</source>
        <translation>Střední mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1142"/>
        <source>Hair Space</source>
        <translation>Vlasová mezera</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1144"/>
        <source>Insert Smart Hyphen</source>
        <translation>Vložit chytré dělení textu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1145"/>
        <source>Insert Non Breaking Dash</source>
        <translation>Vložit nedělitelnou pomlčku</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1146"/>
        <source>Insert Non Breaking Space</source>
        <translation>Vložit nedělitelnou mezeru</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1147"/>
        <source>Insert Page Number</source>
        <translation>Vložit číslo strany</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1152"/>
        <source>ff</source>
        <translation>ff</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1153"/>
        <source>fi</source>
        <translation>fi</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1154"/>
        <source>fl</source>
        <translation>fl</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1155"/>
        <source>ffi</source>
        <translation>ffi</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1156"/>
        <source>ffl</source>
        <translation>ffl</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1157"/>
        <source>ft</source>
        <translation>ft</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1158"/>
        <source>st</source>
        <translation>st</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="903"/>
        <source>S&amp;tyles...</source>
        <translation>S&amp;tyly...</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="939"/>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Obrys</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="981"/>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>O&amp;brysy</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="896"/>
        <source>Paste (&amp;Absolute)</source>
        <translation>Vložit (&amp;Absolutně)</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="897"/>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1013"/>
        <source>Show Text Frame Columns</source>
        <translation>Ukázat sloupce textového rámu</translation>
    </message>
    <message>
        <location filename="../actionmanager.cpp" line="1006"/>
        <source>&amp;400%</source>
        <translation>&amp;400%</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <location filename="../aligndistribute.cpp" line="230"/>
        <source>Align and Distribute</source>
        <translation>Zarovnat a rozmístit</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="231"/>
        <source>Align</source>
        <translation>Zarovnat</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="232"/>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativně k:</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="235"/>
        <source>First Selected</source>
        <translation>Prvně vybranému</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="236"/>
        <source>Last Selected</source>
        <translation>Posledně vybranému</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="237"/>
        <source>Page</source>
        <translation>Straně</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="238"/>
        <source>Margins</source>
        <translation>Okrajům</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="239"/>
        <source>Guide</source>
        <translation>Vodítkům</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="240"/>
        <source>Selection</source>
        <translation>Výběrům</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="244"/>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Zarovnat pravé strany objektů na levou stranu kotvy</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="246"/>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Zarovnat levé strany na pravou stranu kotvy</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="248"/>
        <source>Align bottoms</source>
        <translation>Zarovnat dole</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="250"/>
        <source>Align right sides</source>
        <translation>Zarovnat pravé strany</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="252"/>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Zarovnat horní části objektů na spodní část ukotvení</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="254"/>
        <source>Center on vertical axis</source>
        <translation>Vystředit na svislé osy</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="256"/>
        <source>Align left sides</source>
        <translation>Zarovnat levé strany</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="258"/>
        <source>Center on horizontal axis</source>
        <translation>Vystředit na vodorovné osy</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="260"/>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Zarovnat spodní části objektů na horní část ukotvení</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="262"/>
        <source>Align tops</source>
        <translation>Zarovnat nahoře</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="264"/>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Vybrané vodítko:</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="266"/>
        <source>Distribute</source>
        <translation>Rozmístit</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="268"/>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Sjednotit vodorovné mezery mezi objekty</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="270"/>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Sjednotit vodorovné mezery mezi objekty na zadanou hodnotu</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="272"/>
        <source>Distribute right sides equidistantly</source>
        <translation>Rozmístit pravé strany ve stejné vzdálenosti</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="274"/>
        <source>Distribute bottoms equidistantly</source>
        <translation>Rozmístit spodní části ve stejné vzdálenosti</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="276"/>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Rozmístit středy ve stejné vzdálenosti vodorovně</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="278"/>
        <source>Make vertical gaps between objects equal</source>
        <translation>Sjednotit svislé odsazení mezi objekty</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="280"/>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Sjednotit svislé mezery mezi objekty na zadanou hodnotu</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="282"/>
        <source>Distribute left sides equidistantly</source>
        <translation>Rozmístit levé strany ve stejné vzdálenosti</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="284"/>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Rozmístit středy ve stejné vzdálenosti svisle</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="286"/>
        <source>Distribute tops equidistantly</source>
        <translation>Rozmístit horní části ve stejné vzdálenosti</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="288"/>
        <source>&amp;Distance:</source>
        <translation>&amp;Vzdálenost:</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="289"/>
        <source>Distribute the items with the distance specified</source>
        <translation>Rozmístit objekty na určenou vzdálenost</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="291"/>
        <source>None Selected</source>
        <translation>Nic nevybráno</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="399"/>
        <source>Some objects are locked.</source>
        <translation>Některé objekty jsou uzamčeny.</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="400"/>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odemknout vše</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="1276"/>
        <source>Y: %1%2</source>
        <translation>Y: %1%2</translation>
    </message>
    <message>
        <location filename="../aligndistribute.cpp" line="1284"/>
        <source>X: %1%2</source>
        <translation>X: %1%2</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <location filename="../alignselect.cpp" line="91"/>
        <source>Align Text Left</source>
        <translation>Zarovnat text doleva</translation>
    </message>
    <message>
        <location filename="../alignselect.cpp" line="92"/>
        <source>Align Text Right</source>
        <translation>Zarovnat text doprava</translation>
    </message>
    <message>
        <location filename="../alignselect.cpp" line="93"/>
        <source>Align Text Center</source>
        <translation>Zarovnat text na střed</translation>
    </message>
    <message>
        <location filename="../alignselect.cpp" line="94"/>
        <source>Align Text Justified</source>
        <translation>Zarovnat text vyrovnaně</translation>
    </message>
    <message>
        <location filename="../alignselect.cpp" line="95"/>
        <source>Align Text Forced Justified</source>
        <translation>Zarovnat text vynuceně vyrovnaně</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <location filename="../annot.cpp" line="38"/>
        <source>Field Properties</source>
        <translation>Vlastnosti pole</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="496"/>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="73"/>
        <source>Button</source>
        <translation>Tlačítko</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="73"/>
        <source>Text Field</source>
        <translation>Textové pole</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="73"/>
        <source>Check Box</source>
        <translation>Políčko k zaškrtnuti</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="73"/>
        <source>Combo Box</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="73"/>
        <source>List Box</source>
        <translation>Seznam</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="87"/>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="96"/>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="109"/>
        <source>Tool-Tip:</source>
        <translation>Tipy pro nástroje:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="250"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="148"/>
        <source>Border</source>
        <translation>Ohraničení</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="155"/>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1992"/>
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="171"/>
        <source>Width:</source>
        <translation>Šířka:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="175"/>
        <source>Thin</source>
        <translation>Tenký</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="285"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="175"/>
        <source>Wide</source>
        <translation>Široké</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="183"/>
        <source>Style:</source>
        <translation>Styl:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="187"/>
        <source>Solid</source>
        <translation>Bez přerušení</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="187"/>
        <source>Dashed</source>
        <translation>Přerušovaný</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="187"/>
        <source>Underline</source>
        <translation>Podtržené</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="188"/>
        <source>Beveled</source>
        <translation>Zkosený</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="188"/>
        <source>Inset</source>
        <translation>Příloha</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="198"/>
        <source>Other</source>
        <translation>Jiné</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="205"/>
        <source>Read Only</source>
        <translation>Jen ke čtení</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="211"/>
        <source>Required</source>
        <translation>Požadované</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="217"/>
        <source>Don&apos;t Export Value</source>
        <translation>Neexportovat hodnotu</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="223"/>
        <source>Visibility:</source>
        <translation>Viditelnost:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="227"/>
        <source>Visible</source>
        <translation>Viditelné</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="227"/>
        <source>Hidden</source>
        <translation>Skryté</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="227"/>
        <source>No Print</source>
        <translation>Bez tisku</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="228"/>
        <source>No View</source>
        <translation>Bez náhledu</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="237"/>
        <source>Appearance</source>
        <translation>Vzhled</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="257"/>
        <source>Text for Button Down</source>
        <translation>Text pro tlačítko Dolů</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="260"/>
        <source>Text for Roll Over</source>
        <translation>Text pro přetočení</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="271"/>
        <source>Icons</source>
        <translation>Ikony</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="278"/>
        <source>Use Icons</source>
        <translation>Použít ikony</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="327"/>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="301"/>
        <source>Pressed</source>
        <translation>Stlačené</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="317"/>
        <source>Roll Over</source>
        <translation>Přetočit</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="334"/>
        <source>Icon Placement...</source>
        <translation>Umístění ikon...</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="381"/>
        <source>Highlight</source>
        <translation>Zvýraznění</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="389"/>
        <source>Invert</source>
        <translation>Invertovat</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="389"/>
        <source>Outlined</source>
        <translation>Obkreslené</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="389"/>
        <source>Push</source>
        <translation>Stisknout</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="407"/>
        <source>Multi-Line</source>
        <translation>Víceřádkový</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="411"/>
        <source>Password</source>
        <translation>Heslo</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="416"/>
        <source>Limit of</source>
        <translation>Omezení</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="427"/>
        <source>Characters</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="433"/>
        <source>Do Not Scroll</source>
        <translation>Neposouvat</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="437"/>
        <source>Do Not Spell Check</source>
        <translation>Nekontrolovat pravopis</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="451"/>
        <source>Check Style:</source>
        <translation>Ověřit styl:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Check</source>
        <translation>Kontrolovat</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Cross</source>
        <translation>Kříž</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Circle</source>
        <translation>Kruh</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Star</source>
        <translation>Hvězda</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="454"/>
        <source>Square</source>
        <translation>Čtverec</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="468"/>
        <source>Default is Checked</source>
        <translation>Standardně je označené</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="481"/>
        <source>Editable</source>
        <translation>Upravitelné</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="488"/>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1955"/>
        <source>Go To</source>
        <translation>Jít na</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1956"/>
        <source>Submit Form</source>
        <translation>Potvrdit formulář</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1956"/>
        <source>Reset Form</source>
        <translation>Vynulovat formulář</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1956"/>
        <source>Import Data</source>
        <translation>Importovat data</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="529"/>
        <source>Event:</source>
        <translation>Událost:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1935"/>
        <source>Mouse Up</source>
        <translation>Uvolnění myši</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1935"/>
        <source>Mouse Down</source>
        <translation>Stisk tlačítka myši</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1935"/>
        <source>Mouse Enter</source>
        <translation>Najetí myši</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1936"/>
        <source>Mouse Exit</source>
        <translation>Opuštění myší</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1936"/>
        <source>On Focus</source>
        <translation>Po přepnutí na</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1936"/>
        <source>On Blur</source>
        <translation>Při rozmazání</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="544"/>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1007"/>
        <source>Edit...</source>
        <translation>Upravit...</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="567"/>
        <source>Submit to URL:</source>
        <translation>Odeslat na URL:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="575"/>
        <source>Submit Data as HTML</source>
        <translation>Odeslat údaje jako HTML</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="586"/>
        <source>Import Data from:</source>
        <translation>Importovat data z:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="597"/>
        <source>Destination</source>
        <translation>Cíl</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="606"/>
        <source>To File:</source>
        <translation>Do souboru:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="613"/>
        <source>Change...</source>
        <translation>Změnit...</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="616"/>
        <source>Page:</source>
        <translation>Strana:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="630"/>
        <source>X-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="642"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="638"/>
        <source>Y-Pos:</source>
        <translation>Y-Poz:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="646"/>
        <source>Action</source>
        <translation>Akce</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="667"/>
        <source>Field is formatted as:</source>
        <translation>Pole je naformátované jako:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Plain</source>
        <translation>Obyčejný</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Number</source>
        <translation>Číslo</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Percentage</source>
        <translation>Procento</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Time</source>
        <translation>Čas</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="670"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="688"/>
        <source>Number Format</source>
        <translation>Formát čísla</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="753"/>
        <source>Decimals:</source>
        <translation>Desetinné:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="708"/>
        <source>Use Currency Symbol</source>
        <translation>Použít symbol měny</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="717"/>
        <source>Prepend Currency Symbol</source>
        <translation>Symbol měny vpředu</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="764"/>
        <source>Formatting</source>
        <translation>Formátování</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="745"/>
        <source>Percent Format</source>
        <translation>Formát procent</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="788"/>
        <source>Date Format</source>
        <translation>Formát data</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="813"/>
        <source>Time Format</source>
        <translation>Formát času</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="836"/>
        <source>Custom Scripts</source>
        <translation>Vlastní skripty</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="845"/>
        <source>Format:</source>
        <translation>Formát:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="866"/>
        <source>Keystroke:</source>
        <translation>Stlačení klávesy:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="889"/>
        <source>Format</source>
        <translation>Formát</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="904"/>
        <source>Value is not validated</source>
        <translation>Hodnota není potvrzena</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="911"/>
        <source>Value must be greater than or equal to:</source>
        <translation>Hodnota musí být větší nebo rovná:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="915"/>
        <source>and less or equal to:</source>
        <translation>a menší nebo rovná:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="928"/>
        <source>Custom validate script:</source>
        <translation>Vlastní ověření skriptu:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="946"/>
        <source>Validate</source>
        <translation>Vyhodnotit</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="961"/>
        <source>Value is not calculated</source>
        <translation>Hodnota není vypočítaná</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="968"/>
        <source>Value is the</source>
        <translation>Hodnota je</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="973"/>
        <source>sum</source>
        <translation>součet</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="973"/>
        <source>product</source>
        <translation>součin</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="973"/>
        <source>average</source>
        <translation>průměr</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="973"/>
        <source>minimum</source>
        <translation>minimum</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="974"/>
        <source>maximum</source>
        <translation>maximum</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="981"/>
        <source>of the following fields:</source>
        <translation>následujících polí:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="989"/>
        <source>Pick...</source>
        <translation>Vybrat...</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="994"/>
        <source>Custom calculation script:</source>
        <translation>Vlastní počítací skript:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1011"/>
        <source>Calculate</source>
        <translation>Vypočítat</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1030"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1034"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1087"/>
        <source>Enter a comma separated list of fields here</source>
        <translation>Vložte sem čárkami oddělený seznam polí</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1088"/>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Potřebujete minimálně ikonu pro Normal, abyste mohl(a) použít ikony pro tlačítka</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="2146"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1363"/>
        <source>Example:</source>
        <translation>Příklad:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1984"/>
        <source>Selection Change</source>
        <translation>Změna výběru</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="126"/>
        <source>Font for use with PDF 1.3:</source>
        <translation>Písmo užité v PDF 1.3:</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1086"/>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Indikátor je v PDF 1.3 ignorován</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="2146"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF soubory (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1993"/>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1253"/>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Obrázky (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="389"/>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Žádná</translation>
    </message>
    <message>
        <location filename="../annot.cpp" line="1955"/>
        <source>None</source>
        <comment>action</comment>
        <translation>Žádná</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <location filename="../annota.cpp" line="34"/>
        <source>Annotation Properties</source>
        <translation>Vlastnosti anotace</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="65"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="65"/>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="65"/>
        <source>External Link</source>
        <translation>Odkaz ven</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="66"/>
        <source>External Web-Link</source>
        <translation>Odkaz ven na web</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="83"/>
        <source>Destination</source>
        <translation>Cíl</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="136"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="343"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="343"/>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF dokumenty (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="71"/>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="97"/>
        <source>C&amp;hange...</source>
        <translation>Z&amp;měnit...</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="109"/>
        <source>&amp;Page:</source>
        <translation>Str&amp;ana:</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="131"/>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Poz</translation>
    </message>
    <message>
        <location filename="../annota.cpp" line="138"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <location filename="../applytemplatedialog.cpp" line="154"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="255"/>
        <source>Apply Master Page</source>
        <translation>Použít vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="256"/>
        <source>&amp;Master Page:</source>
        <translation>&amp;Vzorová strana:</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="257"/>
        <source>Apply To</source>
        <translation>Použít na</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="258"/>
        <source>Current &amp;page</source>
        <translation>&amp;Aktuální strana</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="259"/>
        <source>Alt+P</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="260"/>
        <source>&amp;Even pages</source>
        <translation>&amp;Sudé strany</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="261"/>
        <source>Alt+E</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="262"/>
        <source>O&amp;dd pages</source>
        <translation>&amp;Liché strany</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="263"/>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="264"/>
        <source>&amp;All pages</source>
        <translation>Všechny str&amp;any</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="265"/>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="266"/>
        <source>&amp;Within range</source>
        <translation>&amp;Interval stran</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="267"/>
        <source>Alt+W</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="269"/>
        <source>to</source>
        <translation>po</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="271"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="273"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <location filename="../applytemplatedialog.cpp" line="268"/>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Použít vybranou vzorovou stranu na sudé, liché, nebo všechny stránky z tohoto rozmezí</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <location filename="../arrowchooser.cpp" line="30"/>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <location filename="../plugins/barcodegenerator/barcode.cpp" line="22"/>
        <source>&amp;Barcode Generator...</source>
        <translation>Čá&amp;rové kódy...</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcode.cpp" line="37"/>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Rozhraní &quot;Pure Postscript Barcode Writer&quot;</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="291"/>
        <source>Error opening file: %1</source>
        <translation>Chyba při otevírání souboru: %1</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="46"/>
        <source>12 or 13 digits</source>
        <translation>12 nebo 13 číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="48"/>
        <source>8 digits</source>
        <translation>8 číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="50"/>
        <source>11 or 12 digits</source>
        <translation>11 nebo 12 číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="52"/>
        <source>7 or 8 digits</source>
        <translation>7 nebo 8 číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="54"/>
        <source>5 digits</source>
        <translation>5 číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="56"/>
        <source>2 digits</source>
        <translation>2 číslice</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="63"/>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Volitelný počet znaků, číslic a symbolů: -. *$/+%.</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="72"/>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="75"/>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Volitelný počet číslic a symbolů: -$:/.+ABCD.</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="83"/>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Volitelný počet číslic. ITF-14 obsahuje 14 číslic a nemá číslici kontrolní</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="98"/>
        <source>Variable number of digits</source>
        <translation>Volitelný počet číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="94"/>
        <source>Variable number of digits and capital letters</source>
        <translation>Volitelný počet číslic a velkých písmen</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="103"/>
        <source>Variable number of hexadecimal characters</source>
        <translation>Volitelný počet hexadecimálních číslic</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="322"/>
        <source>Barcode incomplete</source>
        <translation>Čárový kód není kompletní</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegenerator.cpp" line="59"/>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation>12 nebo 13 číslic s mezerami. Formát ISBN-10 vyžaduje 9 nebo 10 číslic s mezerami, ale tento standard se nedoporučuje používat po 1. lednu 2007. (Poznámka: K převodu starého ISBN-10 na nový ISBN-13 stačí uvést před prvními devíti číslicemi předponu 978-, např.  1-56592-479-7 -&gt; 978-1-56592-479. Konečné číslo se spočítá automaticky.)</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="18"/>
        <source>Barcode Creator</source>
        <translation>Generátor čárových kódů</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="48"/>
        <source>Barcode</source>
        <translation>Čárový kód</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="83"/>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="115"/>
        <source>Select one of the available barcode type here</source>
        <translation>Vyberte jeden z dostupných typů čárových kódů</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="137"/>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Číselná reprezentace vlastního kódu. Přečtěte si níže uvedenou nápovědu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="162"/>
        <source>Reset the barcode samples</source>
        <translation>Znovu nastavit ukázkovou hodnotu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="176"/>
        <source>&amp;Include text in barcode</source>
        <translation>Vložit &amp;text do výsledného kódu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="179"/>
        <source>Alt+I</source>
        <translation>Alt+TAlt+I</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="185"/>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Jestliže je zaškrtnuto, bude ve výsledném kódu také jeho textová reprezentace</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="193"/>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Hlídat okolní místo</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="196"/>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="199"/>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Kreslit pomocné šipky, aby bylo kolem kódu dostatek místa</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="239"/>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="266"/>
        <source>&amp;Background</source>
        <translation>&amp;Pozadí</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="269"/>
        <source>Alt+B</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="272"/>
        <source>Background color - under the code lines</source>
        <translation>Barva pozadí - pod čárami kódu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="280"/>
        <source>&amp;Lines</source>
        <translation>Čá&amp;ry</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="283"/>
        <source>Alt+L</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="286"/>
        <source>Color of the lines in barcode</source>
        <translation>Barva čar kódu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="294"/>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="297"/>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="300"/>
        <source>Color of the text and numbers</source>
        <translation>Barva textu a čísel</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="388"/>
        <source>Hints and help is shown here</source>
        <translation>Tipy a nápověda</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="402"/>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Náhled výsledného kódu. 72dpi.</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="94"/>
        <source>Co&amp;de:</source>
        <translation>&amp;Kód:</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="207"/>
        <source>I&amp;nclude checksum</source>
        <translation>&amp;Vložit kontrolní součet</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="210"/>
        <source>Alt+N</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="213"/>
        <source>Generate and include a checksum in barcode</source>
        <translation>Vytvořit a vložit kontrolní součet do kódu</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="221"/>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Vložit &amp;kontrolní číslici</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="224"/>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcodegeneratorbase.ui" line="227"/>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Vložit kontrolní číslici do textu kódu</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <location filename="../scrap.cpp" line="680"/>
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="532"/>
        <source>Delete</source>
        <translation>Smazat</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="638"/>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="639"/>
        <source>New Entry</source>
        <translation>Nová položka</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="531"/>
        <source>Rename</source>
        <translation>Přejmenovat</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="684"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="685"/>
        <source>&amp;Load...</source>
        <translation>&amp;Načíst...</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="686"/>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="687"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="681"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="682"/>
        <source>&amp;Preview</source>
        <translation>&amp;Náhled</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="639"/>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="578"/>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Jméno %1 není unikátní.
Prosím, zvolte jiné.</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="419"/>
        <source>Choose a Scrapbook Directory</source>
        <translation>Zvolte adresář zápisníku</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="475"/>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="453"/>
        <source>Scrapbook (*.scs)</source>
        <translation>Zápisník (*.scs)</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="456"/>
        <source>Choose a scrapbook file to import</source>
        <translation>Vyberte zápisník, který importuji</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="688"/>
        <source>&amp;Import Scrapbook File...</source>
        <translation>&amp;Import souborů zápisníku...</translation>
    </message>
    <message>
        <location filename="../scrap.cpp" line="305"/>
        <source>Main</source>
        <translation>Hlavní</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <location filename="../bookmwin.cpp" line="550"/>
        <source>Bookmarks</source>
        <translation>Záložky</translation>
    </message>
    <message>
        <location filename="../bookmwin.cpp" line="215"/>
        <source>Move Bookmark</source>
        <translation>Přesunout záložku</translation>
    </message>
    <message>
        <location filename="../bookmwin.cpp" line="216"/>
        <source>Insert Bookmark</source>
        <translation>Vložit záložku</translation>
    </message>
    <message>
        <location filename="../bookmwin.cpp" line="217"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <location filename="../bookpalette.cpp" line="67"/>
        <source>Bookmarks</source>
        <translation>Záložky</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <location filename="../buttonicon.cpp" line="20"/>
        <source>Icon Placement</source>
        <translation>Umístění ikon</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="25"/>
        <source>Layout:</source>
        <translation>Návrh úpravy sazby:</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="28"/>
        <source>Caption only</source>
        <translation>Jen popisky</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="28"/>
        <source>Icon only</source>
        <translation>Jen ikony </translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="28"/>
        <source>Caption below Icon</source>
        <translation>Popisky pod ikonami</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="28"/>
        <source>Caption above Icon</source>
        <translation>Popisky nad ikonami</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="29"/>
        <source>Caption right to Icon</source>
        <translation>Popisky vpravo od ikon</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="29"/>
        <source>Caption left to Icon</source>
        <translation>Popisky vlevo od ikon</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="29"/>
        <source>Caption overlays Icon</source>
        <translation>Popisky překrývají ikony</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="37"/>
        <source>Scale:</source>
        <translation>Měřítko:</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="42"/>
        <source>Always</source>
        <translation>Vždy</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="42"/>
        <source>When Icon is too small</source>
        <translation>Když jsou ikony příliš malé</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="43"/>
        <source>When Icon is too big</source>
        <translation>Když jsou ikony příliš velké</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="43"/>
        <source>Never</source>
        <translation>Nikdy</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="50"/>
        <source>Scale How:</source>
        <translation>Jak změnit:</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="53"/>
        <source>Proportional</source>
        <translation>Proporcionálně</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="54"/>
        <source>Non Proportional</source>
        <translation>Neproporcionálně</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="73"/>
        <source>Icon</source>
        <translation>Ikona</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="97"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="101"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../buttonicon.cpp" line="104"/>
        <source>Reset</source>
        <translation>Vynulovat</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <location filename="../cmsprefs.cpp" line="25"/>
        <source>System Profiles</source>
        <translation>Systémové profily</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="104"/>
        <source>Rendering Intents</source>
        <translation>Účel reprodukce (rendering)</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="116"/>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="116"/>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="117"/>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="117"/>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="164"/>
        <source>Default color profile for solid colors on the page</source>
        <translation>Implicitní barevný profil pro plné barvy na straně</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="165"/>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Barevný profil, který máte vygenerován nebo dodán od výrobce zařízení.
Tento profil by měl být nastavený na váše prostředí - ne obecný (např. sRGB).</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="166"/>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Barevný profil vaší tiskárny, který máte od výrobce.
Tento profil by měl být nastavený na váše prostředí - ne obecný (např. sRGB).</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="167"/>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Implicitní účel reprodukce monitoru. Jestliže víte, proč jej změnit, zvolte
relativní kolorimetrickou transformaci nebo perceptuální (fotografickou) transformaci.</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="168"/>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Implicitní účel reprodukce tiskárny. Jestliže víte, proč jej změnit, zvolte
relativní kolorimetrickou transformaci nebo perceptuální (fotografickou) transformaci.</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="169"/>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Povolit &quot;soft proofing&quot; (nátisk) založený na vybraném profilu tiskárny.</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="170"/>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Výsledek tisku barev bývá rozdílný od toho, jak jsou zobrazeny barvy na monitoru.
Proto jsou důležité kvalitní profily.</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="171"/>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Mapování černé barvy ze dvou profilů (blackpoint compensation) je způsob,
jakým lze zlepšit kontrast fotografií. Doporučeno, jestliže je máte v dokumentu.</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="21"/>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktivovat správu barev</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="73"/>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="86"/>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="99"/>
        <source>P&amp;rinter:</source>
        <translation>&amp;Tiskárna:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="125"/>
        <source>M&amp;onitor:</source>
        <translation>Mo&amp;nitor:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="135"/>
        <source>Pr&amp;inter:</source>
        <translation>Tis&amp;kárna:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="141"/>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulace tisku na obrazovce</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="145"/>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Označ netisknutelné barvy (&amp;gamut)</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="149"/>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Použí&amp;t mapování černé ze dvou profilů</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="47"/>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB obrázky:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="60"/>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK obrázky:</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="162"/>
        <source>Default color profile for imported CMYK images</source>
        <translation>Implicitní barevný profil importovaných CMYK onrázků</translation>
    </message>
    <message>
        <location filename="../cmsprefs.cpp" line="163"/>
        <source>Default color profile for imported RGB images</source>
        <translation>Implicitní barevný profil importovaných RGB onrázků</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <location filename="../cmykfw.cpp" line="68"/>
        <source>Edit Color</source>
        <translation>Upravit barvu</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="715"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="398"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="799"/>
        <source>Web Safe RGB</source>
        <translation>RGB pro web</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="117"/>
        <source>New</source>
        <translation>Nový</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="134"/>
        <source>Old</source>
        <translation>Starý</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="740"/>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="741"/>
        <source>M:</source>
        <translation>M:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="742"/>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="330"/>
        <source>K:</source>
        <translation>K:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="407"/>
        <source>Dynamic Color Bars</source>
        <translation>Mřížky s dynamickými barvami</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="408"/>
        <source>Static Color Bars</source>
        <translation>Mřížky se statickými barvami</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="784"/>
        <source>R:</source>
        <translation>R:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="785"/>
        <source>G:</source>
        <translation>G:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="786"/>
        <source>B:</source>
        <translation>B:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="739"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="172"/>
        <source>HSV-Colormap</source>
        <translation>HSV-barevná mapa</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="77"/>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="87"/>
        <source>Color &amp;Model</source>
        <translation>&amp;Barevný model</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="101"/>
        <source>Is Spot Color</source>
        <translation type="unfinished">Je &quot;spot color&quot;</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="106"/>
        <source>Is Registration Color</source>
        <translation type="unfinished">Je &quot;registration color&quot;</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="991"/>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Nelze vytvořit barvu jménem &quot;%1&quot;.
Je rezervováno pro průhlednou barvu</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="1000"/>
        <source>Name of the color is not unique</source>
        <translation>Jméno barvy není jedinečné</translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="374"/>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../cmykfw.cpp" line="375"/>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <location filename="../charselect.cpp" line="80"/>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Náhled uvidíte, pokud stisknete a uvolníte pravé tlačítko myši. Klávesa Insert vloží do výběru znak a Delete smaže naposledy vložený</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <location filename="../charselect.cpp" line="719"/>
        <source>Select Character:</source>
        <translation>Vybrat znak:</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="282"/>
        <source>Font:</source>
        <translation>Písmo:</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="291"/>
        <source>Character Class:</source>
        <translation>Třída znaků:</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="346"/>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="348"/>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="350"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="355"/>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Vložit znak na pozici kurzoru v textu</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="356"/>
        <source>Delete the current selection(s).</source>
        <translation>Smazat aktuální výběr(y).</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="508"/>
        <source>Full Character Set</source>
        <translation>Plná znaková sada</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="513"/>
        <source>Basic Latin</source>
        <translation>Základní latinka</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="519"/>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 doplňky</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="525"/>
        <source>Latin Extended-A</source>
        <translation>Rozšířená latinka A</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="531"/>
        <source>Latin Extended-B</source>
        <translation>Rozšířená latinka B</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="537"/>
        <source>General Punctuation</source>
        <translation>Obecná interpunkce</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="543"/>
        <source>Super- and Subscripts</source>
        <translation>Horní a dolní indexy</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="549"/>
        <source>Currency Symbols</source>
        <translation>Měny</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="555"/>
        <source>Letterlike Symbols</source>
        <translation>Symboly dopisu</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="561"/>
        <source>Number Forms</source>
        <translation>Číselné formy</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="567"/>
        <source>Arrows</source>
        <translation>Šipky</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="573"/>
        <source>Mathematical Operators</source>
        <translation>Matematické operátory</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="579"/>
        <source>Box Drawing</source>
        <translation>Kreslení boxů</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="585"/>
        <source>Block Elements</source>
        <translation>Blokové elementy</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="591"/>
        <source>Geometric Shapes</source>
        <translation>Geometrické útvary</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="597"/>
        <source>Miscellaneous Symbols</source>
        <translation>Různé symboly</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="603"/>
        <source>Dingbats</source>
        <translation>Blbinky</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="609"/>
        <source>Small Form Variants</source>
        <translation>Varianty minuskulí</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="615"/>
        <source>Ligatures</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="621"/>
        <source>Specials</source>
        <translation>Speciální znaky</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="627"/>
        <source>Greek</source>
        <translation>Řečtina</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="633"/>
        <source>Greek Extended</source>
        <translation>Rozšířená řečtina</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="639"/>
        <source>Cyrillic</source>
        <translation>Azbuka</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="645"/>
        <source>Cyrillic Supplement</source>
        <translation>Azbuka - doplňky</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="651"/>
        <source>Arabic</source>
        <translation>Arabské</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="657"/>
        <source>Arabic Extended A</source>
        <translation>Arabské rozšířené A</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="663"/>
        <source>Arabic Extended B</source>
        <translation>Arabské rozšířené B</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="669"/>
        <source>Hebrew</source>
        <translation>Hebrejské</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="325"/>
        <source>&amp;Insert Code:</source>
        <translation>&amp;Vložit kód:</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="357"/>
        <source>Close this dialog and return to text editing</source>
        <translation>Zavřít tento dialog a vrátit se k úpravě textu</translation>
    </message>
    <message>
        <location filename="../charselect.cpp" line="358"/>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Zadejte přímo čtyřčíselnou hodnotu Unicode</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <location filename="../checkDocument.cpp" line="242"/>
        <source>Glyphs missing</source>
        <translation>Chybí znak</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="243"/>
        <source>Text overflow</source>
        <translation>Text přetéká</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="244"/>
        <source>Object is not on a Page</source>
        <translation>Objekt není na straně</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="245"/>
        <source>Missing Image</source>
        <translation>Chybí obrázek</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="246"/>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Obrázek má nižší DPI než %1 DPI</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="247"/>
        <source>Object has transparency</source>
        <translation>Objek onsahuje průhlednost</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="248"/>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt je PDF anotace nebo formulář</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="249"/>
        <source>Object is a placed PDF</source>
        <translation>Objekt je umístěné PDF</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="253"/>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="257"/>
        <source>No Problems found</source>
        <translation>Nenalezeny žádné problémy</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="547"/>
        <source>Page </source>
        <translation>Strana</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="687"/>
        <source>Free Objects</source>
        <translation>Volné objekty</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="693"/>
        <source>Problems found</source>
        <translation>Nalezeny problémy</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="706"/>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="707"/>
        <source>Items</source>
        <translation>Onjekty</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="708"/>
        <source>Problems</source>
        <translation>Problémy</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="710"/>
        <source>Current Profile:</source>
        <translation>Aktuální profil:</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="711"/>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorovat chyby</translation>
    </message>
    <message>
        <location filename="../checkDocument.cpp" line="250"/>
        <source>Annotation uses a non TrueType font</source>
        <translation>Anotace používá ne-TrueType písmo</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <location filename="../editformats.cpp" line="89"/>
        <source>Choose Styles</source>
        <translation>Vybrat styly</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="94"/>
        <source>Available Styles</source>
        <translation>Dostupné styly</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <location filename="../collect4output.cpp" line="52"/>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <location filename="../collect4output.cpp" line="68"/>
        <source>Collecting...</source>
        <translation>Provádím export...</translation>
    </message>
    <message>
        <location filename="../collect4output.cpp" line="72"/>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <location filename="../collect4output.cpp" line="72"/>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Nelze exportovat všechny soubory:
%1</translation>
    </message>
    <message>
        <location filename="../collect4output.cpp" line="80"/>
        <source>Cannot collect the file: 
%1</source>
        <translation> Nelze exportovat soubor:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <location filename="../colorm.cpp" line="246"/>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="266"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="268"/>
        <source>&amp;New</source>
        <translation>&amp;Nová</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="270"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="274"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="277"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="282"/>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Odstranit nepoužité</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="289"/>
        <source>Color Sets</source>
        <translation>Množiny barev</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="296"/>
        <source>Current Color Set:</source>
        <translation>Aktuálí množina barev:</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="336"/>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Uložit množinu barev</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="356"/>
        <source>Choose a color set to load</source>
        <translation>Nahrát množinu barev</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="357"/>
        <source>Save the current color set</source>
        <translation>Uložit aktuální množinu barev</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="362"/>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Odstranit nepoužité barvy z aktuální množiny barev</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="364"/>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importovat do současné množiny barvy z jiného dokumentu</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="365"/>
        <source>Create a new color within the current set</source>
        <translation>Vytvořit novou barvu v aktuální množině</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="366"/>
        <source>Edit the currently selected color</source>
        <translation>Upravit zvolenou barvu</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="367"/>
        <source>Make a copy of the currently selected color</source>
        <translation>Vytvořit kopii vybrané barvy</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="368"/>
        <source>Delete the currently selected color</source>
        <translation>Smazat vybranou barvu</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="369"/>
        <source>Make the current colorset the default color set</source>
        <translation>Nastavit aktuální množinu barev jako implicitní</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="386"/>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="386"/>
        <source>Choose a Name</source>
        <translation>Vybrat jméno</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="564"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="562"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="564"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="599"/>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="609"/>
        <source>New Color</source>
        <translation>Nová barva</translation>
    </message>
    <message>
        <location filename="../colorm.cpp" line="370"/>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="116"/>
        <source>Monochromatic</source>
        <translation>Monochromatické</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="117"/>
        <source>Analogous</source>
        <translation>Analogické</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="183"/>
        <source>Complementary</source>
        <translation>Komplementární</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="119"/>
        <source>Split Complementary</source>
        <translation>Dělené komplementární</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="120"/>
        <source>Triadic</source>
        <translation>Triadické</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="121"/>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetradické (zdvojené komplementární)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="157"/>
        <source>Base Color</source>
        <translation>Základní barva</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="167"/>
        <source>Monochromatic Light</source>
        <translation>Monochromatické světlé</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="168"/>
        <source>Monochromatic Dark</source>
        <translation>Monochromatické tmavé</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="175"/>
        <source>1st. Analogous</source>
        <translation>První analogická</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="176"/>
        <source>2nd. Analogous</source>
        <translation>Druhá analogická</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="190"/>
        <source>1st. Split</source>
        <translation>První dělená</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="191"/>
        <source>2nd. Split</source>
        <translation>Druhá dělená</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="192"/>
        <source>3rd. Split</source>
        <translation>Třetí dělená</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="193"/>
        <source>4th. Split</source>
        <translation>Čtvrtá dělená</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="200"/>
        <source>1st. Triadic</source>
        <translation>První triadická</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="201"/>
        <source>2nd. Triadic</source>
        <translation>Druhá triadická</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="208"/>
        <source>1st. Tetradic (base opposite)</source>
        <translation>První tetradická (opak základní)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="209"/>
        <source>2nd. Tetradic (angle)</source>
        <translation>Druhá tetradická (úhel)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheelwidget.cpp" line="210"/>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Třetí tetradická (opačný úhel)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="179"/>
        <source>Normal Vision</source>
        <translation>Normální vidění</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="183"/>
        <source>Full Color Blindness</source>
        <translation>Barvoslepost</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="228"/>
        <source>Vision Defect:</source>
        <translation>Porucha vidění:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="229"/>
        <source>Color Wheel</source>
        <translation type="unfinished">Kruhový výběr barev</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="230"/>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="231"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="232"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="233"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="234"/>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="235"/>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="237"/>
        <source>Select Method:</source>
        <translation>Vyberte metodu:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="238"/>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Úhel (0-90 stupňů):</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="239"/>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Sloučit barvy</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="240"/>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Zaměnit barvy</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="243"/>
        <source>Merge created colors into the document colors</source>
        <translation>Sloučit vytvořené barvy s barvami dokumentu</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="244"/>
        <source>Replace created colors in the document colors</source>
        <translation>Zaměnit vytvořené barvy v barvách dokumentu</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="245"/>
        <source>Leave colors untouched</source>
        <translation>Ponechat barvy nedotknuté</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="328"/>
        <source>Merging colors</source>
        <translation>Slučování barev</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="334"/>
        <source>Error: </source>
        <translation>Chyba:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="343"/>
        <source>Now opening the color manager.</source>
        <translation>Spouštění správce barev.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="346"/>
        <source>Color Merging</source>
        <translation>Slučování barev</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="96"/>
        <source>Cr&amp;eate color...</source>
        <translation>Vy&amp;tvořit barvu...</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="98"/>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Importovat existující barvu...</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="100"/>
        <source>&amp;Merge colors</source>
        <translation>&amp;Sloučit barvy</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="101"/>
        <source>&amp;Replace colors</source>
        <translation>Z&amp;aměnit barvy</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="102"/>
        <source>E&amp;xit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="103"/>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="246"/>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Rozdíl mezi vybranou a počítanou hodnotou. Více informací naleznete v dokumentaci.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="247"/>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Klikněte kolečkem pro získání základní barvy. Jedná se o odstín v režimu HSV.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="248"/>
        <source>Sample color scheme</source>
        <translation>Ukázkové barevné schéma</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="249"/>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Vyberte jednu z metod pro vytvoření barevného schématu. Více informací naleznete v dokumentaci.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="250"/>
        <source>Colors of your chosen color scheme</source>
        <translation>Barvy vybraného barevného schématu</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="251"/>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Simulace běžných zrakových vad. Vyberte typ vady.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="429"/>
        <source>New Color</source>
        <translation>Nová barva</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="459"/>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Není možné nalézt požadovanou barvu. Pravděpodobně jste vybrali černou, šedou nebo bílou. Tuto barvu není možné zpracovat.</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="97"/>
        <source>C&amp;olor Components...</source>
        <translation>&amp;Barevné komponenty...</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="180"/>
        <source>Protanopia (Red)</source>
        <translation>Barvoslepost (červená)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="181"/>
        <source>Deuteranopia (Green)</source>
        <translation>Barvoslepost (zelená)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="182"/>
        <source>Tritanopia (Blue)</source>
        <translation>Barvoslepost (modrá)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="334"/>
        <source>Color %1 exists already!</source>
        <translation>Barva %1 už existuje!</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="339"/>
        <source>Color %1 appended.</source>
        <translation>Barva %1 přidána.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <location filename="../plugins/colorwheel/colorwheel.cpp" line="50"/>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Barevný kruh...</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheel.cpp" line="66"/>
        <source>Color setting helper</source>
        <translation>Pomocník pro nastavení barev</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheel.cpp" line="67"/>
        <source>Color selector with color theory included.</source>
        <translation>Výběr barev s naukou o barvách.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <location filename="../commonstrings.cpp" line="154"/>
        <source>&amp;Apply</source>
        <translation>&amp;Aplikovat</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="157"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="163"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="166"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="169"/>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="160"/>
        <source>None</source>
        <comment>color name</comment>
        <translation>Žádný</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="172"/>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="178"/>
        <source>Single Page</source>
        <translation>Jedna strana</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="179"/>
        <source>Double Sided</source>
        <translation>Dvojstrany</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="180"/>
        <source>3-Fold</source>
        <translation>3 složení</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="181"/>
        <source>4-Fold</source>
        <translation>4 složení</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="188"/>
        <source>Left Page</source>
        <translation>Levá strana</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="189"/>
        <source>Middle</source>
        <translation>Střední</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="190"/>
        <source>Middle Left</source>
        <translation>Střední levá</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="191"/>
        <source>Middle Right</source>
        <translation>Střední pravá</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="192"/>
        <source>Right Page</source>
        <translation>Pravá strana</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="195"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="197"/>
        <source>Normal Left</source>
        <translation>Normální vlevo</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="199"/>
        <source>Normal Middle</source>
        <translation>Normální na střed</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="201"/>
        <source>Normal Right</source>
        <translation>Normální vpravo</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="209"/>
        <source>Monday</source>
        <translation>Pondělí</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="210"/>
        <source>Tuesday</source>
        <translation>Úterý</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="211"/>
        <source>Wednesday</source>
        <translation>Středa</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="212"/>
        <source>Thursday</source>
        <translation>Čtvrtek</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="213"/>
        <source>Friday</source>
        <translation>Pátek</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="214"/>
        <source>Saturday</source>
        <translation>Sobota</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="215"/>
        <source>Sunday</source>
        <translation>Neděle</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="216"/>
        <source>January</source>
        <translation>Leden</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="217"/>
        <source>February</source>
        <translation>Únor</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="218"/>
        <source>March</source>
        <translation>Březen</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="219"/>
        <source>April</source>
        <translation>Duben</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="220"/>
        <source>May</source>
        <translation>Květen</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="221"/>
        <source>June</source>
        <translation>Červen</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="222"/>
        <source>July</source>
        <translation>Červenec</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="223"/>
        <source>August</source>
        <translation>Srpen</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="224"/>
        <source>September</source>
        <translation>Září</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="225"/>
        <source>October</source>
        <translation>Říjen</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="226"/>
        <source>November</source>
        <translation>Listopad</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="227"/>
        <source>December</source>
        <translation>Prosinec</translation>
    </message>
    <message>
        <location filename="../commonstrings.cpp" line="230"/>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <location filename="../cpalette.cpp" line="568"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="569"/>
        <source>Horizontal Gradient</source>
        <translation>Vodorovný přechod</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="570"/>
        <source>Vertical Gradient</source>
        <translation>Svislý přechod</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="571"/>
        <source>Diagonal Gradient</source>
        <translation>Úhlopříčný přechod</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="572"/>
        <source>Cross Diagonal Gradient</source>
        <translation>Obrácený úhlopříčný přechod</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="573"/>
        <source>Radial Gradient</source>
        <translation>Kruhový přechod</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="559"/>
        <source>Opacity:</source>
        <translation>Neprůsvitnost:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="554"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="558"/>
        <source>Shade:</source>
        <translation>Stín:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="578"/>
        <source>Edit Line Color Properties</source>
        <translation>Upravit barvu čáry</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="579"/>
        <source>Edit Fill Color Properties</source>
        <translation>Barva výplně</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="580"/>
        <source>Saturation of color</source>
        <translation>Sytost barvy</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="581"/>
        <source>Normal or gradient fill method</source>
        <translation>Normální výplň nebo přechody</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="582"/>
        <source>Set the transparency for the color selected</source>
        <translation>Nastavení průhlednosti vybrané barvy</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="574"/>
        <source>Free linear Gradient</source>
        <translation>Volný lineární gradient</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="575"/>
        <source>Free radial Gradient</source>
        <translation>Volný radiální gradient</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="560"/>
        <source>X1:</source>
        <translation>X1:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="561"/>
        <source>Y1:</source>
        <translation>Y1:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="549"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="562"/>
        <source>X2:</source>
        <translation>X2:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="563"/>
        <source>Y2:</source>
        <translation>Y2:</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="564"/>
        <source>Move Vector</source>
        <translation>Přesunout vektor</translation>
    </message>
    <message>
        <location filename="../cpalette.cpp" line="583"/>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Začátek vektoru přesuňte pomocí stisknutého levého tlačítka myši, konec vektoru pomocí pravého stisknutého tlačítka</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="16"/>
        <source>CSV Importer Options</source>
        <translation>Nastavení importu CSV</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="22"/>
        <source>Field delimiter:</source>
        <translation>Oddělovač položek:</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="66"/>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="35"/>
        <source>Value delimiter:</source>
        <translation>Oddělovač hodnot:</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="47"/>
        <source>First row is a header</source>
        <translation>První řádek je hlavička</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="53"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="56"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvdia.cpp" line="40"/>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <location filename="../cupsoptions.cpp" line="43"/>
        <source>Printer Options</source>
        <translation>Možnosti tiskárny</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="194"/>
        <source>Option</source>
        <translation>Volba</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="195"/>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="129"/>
        <source>Page Set</source>
        <translation>Nastavení strany</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="133"/>
        <source>All Pages</source>
        <translation>Všechny strany</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="127"/>
        <source>Even Pages only</source>
        <translation>Pouze sudé strany</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="128"/>
        <source>Odd Pages only</source>
        <translation>Pouze liché strany</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="146"/>
        <source>Mirror</source>
        <translation>Zrcadlo</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="150"/>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="144"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="163"/>
        <source>Orientation</source>
        <translation>Orientace</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="167"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="161"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="183"/>
        <source>N-Up Printing</source>
        <translation>N-Up tisk</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="187"/>
        <source>Page per Sheet</source>
        <translation>Strana na arch</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="182"/>
        <source>Pages per Sheet</source>
        <translation>Stran na arch</translation>
    </message>
    <message>
        <location filename="../cupsoptions.cpp" line="215"/>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Tento panel zobrazuje volby CUPS při tisku. Přesné dostupné parametry zavisí na ovladači vaší tiskárny. Podporu pro CUPS si můžete ověřit v nabídce Nápověda &gt; O Scribusu. Hledejte výpis C-C-T, což znamená C=CUPS, C=littlecms a T=TIFF. Chybějící knihovna je označena hvězdičkou *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <location filename="../customfdialog.cpp" line="301"/>
        <source>Encoding:</source>
        <translation>Kódování:</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="347"/>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Přesunutí do adresáře dokumentů.
Lze uvést v Nastavení.</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="281"/>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimovaný soubor</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="263"/>
        <source>&amp;Include Fonts</source>
        <translation>Včetně &amp;písem</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="185"/>
        <source>Set Color Components</source>
        <translation>Nastavit barevné složky</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="186"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="187"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="188"/>
        <source>HSV</source>
        <translation>HSV</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="189"/>
        <source>H:</source>
        <translation>H:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="190"/>
        <source>S:</source>
        <translation>S:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="191"/>
        <source>V:</source>
        <translation>V:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="192"/>
        <source>R:</source>
        <translation>R:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="193"/>
        <source>G:</source>
        <translation>G:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="194"/>
        <source>B:</source>
        <translation>B:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="195"/>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="196"/>
        <source>M:</source>
        <translation>M:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="197"/>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="198"/>
        <source>K:</source>
        <translation>K:</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="199"/>
        <source>Set &amp;RGB</source>
        <translation>Nastavit &amp;RGB</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="200"/>
        <source>Set C&amp;MYK</source>
        <translation>Nastavit &amp;CMYK</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/cwsetcolor.cpp" line="201"/>
        <source>Set &amp;HSV</source>
        <translation>Nastavit &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <location filename="../deferredtask.cpp" line="82"/>
        <source>Cancelled by user</source>
        <translation>Zrušeno uživatelem</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <location filename="../dcolor.cpp" line="31"/>
        <source>Delete Color</source>
        <translation>Smazat barvu</translation>
    </message>
    <message>
        <location filename="../dcolor.cpp" line="37"/>
        <source>Delete Color:</source>
        <translation>Smazat barvu:</translation>
    </message>
    <message>
        <location filename="../dcolor.cpp" line="46"/>
        <source>Replace With:</source>
        <translation>Nehradit čím:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <location filename="../delpages.cpp" line="30"/>
        <source>Delete Pages</source>
        <translation>Smazat strany</translation>
    </message>
    <message>
        <location filename="../delpages.cpp" line="42"/>
        <source>to:</source>
        <translation>po:</translation>
    </message>
    <message>
        <location filename="../delpages.cpp" line="36"/>
        <source>Delete From:</source>
        <translation>Smazat od:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <location filename="../editformats.cpp" line="30"/>
        <source>Delete Style</source>
        <translation>Smazat styl</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="36"/>
        <source>Delete Style:</source>
        <translation>Smazat styl:</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="40"/>
        <source>Replace With:</source>
        <translation>Nehradit čím:</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="43"/>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <location filename="../plugins/gettext/docim/docim.cpp" line="173"/>
        <source>Importing failed</source>
        <translation>Chyba importu</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/docim/docim.cpp" line="174"/>
        <source>Importing Word document failed 
%1</source>
        <translation>Chyba importu Word souboru
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <location filename="../docinfo.cpp" line="26"/>
        <source>Document Information</source>
        <translation>Informace o dokumentu</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="35"/>
        <source>&amp;Title:</source>
        <translation>&amp;Název:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="41"/>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="47"/>
        <source>&amp;Keywords:</source>
        <translation>&amp;Klíčová slova:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="54"/>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Popis:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="66"/>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Vydavatel:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="72"/>
        <source>&amp;Contributors:</source>
        <translation>Př&amp;ispěvatelé:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="79"/>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="85"/>
        <source>T&amp;ype:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="91"/>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formát:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="97"/>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;fikátor:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="103"/>
        <source>&amp;Source:</source>
        <translation>&amp;Zdroj:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="109"/>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="115"/>
        <source>&amp;Relation:</source>
        <translation>&amp;Odkaz:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="121"/>
        <source>Co&amp;verage:</source>
        <translation>O&amp;blast:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="127"/>
        <source>Ri&amp;ghts:</source>
        <translation>P&amp;ráva:</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="134"/>
        <source>Further &amp;Information</source>
        <translation>Da&amp;lší informace</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="143"/>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Osoba nebo organizace, která dokument zveřejňuje, publikuje nebo vydává</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="144"/>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Osoby nebo organizace, které přispěly k obsahu dokumentu</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="145"/>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datum přiřazené k události v životním cyklu dokumentu. Použijte RRRR-MM-DD formát podle ISO 8601</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="146"/>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Povaha dokumentu. Např. kategorie, finkce atd</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="148"/>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Jednoznačný odkaz na dokument. Např. ISBN nebo URI</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="149"/>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Odkaz na dokument, ze kterého tento vychází. Např. ISBN nebo URI</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="151"/>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Odkaz na související dokument např. ISBN nebo URI</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="152"/>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Rozšířený rámec obsahu dokumentu - oblast působnosti, datum anebo soudní pravomoc atd</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="153"/>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informace o autorských právech, licencích apod. Např. copyright, patenty nebo obchodní značka</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="133"/>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="139"/>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Osoba nebo organizace primárně odpovědná za obsah dokumentu. Toto pole může být vloženo do dokumentu Scribus jako odkaz, stejně tak do PDF jako metadata</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="140"/>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Název dokumentu. Toto pole může být vloženo do dokumentu Scribus jako odkaz, stejně tak do PDF jako metadata</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="141"/>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Popis obsahu dokumentu. Jedná se stručné shrnutí nebo abstrakt dokumentu. Je vloženo do PDF při exportu</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="142"/>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Námět obsahu dokumentu. Toto pole obsahuje klíčová slova, která si přejete vložit do PDF kvůli vyhledávání a indexování PDF souborů</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="147"/>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fyzická nebo digitální prezentace dokumentu. Neuškodí typ média nebo rozměry. Praktické jsou zde také  RFC2045, RFC2046 jako MIME typy</translation>
    </message>
    <message>
        <location filename="../docinfo.cpp" line="150"/>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Jazyk obsahu dokumentu, obvykle kód jazyka podle ISO-639 doplněný pomlčkou a kódem země podle ISO-3166, např. en-GB, fr-CH, cs-CZ</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <location filename="../docsections.cpp" line="58"/>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Přidat do dokumentu sekci s číslováním stran. Nová sekce bude přidána za aktuálně vybranou sekci.</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="59"/>
        <source>Delete the currently selected section.</source>
        <translation>Odstranit aktuálně vybranou sekci.</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="73"/>
        <source>1, 2, 3, ...</source>
        <translation>1, 2, 3, ...</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="73"/>
        <source>i, ii, iii, ...</source>
        <translation>i, ii, iii, ...</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="73"/>
        <source>I, II, III, ...</source>
        <translation>I, II, III, ...</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="73"/>
        <source>a, b, c, ...</source>
        <translation>a, b, c, ...</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="73"/>
        <source>A, B, C, ...</source>
        <translation>A, B, C, ...</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="65"/>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Název:&lt;/b&gt; Volitelný název pro sekci, např. Index&lt;br/&gt;&lt;b&gt;Zobrazeno:&lt;/b&gt;Vyberte, pokud chcete v této sekci zobrazit čísla stran a pokud je v sekci alespoň jeden textový rámec.&lt;br/&gt;&lt;b&gt;Od:&lt;/b&gt; Index strany, od které se má začít.&lt;br/&gt;&lt;b&gt;Do:&lt;/b&gt; Index strany, u které se má skončit.&lt;br/&gt;&lt;b&gt;Styl:&lt;/b&gt; Vyberte styl pro číslování stran.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; Index v rámci rozmezí stylu, kde se má začít, např. pokud je Start=2 a Style=a,b,c ...,potom budou čísla začínat u &quot;b&quot;. U první sekce v dokumentu to nahradí původní první číslo strany v novém okně souboru. (fixme).</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="183"/>
        <source>Page Number Out Of Bounds</source>
        <translation>Číslo strany je mimo použitelnou oblast</translation>
    </message>
    <message>
        <location filename="../docsections.cpp" line="183"/>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Hodnota, kterou jste zadali, je mimo rozsah stran v současném dokumentu (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <location filename="../docsectionsbase.ui" line="16"/>
        <source>Document Sections</source>
        <translation>Sekce dokumentu</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="39"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="55"/>
        <source>From</source>
        <translation>Od</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="63"/>
        <source>To</source>
        <translation>po</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="71"/>
        <source>Style</source>
        <translation>Styl</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="79"/>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="125"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="128"/>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="136"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="139"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../docsectionsbase.ui" line="47"/>
        <source>Shown</source>
        <translation>Zobrazen</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <location filename="../docitemattrprefs.cpp" line="16"/>
        <source>Relates To</source>
        <translation>Vztahuje se k</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="16"/>
        <source>Is Parent Of</source>
        <translation>Je rodičem</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="16"/>
        <source>Is Child Of</source>
        <translation>Je potomkem</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="18"/>
        <source>Text Frames</source>
        <translation>Textové rámce</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="18"/>
        <source>Image Frames</source>
        <translation>Obrázkové rámce</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="20"/>
        <source>Boolean</source>
        <translation>Ano-ne</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="20"/>
        <source>Integer</source>
        <translation>Číslo</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="20"/>
        <source>String</source>
        <translation>Řetězec</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="16"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Žádný</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="18"/>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Nic</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="20"/>
        <source>None</source>
        <comment>types</comment>
        <translation>Žádné</translation>
    </message>
    <message>
        <location filename="../docitemattrprefs.cpp" line="20"/>
        <source>Real Number</source>
        <translation>Reálné číslo</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="30"/>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položek dokumentu</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="47"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="52"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="57"/>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="62"/>
        <source>Parameter</source>
        <translation>Parametr</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="67"/>
        <source>Relationship</source>
        <translation>Vztah</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="72"/>
        <source>Relationship To</source>
        <translation>Vztah k</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="77"/>
        <source>Auto Add To</source>
        <translation>Automaticky přidat k</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="120"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="123"/>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="131"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="134"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="142"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="145"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="153"/>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <location filename="../docitemattrprefsbase.ui" line="156"/>
        <source>Alt+L</source>
        <translation>Alt+N</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <location filename="../druck.cpp" line="44"/>
        <source>Setup Printer</source>
        <translation>Nastavit tiskárnu</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="556"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="194"/>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="464"/>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="501"/>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="178"/>
        <source>Cyan</source>
        <translation>Azurová (Cyan)</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="178"/>
        <source>Magenta</source>
        <translation>Purpurová (Magenta)</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="178"/>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="180"/>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="275"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="51"/>
        <source>Print Destination</source>
        <translation>Tisk do</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="95"/>
        <source>&amp;Options...</source>
        <translation>&amp;Možnosti...</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="107"/>
        <source>&amp;File:</source>
        <translation>&amp;Soubor:</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="112"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="119"/>
        <source>A&amp;lternative Printer Command</source>
        <translation>A&amp;lternativní příkaz tisku</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="128"/>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Příkaz:</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="135"/>
        <source>Range</source>
        <translation>Interval</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="141"/>
        <source>Print &amp;All</source>
        <translation>Tisknout &amp;vše</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="515"/>
        <source>Print Current Pa&amp;ge</source>
        <translation>Ti&amp;sknout aktuální stranu</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="145"/>
        <source>Print &amp;Range</source>
        <translation>T&amp;isknout interval</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="157"/>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Počet kopií:</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="252"/>
        <source>&amp;Print</source>
        <translation>&amp;Tisk</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="462"/>
        <source>Print Normal</source>
        <translation>Tisknout normálně</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="167"/>
        <source>Print Separations</source>
        <translation>Tisknout separace </translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="171"/>
        <source>Print in Color if Available</source>
        <translation>Tisknout barevně, pokud lze</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="172"/>
        <source>Print in Grayscale</source>
        <translation>Tisknout v odstínech šedé</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="189"/>
        <source>PostScript Level 1</source>
        <translation>PostScript Level 1</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="190"/>
        <source>PostScript Level 2</source>
        <translation>PostScript Level 2</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="191"/>
        <source>PostScript Level 3</source>
        <translation>PostScript Level 3</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="198"/>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="205"/>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Zrcadlit stránku(y) vodorovně</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="208"/>
        <source>Mirror Page(s) Vertical</source>
        <translation>Zrcadlit stránky svisle</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="211"/>
        <source>Set Media Size</source>
        <translation>Nastavit velikost média</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="217"/>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="224"/>
        <source>Apply Under Color Removal</source>
        <translation>Použít během odstranění barvy</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="227"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertovat spot barvy na process</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="233"/>
        <source>Apply ICC Profiles</source>
        <translation>Použít ICC profily</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="238"/>
        <source>Advanced Options</source>
        <translation>Pokročilé možnosti</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="247"/>
        <source>Preview...</source>
        <translation>Náhled...</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="277"/>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Nastaví úroveň PostScriptu.
 Nastavení na Level 1 nebo 2 způsobí vytváření velkých souborů</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="501"/>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Soubory PostScriptu (*.ps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="276"/>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Použít alternativní tiskový program, např. kprinter nebo gtklp, který nabízí další možnosti při tisku</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="278"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="279"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="282"/>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="284"/>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="213"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="448"/>
        <source>Failed to retrieve printer settings</source>
        <translation>Není možné získat nastavení tiskárny</translation>
    </message>
    <message>
        <location filename="../druck.cpp" line="272"/>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Nezobrazovat na tištěné straně objekty přesahující okraje</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="352"/>
        <source>Importing File:
%1
failed!</source>
        <translation>Import souboru:
%1
se nepodařil!</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="353"/>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="606"/>
        <source>Error</source>
        <translation>Chyba</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="54"/>
        <source>Importing PostScript</source>
        <translation>Importuji PostScript</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="57"/>
        <source>Analyzing PostScript:</source>
        <translation>Analyzuji PostScript:</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="358"/>
        <source>Generating Items</source>
        <translation>Vytvářím objekty</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importps.cpp" line="605"/>
        <source>Converting of %1 images failed!</source>
        <translation>Konverze %1 obrázků selhala!</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <location filename="../edit1format.cpp" line="39"/>
        <source>Edit Style</source>
        <translation>Upravit styl</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="72"/>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="264"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="366"/>
        <source>Line Spacing</source>
        <translation>Řádkování</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="353"/>
        <source>Name of your paragraph style</source>
        <translation>Jméno stylu odstavce</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="354"/>
        <source>Font of selected text or object</source>
        <translation>Písmo vybraného textu nebo objektu</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="355"/>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="356"/>
        <source>Color of text fill</source>
        <translation>Barva výplně písma</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="357"/>
        <source>Color of text stroke</source>
        <translation>Barva obrysu písma</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="359"/>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Určuje celkovou výšku iniciál počtem řádků</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="362"/>
        <source>Spacing above the paragraph</source>
        <translation>Horní odsazení odstavce</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="363"/>
        <source>Spacing below the paragraph</source>
        <translation>Dolní odsazení odstavce</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="292"/>
        <source>Tabulators and Indentation</source>
        <translation>Tabelátory a odsazení</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="59"/>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="260"/>
        <source>&amp;Lines:</source>
        <translation>Čá&amp;ry:</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="231"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="192"/>
        <source>Distances</source>
        <translation>Vzdálenosti</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="201"/>
        <source>Fixed Linespacing</source>
        <translation>Pevné řádkování</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="202"/>
        <source>Automatic Linespacing</source>
        <translation>Automatické řádkování</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="203"/>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnat k pomocné mřížce</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="249"/>
        <source>Drop Caps</source>
        <translation>Iniciály</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="266"/>
        <source>Distance from Text:</source>
        <translation>Vzdálenost od textu:</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="316"/>
        <source>Preview of the Paragraph Style</source>
        <translation>Náhled odstavcového stylu</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="360"/>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Určuje odstup mezi iniciálou a textem</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="368"/>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Přepíná ukázkový text tohoto odstavcového stylu</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="501"/>
        <source>Name of the style is not unique</source>
        <translation>Jméno stylu není jedinečné</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="320"/>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="352"/>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Vyberte pro snazší čtení lehce barevných textových stylů</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="364"/>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="365"/>
        <source>Offset to baseline of characters</source>
        <translation>Vzdálenost k účaří znaků</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="367"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Stiskněte a držte tlačítko pro výběr řádkování.</translation>
    </message>
    <message>
        <location filename="../edit1format.cpp" line="441"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <location filename="../editor.cpp" line="23"/>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="31"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="32"/>
        <source>&amp;Open...</source>
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="33"/>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="35"/>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Uložit a skončit</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="36"/>
        <source>&amp;Exit without Saving</source>
        <translation>S&amp;končit bez uložení</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="38"/>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="39"/>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="41"/>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="42"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="43"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="44"/>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="46"/>
        <source>&amp;Get Field Names</source>
        <translation>Získat &amp;jména polí</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="48"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="49"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../editor.cpp" line="86"/>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripty (*.js);;Všechny soubory (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <location filename="../effectsdialog.cpp" line="32"/>
        <source>Image Effects</source>
        <translation>Efekty obrázku</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="61"/>
        <source>Options:</source>
        <translation>Volby:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="74"/>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="85"/>
        <source>Shade:</source>
        <translation>Stín:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="97"/>
        <source>Brightness:</source>
        <translation>Jas:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="115"/>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="152"/>
        <source>Radius:</source>
        <translation>Poloměr:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="159"/>
        <source>Value:</source>
        <translation>Hodnota:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="171"/>
        <source>Posterize:</source>
        <translation>Posterizovat:</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="228"/>
        <source>Available Effects</source>
        <translation>Dostupné efekty</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="776"/>
        <source>Blur</source>
        <translation>Blur</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="754"/>
        <source>Brightness</source>
        <translation>Jas</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="745"/>
        <source>Colorize</source>
        <translation>Kolorovat</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="760"/>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="626"/>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="628"/>
        <source>Invert</source>
        <translation>Invertovat</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="786"/>
        <source>Posterize</source>
        <translation>Posterizovat</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="766"/>
        <source>Sharpen</source>
        <translation>Zaostřit</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="212"/>
        <source>&gt;&gt;</source>
        <translation>&gt;&gt;</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="216"/>
        <source>&lt;&lt;</source>
        <translation>&lt;&lt;</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="225"/>
        <source>Effects in use</source>
        <translation>Použité efekty</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="299"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../effectsdialog.cpp" line="303"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="198"/>
        <source>&amp;All pages</source>
        <translation>Všechny str&amp;any</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="212"/>
        <source>Change the output directory</source>
        <translation>Změnit výstupní adresář</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="211"/>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Výstupní adresář - místo, kde budou uloženy vaše obrázky.
Jména exportovaných souborů budou ve tvaru &apos;jmenodokumentu-cislostrany.typ&apos;</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="206"/>
        <source>Export only the current page</source>
        <translation>Exportovat pouze aktuální stranu</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="210"/>
        <source>Available export formats</source>
        <translation>Dostupné exportní formáty</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="141"/>
        <source>Choose a Export Directory</source>
        <translation>Vyberte adresář</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="187"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="186"/>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Export do adresáře:</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="189"/>
        <source>Image &amp;Type:</source>
        <translation>&amp;Typ obrázku:</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="190"/>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalita:</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="185"/>
        <source>Export as Image(s)</source>
        <translation>Uložit jako obrázky</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="188"/>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="191"/>
        <source>&amp;Resolution:</source>
        <translation>&amp;Rozlišení:</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="195"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="194"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="196"/>
        <source>Range</source>
        <translation>Interval</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="197"/>
        <source>&amp;Current page</source>
        <translation>&amp;Aktuální strana</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="199"/>
        <source>&amp;Range</source>
        <translation>&amp;Interval</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="202"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="203"/>
        <source>Export a range of pages</source>
        <translation>Uložit interval stran</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="204"/>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="205"/>
        <source>Export all pages</source>
        <translation>Uložit všechny strany</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="207"/>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Rozlišení obrázků
Použijte 72 dpi, jestliže je obrázek určen na web nebo monitor</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="209"/>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kvalita obrázků - 100% je nejvyšší, 1% nejnižší</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="192"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/dialog.cpp" line="208"/>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Velikost obrázků. 100% beze změny, 200% dvakrát větší atd.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <location filename="../extimageprops.cpp" line="33"/>
        <source>Extended Image Properties</source>
        <translation>Rozšířené vlastnosti obrázku</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="174"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="85"/>
        <source>Darken</source>
        <translation>Ztmavit</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="86"/>
        <source>Lighten</source>
        <translation>Zesvětlit</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="87"/>
        <source>Hue</source>
        <translation>Odstín</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="88"/>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="89"/>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="90"/>
        <source>Luminosity</source>
        <translation>Světlost</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="91"/>
        <source>Multiply</source>
        <translation>Násobit</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="92"/>
        <source>Screen</source>
        <translation>Obrazovka</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="93"/>
        <source>Dissolve</source>
        <translation>Rozpuštění</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="94"/>
        <source>Overlay</source>
        <translation type="unfinished">Overlay</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="95"/>
        <source>Hard Light</source>
        <translation>Studené světlo</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="96"/>
        <source>Soft Light</source>
        <translation>Teplé světlo</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="97"/>
        <source>Difference</source>
        <translation>Rozdíl</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="53"/>
        <source>Exclusion</source>
        <translation>Exkluze</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="99"/>
        <source>Color Dodge</source>
        <translation type="unfinished">Barevný fígl</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="100"/>
        <source>Color Burn</source>
        <translation type="unfinished">Vypálení barvy</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="98"/>
        <source>Exlusion</source>
        <translation>Exkluze</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="80"/>
        <source>Blend Mode:</source>
        <translation>Režim směšování:</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="103"/>
        <source>Opacity:</source>
        <translation>Neprůsvitnost:</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="109"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="118"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="185"/>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="195"/>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="240"/>
        <source>Don&apos;t use any Path</source>
        <translation>Nepoužívat žádnou cestu</translation>
    </message>
    <message>
        <location filename="../extimageprops.cpp" line="242"/>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <location filename="../customfdialog.cpp" line="166"/>
        <source>Size:</source>
        <translation>Velikost:</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="205"/>
        <source>Title:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="208"/>
        <source>No Title</source>
        <translation>Bez názvu</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="210"/>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="213"/>
        <source>Unknown</source>
        <translation>Neznámý</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="215"/>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="167"/>
        <source>Resolution:</source>
        <translation>Rozlišení:</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="167"/>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="176"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="179"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="182"/>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <location filename="../customfdialog.cpp" line="186"/>
        <source>Colorspace:</source>
        <translation>Barevný prostor:</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <location filename="../fileloader.cpp" line="2932"/>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Některá písma, použitá v tomto dokumentu, byla nahrazena:</translation>
    </message>
    <message>
        <location filename="../fileloader.cpp" line="2936"/>
        <source> was replaced by: </source>
        <translation> nahrazeno: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <location filename="../fontprefs.cpp" line="34"/>
        <source>Available Fonts</source>
        <translation>Dostupná písma</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="34"/>
        <source>Font Substitutions</source>
        <translation>Náhrady písem</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="34"/>
        <source>Additional Paths</source>
        <translation>Dodatečné cesty</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="125"/>
        <source>Font Name</source>
        <translation>Název písma</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="126"/>
        <source>Replacement</source>
        <translation>Náhrada</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="329"/>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="113"/>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Dostupná písma</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="153"/>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Substituce písem</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="193"/>
        <source>Additional &amp;Paths</source>
        <translation>Dodatečné &amp;cesty</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="147"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="168"/>
        <source>C&amp;hange...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="170"/>
        <source>A&amp;dd...</source>
        <translation>&amp;Připojit...</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="172"/>
        <source>&amp;Remove</source>
        <translation>&amp;Odstranit</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="40"/>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Název písma</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="41"/>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Použít písmo</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="42"/>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Obsažen v:</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="43"/>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Podmnožina glyphů</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="45"/>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Cesta k souboru s písmem</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <location filename="../fontprefs.cpp" line="189"/>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation>Cesty k písmům lze nastavit, jen pokud nejsou otevřené žádné dokumenty. Zavřete všechny dokumenty a potom změňte cestu pro písma v nabídce Soubor - Nastavení - Písma.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="107"/>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Přidat zvolené písmo do Styl, Písmo menu</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="108"/>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Zavřít náhled písem</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="110"/>
        <source>Start searching</source>
        <translation>Spustit hledání</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="111"/>
        <source>Size of the selected font</source>
        <translation>Velikost zvoleného písma</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="75"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="201"/>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Uživatel</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="202"/>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Systém</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="126"/>
        <source>Sample will be shown after key release</source>
        <translation>Ukázka bude zobrazena až po puštění tlačítka</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreview.cpp" line="109"/>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <location filename="" line="7471221"/>
        <source>Fonts Preview</source>
        <translation type="obsolete">Náhled písem</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="43"/>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Rychlé hledání:</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="59"/>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="62"/>
        <source>Alt+S</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="70"/>
        <source>Font Name</source>
        <translation>Název písma</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="81"/>
        <source>Doc</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="92"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="103"/>
        <source>Subset</source>
        <translation>Podmnožina glyphů</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="114"/>
        <source>Access</source>
        <translation>Přístup</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="168"/>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost písma:</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="200"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="208"/>
        <source>Sample text to display</source>
        <translation>Text, který se zobrazí</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="216"/>
        <source>Se&amp;t</source>
        <translation>Na&amp;stavit</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="219"/>
        <source>Alt+T</source>
        <translation>Alt-T</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="230"/>
        <source>Reset the text</source>
        <translation>Obnovit text</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="267"/>
        <source>&amp;Append</source>
        <translation>Připoji&amp;t</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="270"/>
        <source>Alt+A</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="278"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="281"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewbase.ui" line="16"/>
        <source>Font Preview</source>
        <translation>Náhled písem</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewplugin.cpp" line="49"/>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Náhled písem...</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewplugin.cpp" line="65"/>
        <source>Font Preview dialog</source>
        <translation>Dialog náhledu písem</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewplugin.cpp" line="66"/>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Třídění, prohledávání a procházení písem.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <location filename="../fontreplacedialog.cpp" line="27"/>
        <source>Font Substitution</source>
        <translation>Náhrady písem</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="39"/>
        <source>Original Font</source>
        <translation>Původní písmo</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="40"/>
        <source>Substitution Font</source>
        <translation>Nové písmo</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="62"/>
        <source>Make these substitutions permanent</source>
        <translation>Nastavit tyto náhrady jako trvalé</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="34"/>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Tento dokument obsahuje některá písma, která nejsou na vašem systému instalována, vyberte prosím jejich odpovídající náhrady. Zrušit načítání dokumentu zastaví.</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="73"/>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Zruší náhrady písem a zastaví načítání dokumentu.</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="74"/>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Pokud volbu povolíte, Scribus použije tyto náhrady chybějících písem ve všech budoucích dokumentech. Později to lze zrušit v nabídce Úpravy - Nastavení - Písma.</translation>
    </message>
    <message>
        <location filename="../fontreplacedialog.cpp" line="75"/>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Pokud vyberete OK a uložíte, náhrady budou pro dokument trvalé.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <location filename="../gradienteditor.cpp" line="301"/>
        <source>Position:</source>
        <translation>Pozice:</translation>
    </message>
    <message>
        <location filename="../gradienteditor.cpp" line="302"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../gradienteditor.cpp" line="300"/>
        <source>Add, change or remove color stops here</source>
        <translation>Přidat, změnit nebo odebrat zarážky barev</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <location filename="../guidemanager.cpp" line="69"/>
        <source>Manage Guides</source>
        <translation>Ovládání vodítek</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="87"/>
        <source>Horizontal Guides</source>
        <translation>Vodorovná vodítka</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="129"/>
        <source>Vertical Guides</source>
        <translation>Svislá vodítka</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="105"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="116"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="120"/>
        <source>D&amp;elete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="147"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="158"/>
        <source>A&amp;dd</source>
        <translation>Přip&amp;ojit</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="162"/>
        <source>De&amp;lete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="276"/>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Zamknout vodítka</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="176"/>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Řady a sloupce - automatická vodítka</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="184"/>
        <source>&amp;Rows:</source>
        <translation>Řá&amp;dky:</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="188"/>
        <source>C&amp;olumns:</source>
        <translation>&amp;Sloupce:</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="199"/>
        <source>Row &amp;Gap</source>
        <translation>&amp;Mezera mezi řádky</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="205"/>
        <source>Colum&amp;n Gap</source>
        <translation>M&amp;ezera mezi sloupci</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="225"/>
        <source>Refer to:</source>
        <translation>Odkaz na:</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="227"/>
        <source>&amp;Page</source>
        <translation>Str&amp;ana</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="229"/>
        <source>&amp;Margins</source>
        <translation>&amp;Okraje</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="230"/>
        <source>&amp;Selection</source>
        <translation>&amp;Výběr</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="249"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="252"/>
        <source>&amp;Update</source>
        <translation>&amp;Aktualizovat</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="296"/>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Nastaví v dokumentu vodítka. Správce vodítek je stále otevřený, ale změny jsou trvalé</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="280"/>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;Použít pro všechny strany</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="137"/>
        <source>Guide</source>
        <translation>Vodítko</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="138"/>
        <source>Unit</source>
        <translation>Jednotka</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="265"/>
        <source>Preview</source>
        <translation>Náhled</translation>
    </message>
    <message>
        <location filename="../guidemanager.cpp" line="362"/>
        <source>There is empty (0.0) guide already</source>
        <translation>Již existuje prázdné (0.0) vodítko</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <location filename="../helpbrowser.cpp" line="487"/>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Lituji, ale není dostupný žádný manuál! Navštivte prosím: http://docs.scribus.net
kde naleznete aktuální dokumentaci.</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="415"/>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="300"/>
        <source>Link</source>
        <translation>Odkaz</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="414"/>
        <source>Scribus Online Help</source>
        <translation>Scribus online nápověda</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="418"/>
        <source>&amp;Contents</source>
        <translation>&amp;Obsah</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="276"/>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="793"/>
        <source>Find</source>
        <translation>Najít</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="793"/>
        <source>Search Term:</source>
        <translation>Hledaný řetězec:</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="293"/>
        <source>Se&amp;arch</source>
        <translation>Hl&amp;edat</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="310"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="340"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="312"/>
        <source>De&amp;lete All</source>
        <translation>S&amp;mazat vše</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="317"/>
        <source>Book&amp;marks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="329"/>
        <source>&amp;Print...</source>
        <translation>&amp;Tisk...</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="331"/>
        <source>E&amp;xit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="419"/>
        <source>Searching is case unsensitive</source>
        <translation>Hlednání ignoruje velikost písmen</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="858"/>
        <source>New Bookmark</source>
        <translation>Nová záložka</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="858"/>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Nadpis nové záložky:</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="332"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="334"/>
        <source>&amp;Find...</source>
        <translation>&amp;Hledat...</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="335"/>
        <source>Find &amp;Next</source>
        <translation>Hledat &amp;další</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="336"/>
        <source>Find &amp;Previous</source>
        <translation>Hledat &amp;předchozí</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="337"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="339"/>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Přidat záložku</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="341"/>
        <source>D&amp;elete All</source>
        <translation>S&amp;mazat vše</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="343"/>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Záložky</translation>
    </message>
    <message>
        <location filename="../helpbrowser.cpp" line="282"/>
        <source>Relevance</source>
        <translation>Relevance</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <location filename="../hyask.cpp" line="44"/>
        <source>Possible Hyphenation</source>
        <translation>Přijatelné dělení slov</translation>
    </message>
    <message>
        <location filename="../hyask.cpp" line="59"/>
        <source>Accept</source>
        <translation>Akceptovat</translation>
    </message>
    <message>
        <location filename="../hyask.cpp" line="65"/>
        <source>Skip</source>
        <translation>Přeskočit</translation>
    </message>
    <message>
        <location filename="../hyask.cpp" line="70"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <location filename="../hysettings.cpp" line="46"/>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Délka nejkratšího slova, které může být děleno.</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="47"/>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximální počet po sobě následujících dělení slov.
Nula (0) funkci vypíná. Pozn. překl.: V české typografii max. 3.</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="28"/>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="34"/>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Nejmenší slovo:</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="16"/>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Návrhy &amp;dělění</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="19"/>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;Automaticky dělit slova při psaní</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="44"/>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Po volbě &quot;Extra&quot;, &quot;Dělení slov v textu&quot; se objeví dialog,
ve kterém budou zobrazeny všechny možnosti dělení slova.</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="45"/>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Povolí automatické dělení slov během psaní textu.</translation>
    </message>
    <message>
        <location filename="../hysettings.cpp" line="40"/>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Maximální počet po sobě následujících dělění:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <location filename="../imageinfodialog.cpp" line="15"/>
        <source>Image Info</source>
        <translation>Informace o obrázku</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="20"/>
        <source>General Info</source>
        <translation>Obecné informace</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="25"/>
        <source>Date / Time:</source>
        <translation>Datum/čas:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="29"/>
        <source>Has Embedded Profile:</source>
        <translation>Má vložený profil:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="54"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="56"/>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="37"/>
        <source>Profile Name:</source>
        <translation>Jméno profilu:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="41"/>
        <source>Has Embedded Paths:</source>
        <translation>Má vložené cesty:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="49"/>
        <source>Has Layers:</source>
        <translation>Má vrstvy:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="60"/>
        <source>EXIF Info</source>
        <translation>EXIF informace</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="76"/>
        <source>Artist:</source>
        <translation>Umělec:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="94"/>
        <source>Comment:</source>
        <translation>Komentář:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="95"/>
        <source>User Comment:</source>
        <translation>Uživatelův komentář:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="96"/>
        <source>Camera Model:</source>
        <translation>Typ fotoaparátu:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="97"/>
        <source>Camera Manufacturer:</source>
        <translation>Výrobce fotoaparátu:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="100"/>
        <source>Description:</source>
        <translation>Popis:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="101"/>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="102"/>
        <source>Scanner Model:</source>
        <translation>Typ skeneru:</translation>
    </message>
    <message>
        <location filename="../imageinfodialog.cpp" line="103"/>
        <source>Scanner Manufacturer:</source>
        <translation>Výrobce skeneru:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="53"/>
        <source>Import &amp;EPS/PS...</source>
        <translation>Import &amp;EPS/PS...</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="74"/>
        <source>Imports EPS Files</source>
        <translation>Import EPS souborů</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="75"/>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje do dokumentu většinu EPS souborů,
přičemž převede vektorová data na objekty Scribusu.</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="101"/>
        <source>PDF</source>
        <translation>PDF</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <location filename="../inspage.cpp" line="31"/>
        <source>Insert Page</source>
        <translation>Vložit stranu</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="46"/>
        <source>before Page</source>
        <translation>před stranu</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="47"/>
        <source>after Page</source>
        <translation>za stranu</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="48"/>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="87"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="42"/>
        <source>Page(s)</source>
        <translation>Strana(y)</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="58"/>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="62"/>
        <source>Master Pages</source>
        <translation>Vzorové strany</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="75"/>
        <source>&amp;Master Page:</source>
        <translation>&amp;Vzorová strana:</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="99"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="107"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="211"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="123"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="126"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="127"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="133"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="142"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../inspage.cpp" line="146"/>
        <source>Move Objects with their Page</source>
        <translation>Přesunout objekty společně s jejich stranou</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <location filename="../insertTable.cpp" line="18"/>
        <source>Insert Table</source>
        <translation>Vložit tabulku</translation>
    </message>
    <message>
        <location filename="../insertTable.cpp" line="25"/>
        <source>Number of rows:</source>
        <translation>Počet řádek:</translation>
    </message>
    <message>
        <location filename="../insertTable.cpp" line="26"/>
        <source>Number of columns:</source>
        <translation>Počet sloupců:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <location filename="../javadocs.cpp" line="71"/>
        <source>New Script</source>
        <translation>Nový skript</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="22"/>
        <source>Edit JavaScripts</source>
        <translation>Editace Java skriptů</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="37"/>
        <source>&amp;Edit...</source>
        <translation>&amp;Upravit...</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="40"/>
        <source>&amp;Add...</source>
        <translation>&amp;Přidat...</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="43"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="48"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="70"/>
        <source>&amp;New Script:</source>
        <translation>&amp;Nový skript:</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="118"/>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="117"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="116"/>
        <source>Do you really want to delete this script?</source>
        <translation>Opravdu chcete smazat tento skript?</translation>
    </message>
    <message>
        <location filename="../javadocs.cpp" line="64"/>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Přidá nový skript a definuje funkci se stejným jménem. Pokud chcete tento skript použít jako skript pro &quot;otevřenou akci&quot;, ujistěte se, že nezměníte jméno funkce.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <location filename="../keymanager.cpp" line="124"/>
        <source>Action</source>
        <translation>Akce</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="125"/>
        <source>Current Key</source>
        <translation>Aktuální klávesa</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="137"/>
        <source>Select a Key for this Action</source>
        <translation>Zvolte klávesu pro tuto akci</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="151"/>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="298"/>
        <source>Alt</source>
        <translation>Alt</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="298"/>
        <source>Ctrl</source>
        <translation>Ctrl</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="298"/>
        <source>Shift</source>
        <translation>Shift</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="246"/>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="250"/>
        <source>Alt+</source>
        <translation>Alt+</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="254"/>
        <source>Ctrl+</source>
        <translation>ctrl+</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="146"/>
        <source>&amp;No Key</source>
        <translation>Žádná &amp;klávesa</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="148"/>
        <source>&amp;User Defined Key</source>
        <translation>Definováno &amp;uživatelem</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="158"/>
        <source>Set &amp;Key</source>
        <translation>&amp;Nastavit klávesu</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="164"/>
        <source>Loadable Shortcut Sets</source>
        <translation>Sady klávesových zkratek k načtení</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="175"/>
        <source>&amp;Load</source>
        <translation>&amp;Načíst</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="182"/>
        <source>&amp;Import...</source>
        <translation>&amp;Import...</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="183"/>
        <source>&amp;Export...</source>
        <translation>&amp;Export...</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="185"/>
        <source>&amp;Reset</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="197"/>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Sady klávesových zkratek, které lze načíst</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="198"/>
        <source>Load the selected shortcut set</source>
        <translation>Načíst vybranou sadu klávesových zkratek</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="199"/>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importovat do současného nastavení sadu klávesových zkratek</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="200"/>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Exportovat současné klávesové zkratky do souboru</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="201"/>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Znovu načíst předdefinované klávesové zkratky</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="436"/>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>XML soubory klávesových zkratek (*.ksxml)</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="265"/>
        <source>This key sequence is already in use</source>
        <translation>Tato posloupnost kláves se již používá</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="298"/>
        <source>Meta</source>
        <translation>Meta</translation>
    </message>
    <message>
        <location filename="../keymanager.cpp" line="242"/>
        <source>Meta+</source>
        <translation>Meta+</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <location filename="../layers.cpp" line="361"/>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="237"/>
        <source>Delete Layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="238"/>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Opravdu chcete smazat také všechny objekty v této vrstvě?</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="362"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="367"/>
        <source>Add a new layer</source>
        <translation>Přidat vrstvu</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="368"/>
        <source>Delete layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="369"/>
        <source>Raise layer</source>
        <translation>Vrstvu dopředu</translation>
    </message>
    <message>
        <location filename="../layers.cpp" line="370"/>
        <source>Lower layer</source>
        <translation>Vrstvu dozadu</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <location filename="../lineformats.cpp" line="28"/>
        <source>Edit Line Styles</source>
        <translation>Editace stylů čar</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="120"/>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="148"/>
        <source>New Style</source>
        <translation>Nový styl</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="215"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="213"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="215"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="47"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="50"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="55"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="59"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="63"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="190"/>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="189"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="44"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <location filename="../lineformats.cpp" line="188"/>
        <source>Do you really want to delete this style?</source>
        <translation>Opravdu chcete smazat tento styl?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <location filename="../smlinestylewbase.ui" line="214"/>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../smlinestylewbase.ui" line="239"/>
        <source>Line Width:</source>
        <translation>Šířka čáry:</translation>
    </message>
</context>
<context>
    <name>LineStyleWidget</name>
    <message>
        <location filename="../smlinestyle.cpp" line="144"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="54"/>
        <source>Flat Cap</source>
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="55"/>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="56"/>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="58"/>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="59"/>
        <source>Bevel Join</source>
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="60"/>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="163"/>
        <source>Solid Line</source>
        <translation>Plná čára</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="151"/>
        <source>Dashed Line</source>
        <translation>Čárkovaná čára</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="154"/>
        <source>Dotted Line</source>
        <translation>Tečkovaná čára</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="157"/>
        <source>Dash Dot Line</source>
        <translation>Čerchovaná čára</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="160"/>
        <source>Dash Dot Dot Line</source>
        <translation>Dvojitě čerchovaná čára</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <location filename="../loadsaveplugin.cpp" line="66"/>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <location filename="../loremipsum.cpp" line="113"/>
        <source>Select Lorem Ipsum</source>
        <translation>Vybrat Lorem Ipsum</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="165"/>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="166"/>
        <source>Get More:</source>
        <translation>Více:</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="167"/>
        <source>XML File:</source>
        <translation>XML soubor:</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="181"/>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="182"/>
        <source>Paragraphs:</source>
        <translation>Odstavce:</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="184"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../loremipsum.cpp" line="186"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <location filename="../margindialog.cpp" line="30"/>
        <source>Manage Page Properties</source>
        <translation>Vlastnosti strany</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="36"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="44"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="208"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="60"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="63"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="64"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="71"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="80"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="84"/>
        <source>Move Objects with their Page</source>
        <translation>Přesunout objekty společně s jejich stránkou</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="90"/>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <location filename="../margindialog.cpp" line="110"/>
        <source>Margin Guides</source>
        <translation>Vodítka okrajů</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <location filename="scribus/marginwidget.cpp" line="53"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="54"/>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="110"/>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="109"/>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="93"/>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Vzdálenost mezi horním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="94"/>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Vzdálenost mezi dolním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="109"/>
        <source>&amp;Inside:</source>
        <translation>&amp;Uvnitř:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="110"/>
        <source>O&amp;utside:</source>
        <translation>&amp;Vnější:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="35"/>
        <source>Preset Layouts:</source>
        <translation>Předdefinované vzhledy:</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="74"/>
        <source>Apply margin settings to all pages</source>
        <translation>Použít nastavení okrajů na všechny stránky</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="78"/>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Použít změny okrajů na všechny existující stránky v dokumentu</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="95"/>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="96"/>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="86"/>
        <source>Printer Margins...</source>
        <translation>Okraje tiskárny...</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="88"/>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importovat okraje pro vybranou velikost stránky z dostupných tiskáren.</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <location filename="../muster.cpp" line="30"/>
        <source>Edit Master Pages</source>
        <translation>Upravit vzorové strany</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="102"/>
        <source>Do you really want to delete this master page?</source>
        <translation>Opravdu chcete smazat tuto vzorovou stranu?</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="104"/>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="103"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="129"/>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="129"/>
        <source>New Master Page</source>
        <translation>Nová vzorová strana</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="129"/>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="269"/>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="269"/>
        <source>New MasterPage</source>
        <translation>Nová vzorová strana</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="331"/>
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="407"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="71"/>
        <source>Duplicate the selected master page</source>
        <translation>Duplikovat zvolenou vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="72"/>
        <source>Delete the selected master page</source>
        <translation>Smazat zvolenou vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="73"/>
        <source>Add a new master page</source>
        <translation>Přidat novou vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="74"/>
        <source>Import master pages from another document</source>
        <translation>Import vzorových stran z existujícího dokumentu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="269"/>
        <source>New Master Page %1</source>
        <translation>Nová vzorová strana %1</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="418"/>
        <source>Unable to Rename Master Page</source>
        <translation>Nelze přejmenovat vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="418"/>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>Normální strana nemůže být přejmenována.</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="423"/>
        <source>Rename Master Page</source>
        <translation>Přejmenovat vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../muster.cpp" line="423"/>
        <source>New Name:</source>
        <translation>Nové jméno:</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <location filename="../mdup.cpp" line="24"/>
        <source>Multiple Duplicate</source>
        <translation>Vícenásobné duplikování</translation>
    </message>
    <message>
        <location filename="../mdup.cpp" line="46"/>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Počet kopií:</translation>
    </message>
    <message>
        <location filename="../mdup.cpp" line="48"/>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vodorovné posunutí:</translation>
    </message>
    <message>
        <location filename="../mdup.cpp" line="50"/>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Svislé posunutí:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <location filename="../measurements.cpp" line="120"/>
        <source>Distances</source>
        <translation>Vzdálenosti</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="122"/>
        <source>X1:</source>
        <translation>X1:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="123"/>
        <source>Y1:</source>
        <translation>Y1:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="124"/>
        <source>X2:</source>
        <translation>X2:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="125"/>
        <source>Y2:</source>
        <translation>Y2:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="126"/>
        <source>DX:</source>
        <translation>DX:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="127"/>
        <source>DY:</source>
        <translation>DY:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="128"/>
        <source>Angle:</source>
        <translation>Úhel:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="129"/>
        <source>Length:</source>
        <translation>Délka:</translation>
    </message>
    <message>
        <location filename="../measurements.cpp" line="54"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <location filename="../mergedoc.cpp" line="131"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="129"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="131"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="38"/>
        <source>Import Page(s)</source>
        <translation>Importovat stranu(y)</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="71"/>
        <source> from 0</source>
        <translation>od 0</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="74"/>
        <source>Create Page(s)</source>
        <translation>Vytvořit stranu(y)</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="183"/>
        <source> from %1</source>
        <translation>od %1</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="38"/>
        <source>Import Master Page</source>
        <translation>Importovat vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="45"/>
        <source>&amp;From Document:</source>
        <translation>&amp;Z dokumentu:</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="48"/>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="51"/>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importovat stranu(y):</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="55"/>
        <source>&amp;Import Master Page</source>
        <translation>Importovat &amp;vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="68"/>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="78"/>
        <source>Before Page</source>
        <translation>Před stranu</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="79"/>
        <source>After Page</source>
        <translation>Za stranu</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="80"/>
        <source>At End</source>
        <translation>Na konec</translation>
    </message>
    <message>
        <location filename="../mergedoc.cpp" line="93"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <location filename="../missing.cpp" line="309"/>
        <source>Missing Font</source>
        <translation>Chybějící písmo</translation>
    </message>
    <message>
        <location filename="../missing.cpp" line="315"/>
        <source>The Font %1 is not installed.</source>
        <translation>Písmo %1 není nainstalované.</translation>
    </message>
    <message>
        <location filename="../missing.cpp" line="320"/>
        <source>Use</source>
        <translation>Použít</translation>
    </message>
    <message>
        <location filename="../missing.cpp" line="334"/>
        <source>instead</source>
        <translation>místo</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <location filename="../movepage.cpp" line="32"/>
        <source>Move Pages</source>
        <translation>Přesunout strany</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="38"/>
        <source>Copy Page</source>
        <translation>Kopírovat stranu</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="74"/>
        <source>Move Page(s):</source>
        <translation>Posun stran:</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="38"/>
        <source>Move Page(s)</source>
        <translation>Posun stran(y)</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="65"/>
        <source>Before Page</source>
        <translation>Před stranu</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="66"/>
        <source>After Page</source>
        <translation>Za stranu</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="67"/>
        <source>At End</source>
        <translation>Na konec</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="47"/>
        <source>To:</source>
        <translation>po:</translation>
    </message>
    <message>
        <location filename="../movepage.cpp" line="55"/>
        <source>Number of copies:</source>
        <translation>Počet kopií:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <location filename="../mpalette.cpp" line="3742"/>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3751"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3752"/>
        <source>Geometry</source>
        <translation>Geometrie</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3853"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3758"/>
        <source>Basepoint:</source>
        <translation>Střed otáčení:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3759"/>
        <source>Level</source>
        <translation>Hladina</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3760"/>
        <source>Shape:</source>
        <translation>Obrys:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3763"/>
        <source>Distance of Text</source>
        <translation>Vzdálenost textu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3772"/>
        <source>Show Curve</source>
        <translation>Zobrazit křivku</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3773"/>
        <source>Start Offset:</source>
        <translation>Počáteční posun:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3774"/>
        <source>Distance from Curve:</source>
        <translation>Vzdálenost od křivky:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3845"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3792"/>
        <source>Input Profile:</source>
        <translation>Vložit profil:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3793"/>
        <source>Rendering Intent:</source>
        <translation>Účel reprodukce:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3799"/>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3800"/>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3801"/>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3802"/>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3806"/>
        <source>Left Point</source>
        <translation>Levý bod</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3807"/>
        <source>End Points</source>
        <translation>Koncové body</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3826"/>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3827"/>
        <source>Bevel Join</source>
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3828"/>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3833"/>
        <source>Flat Cap</source>
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3834"/>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3835"/>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3878"/>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3983"/>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3993"/>
        <source>Line Spacing</source>
        <translation>Řádkování</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3955"/>
        <source>Name of selected object</source>
        <translation>Jméno vybraného objektu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3956"/>
        <source>Horizontal position of current basepoint</source>
        <translation>Vodorovné umístění středu otáčení</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3957"/>
        <source>Vertical position of current basepoint</source>
        <translation>Svislé umístění středu otáčení</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3958"/>
        <source>Width</source>
        <translation>Šířka</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3959"/>
        <source>Height</source>
        <translation>Výška</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3960"/>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotace objektu podle aktuálního středu otáčení</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3961"/>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Bod (počátek souřadnic), od kterého jsou 
odvozeny vzdálenosti nebo úhly rotace</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3962"/>
        <source>Select top left for basepoint</source>
        <translation>Střed otáčení vlevo nahoře</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3963"/>
        <source>Select top right for basepoint</source>
        <translation>Střed otáčení vpravo nahoře</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3964"/>
        <source>Select bottom left for basepoint</source>
        <translation>Střed otáčení vlevo dole</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3965"/>
        <source>Select bottom right for basepoint</source>
        <translation>Střed otáčení vpravo dole</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3966"/>
        <source>Select center for basepoint</source>
        <translation>Střed otáčení uprostřed</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3967"/>
        <source>Flip Horizontal</source>
        <translation>Překlopit vodorovně</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3968"/>
        <source>Flip Vertical</source>
        <translation>Překlopit svisle</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3969"/>
        <source>Move one level up</source>
        <translation>O hladinu výš</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3970"/>
        <source>Move one level down</source>
        <translation>O hladinu níž</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3971"/>
        <source>Move to front</source>
        <translation>Přesunout navrch</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3972"/>
        <source>Move to back</source>
        <translation>Přesunout dospodu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3974"/>
        <source>Lock or unlock the object</source>
        <translation>Zamknout nebo odemknout objekt</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3975"/>
        <source>Lock or unlock the size of the object</source>
        <translation>Zamknout nebo odemknout rozměry objektu</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Enable or disable printing of the object</source>
        <translation type="obsolete">Povolit nebo zakázat tisk objektu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3982"/>
        <source>Font of selected text or object</source>
        <translation>Písmo vybraného textu nebo objektu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3985"/>
        <source>Scaling width of characters</source>
        <translation>Změna šířky znaků písma</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3989"/>
        <source>Saturation of color of text stroke</source>
        <translation>Sytost barvy obrysu písma</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3990"/>
        <source>Saturation of color of text fill</source>
        <translation>Sytost barvy výplně písma</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3995"/>
        <source>Style of current paragraph</source>
        <translation>Styl aktuálního odstavce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3998"/>
        <source>Change settings for left or end points</source>
        <translation>Změna nastavení bodů čáry</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3999"/>
        <source>Pattern of line</source>
        <translation>Styl čáry</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4000"/>
        <source>Thickness of line</source>
        <translation>Šířka (tloušťka) čáry</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4001"/>
        <source>Type of line joins</source>
        <translation>Typy spojení čar</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4002"/>
        <source>Type of line end</source>
        <translation>Typy ukončení čáry</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4003"/>
        <source>Line style of current object</source>
        <translation>Styl čáry aktuálního objektu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4005"/>
        <source>Choose the shape of frame...</source>
        <translation>Výběr obrys rámu...</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4006"/>
        <source>Edit shape of the frame...</source>
        <translation>Úprava obrysu rámu...</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4007"/>
        <source>Set radius of corner rounding</source>
        <translation>Nastavní poloměru zakulacení rohů</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4008"/>
        <source>Number of columns in text frame</source>
        <translation>Počet sloupců v textovém rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4010"/>
        <source>Distance between columns</source>
        <translation>Vzdálenost mezi sloupci</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4011"/>
        <source>Distance of text from top of frame</source>
        <translation>Vzdálenost textu od horní části rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4012"/>
        <source>Distance of text from bottom of frame</source>
        <translation>Vzdálenost textu od spodního obrysu rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4013"/>
        <source>Distance of text from left of frame</source>
        <translation>Vzdálenost textu od levého obrysu rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4014"/>
        <source>Distance of text from right of frame</source>
        <translation>Vzdálenost textu od pravého obrysu rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4015"/>
        <source>Edit tab settings of text frame...</source>
        <translation>Nastavení tabelátorů textového rámu...</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4017"/>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Možnost nastavit jiné rozměry obrázku než rámce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4018"/>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodorovné posunutí obrázku uvnitř rámce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4019"/>
        <source>Vertical offset of image within frame</source>
        <translation>Svislé posunutí obrázku uvnitř rámce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4020"/>
        <source>Resize the image horizontally</source>
        <translation>Změnit šířku obrázku</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4021"/>
        <source>Resize the image vertically</source>
        <translation>Změnit výšku obrázku</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4022"/>
        <source>Keep the X and Y scaling the same</source>
        <translation>Použít stejnou změnu velikosti pro oba rozměry (X a Y)</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4024"/>
        <source>Make the image fit within the size of the frame</source>
        <translation>Obrázek změní velikost podle rozměru rámce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4025"/>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Obrázek si zachová své proporce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3839"/>
        <source>Cell Lines</source>
        <translation>Čáry buňky v tabulce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3840"/>
        <source>Line at Top</source>
        <translation>Horní čára</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3841"/>
        <source>Line at the Left</source>
        <translation>Levá čára</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3842"/>
        <source>Line at the Right </source>
        <translation>Pravá čára</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3843"/>
        <source>Line at Bottom</source>
        <translation>Dolní čára</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4023"/>
        <source>Keep the aspect ratio</source>
        <translation>Dodržet poměr stran</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4026"/>
        <source>Source profile of the image</source>
        <translation>Zdrojový profil obrázku</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4027"/>
        <source>Rendering intent for the image</source>
        <translation>Účel reprodukce obrázku</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3771"/>
        <source>Path Text Properties</source>
        <translation>Vlastnosti textu na křivky</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3973"/>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Ukazuje hladinu zvoleného objektu. 0 znamená, že je objekt nejníž</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="4009"/>
        <source>Switches between Gap or Column width</source>
        <translation>Přepínat mezi mezisloupcovou mezerou a šířkou sloupce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="2510"/>
        <source>Column width</source>
        <translation>Šířka sloupce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3744"/>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3747"/>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3745"/>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3746"/>
        <source>&amp;Image</source>
        <translation>&amp;Obrázek</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3748"/>
        <source>&amp;Line</source>
        <translation>Čá&amp;ra</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3749"/>
        <source>&amp;Colors</source>
        <translation>&amp;Barvy</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3786"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3787"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3765"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3756"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3757"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3761"/>
        <source>&amp;Edit Shape...</source>
        <translation>Upravit &amp;obrys...</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3762"/>
        <source>R&amp;ound
Corners:</source>
        <translation>Zakolatit
&amp;rohy:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3764"/>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3765"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3766"/>
        <source>To&amp;p:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3767"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3768"/>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3769"/>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3770"/>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabelátory...</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3778"/>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Te&amp;xt obtéká okolo rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3779"/>
        <source>Use &amp;Bounding Box</source>
        <translation>Použít &amp;celostránkový rámec</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3780"/>
        <source>&amp;Use Contour Line</source>
        <translation>Použít &amp;obrysovou čáru</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3781"/>
        <source>St&amp;yle:</source>
        <translation>St&amp;yl:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3782"/>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3783"/>
        <source>&amp;Free Scaling</source>
        <translation>Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3788"/>
        <source>X-Sc&amp;ale:</source>
        <translation>X-Měří&amp;tko:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3789"/>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-Měřít&amp;ko:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3790"/>
        <source>Scale &amp;To Frame Size</source>
        <translation>Změni&amp;t velikost na rozměr rámu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3791"/>
        <source>P&amp;roportional</source>
        <translation>&amp;Proporcionálně</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3809"/>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Střed otáčení:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3810"/>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Typ čáry:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3822"/>
        <source>Line &amp;Width:</source>
        <translation>Šíř&amp;ka čáry:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3823"/>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Hrany:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3837"/>
        <source>&amp;Endings:</source>
        <translation>&amp;Ukončení:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="2717"/>
        <source>&amp;X1:</source>
        <translation>&amp;X1:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="2718"/>
        <source>X&amp;2:</source>
        <translation>X&amp;2:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="2719"/>
        <source>Y&amp;1:</source>
        <translation>Y&amp;1:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="2720"/>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3996"/>
        <source>Hyphenation language of frame</source>
        <translation>Jazyk dělení slov</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3991"/>
        <source>Right to Left Writing</source>
        <translation>Psaní zprava doleva</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3992"/>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3794"/>
        <source>Fixed Linespacing</source>
        <translation>Pevné řádkování</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3795"/>
        <source>Automatic Linespacing</source>
        <translation>Automatické řádkování</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3796"/>
        <source>Align to Baseline Grid</source>
        <translation>Zarovnat k pomocné mřížce</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3784"/>
        <source>Actual X-DPI:</source>
        <translation>Aktuální X-DPI:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3785"/>
        <source>Actual Y-DPI:</source>
        <translation>Aktuální Y-DPI:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3811"/>
        <source>Start Arrow:</source>
        <translation>Počáteční šipka:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3812"/>
        <source>End Arrow:</source>
        <translation>Koncová šipka:</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3984"/>
        <source>Offset to baseline of characters</source>
        <translation>Offset k účaří znaků</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3986"/>
        <source>Scaling height of characters</source>
        <translation>Měřená výška znaků</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3634"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Jméno &quot;%1&quot; není jedinečné.&lt;br/&gt;Zvolte prosím jiné.</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3775"/>
        <source>Fill Rule</source>
        <translation>Pravidla pro vyplňování</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3776"/>
        <source>Even-Odd</source>
        <translation>Lichý-sudý</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3777"/>
        <source>Non Zero</source>
        <translation>Nenulový</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3987"/>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Barva obrysu textu nebo stínu, podle výběru. Pokud jsou vybrána obě formátování, potom jsou stejnou barvou.</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3988"/>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Barva vybraného textu. Pokud je povoleno formátování textu Křivky, tato barva bude výplňová. Pokud je povoleno stínování textu, tato barva bude nejvýše.</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3977"/>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation>Nechat text ve vzdálenějších rámcích plavat kolem objektu. Možnosti níže chování upřesňují.</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3978"/>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Pro obtékání textových rámců pod objektem použijte místo obrysu rámce raději bounding box, který je vždy pravoúhlý. </translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3979"/>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Pro obtékání textových rámců pod objektem použijte druhou čáru založenou na obrysu rámce. </translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="1814"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3994"/>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Stiskněte a držte tlačítko pro výběr řádkování.</translation>
    </message>
    <message>
        <location filename="../mpalette.cpp" line="3976"/>
        <source>Enable or disable exporting of the object</source>
        <translation>Povolit nebo zakázat exportování objektu</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <location filename="../multiline.cpp" line="26"/>
        <source>Edit Style</source>
        <translation>Upravit styl</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="80"/>
        <source>Flat Cap</source>
        <translation>Ostrá hlavička</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="81"/>
        <source>Square Cap</source>
        <translation>Čtvercová hlavička</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="82"/>
        <source>Round Cap</source>
        <translation>Oblá hlavička</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="86"/>
        <source>Miter Join</source>
        <translation>Kolmý spoj</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="87"/>
        <source>Bevel Join</source>
        <translation>Zkosený spoj</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="88"/>
        <source>Round Join</source>
        <translation>Oblý spoj</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="94"/>
        <source>Line Width:</source>
        <translation>Šířka čáry:</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="261"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="112"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="295"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="280"/>
        <source>Solid Line</source>
        <translation>Plná čára</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="268"/>
        <source>Dashed Line</source>
        <translation>Čárkovaná čára</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="271"/>
        <source>Dotted Line</source>
        <translation>Tečkovaná čára</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="274"/>
        <source>Dash Dot Line</source>
        <translation>Čerchovaná čára</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="277"/>
        <source>Dash Dot Dot Line</source>
        <translation>Dvojitě čerchovaná čára</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="295"/>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Jméno %1 není jedinečné.&lt;br/&gt;Zvolte prosím jiné.</translation>
    </message>
    <message>
        <location filename="../multiline.cpp" line="181"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <location filename="../multiprogressdialogbase.ui" line="16"/>
        <source>Progress</source>
        <translation>Průběh</translation>
    </message>
    <message>
        <location filename="../multiprogressdialogbase.ui" line="43"/>
        <source>Overall Progress:</source>
        <translation>Celkový průběh:</translation>
    </message>
    <message>
        <location filename="../multiprogressdialogbase.ui" line="78"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <location filename="../plugins/myplugin/myplugin.cpp" line="31"/>
        <source>My &amp;Plugin</source>
        <translation>Můj &amp;Modul</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <location filename="../plugins/myplugin/mypluginimpl.cpp" line="23"/>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Můj modul</translation>
    </message>
    <message>
        <location filename="../plugins/myplugin/mypluginimpl.cpp" line="24"/>
        <source>The plugin worked!</source>
        <translation>Modul funguje!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <location filename="../newfile.cpp" line="45"/>
        <source>New Document</source>
        <translation>Nový dokument</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="122"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="39"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="141"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="142"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="172"/>
        <source>Margin Guides</source>
        <translation>Vodítka okrajů</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="200"/>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="82"/>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Velikost strany dokumentu - buď standardní, nebo volitelný rozměr</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="83"/>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientace stran dokumentu</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="84"/>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, je-li vybrán volitelný rozměr stránky</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="85"/>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Výška stran dokumentu - upravitelná, je-li vybrán volitelný rozměr stránky</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="90"/>
        <source>Default unit of measurement for document editing</source>
        <translation>Implicitní měrná jednotka dokumentu</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="91"/>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Vytvářet automaticky textové rámce, jsou-li přidány nové strany</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="92"/>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Počet sloupců v automaticky vytvořených textových rámcích</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="93"/>
        <source>Distance between automatically created columns</source>
        <translation>Vzdálenost mezi automaticky vytvořenými sloupci</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="129"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="138"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="150"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="157"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="221"/>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Implicitní jednotka:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="232"/>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Automatické textové rámce</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="245"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="247"/>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="68"/>
        <source>Do not show this dialog again</source>
        <translation>Tento dialog znovu nezobrazovat</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="89"/>
        <source>Initial number of pages of the document</source>
        <translation>Počáteční počet stran dokumentu</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="214"/>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Počet stran:</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="279"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="53"/>
        <source>&amp;New Document</source>
        <translation>&amp;Nový dokument</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="55"/>
        <source>Open &amp;Existing Document</source>
        <translation>Otevřít &amp;existující dokument</translation>
    </message>
    <message>
        <location filename="../newfile.cpp" line="57"/>
        <source>Open Recent &amp;Document</source>
        <translation>Otevřít ne&amp;dávný dokument</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftemplate.cpp" line="59"/>
        <source>New &amp;from Template...</source>
        <translation>N&amp;ový ze šablony...</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftemplate.cpp" line="78"/>
        <source>Load documents with predefined layout</source>
        <translation>Načíst dokumenty s předdefinovaným vzhledem</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftemplate.cpp" line="80"/>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Vytvořit dokument ze šablony připravené jinými uživateli nebo vámi (vhodné pro dokumenty s jednotným vzhledem).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <location filename="../frameedit.cpp" line="749"/>
        <source>Nodes</source>
        <translation>Uzly</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="760"/>
        <source>Move Nodes</source>
        <translation>Přesunout uzly</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="761"/>
        <source>Move Control Points</source>
        <translation>Přesunout řídicí body</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="762"/>
        <source>Add Nodes</source>
        <translation>Přidat uzly</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="763"/>
        <source>Delete Nodes</source>
        <translation>Smazat uzly</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="766"/>
        <source>Reset Control Points</source>
        <translation>Vynulovat řídicí body</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="767"/>
        <source>Reset this Control Point</source>
        <translation>Vynulovat tento řídicí bod</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="752"/>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutní souřadnice</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="755"/>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="756"/>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="757"/>
        <source>Edit &amp;Contour Line</source>
        <translation>Upravit &amp;obrysovou čáru</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="758"/>
        <source>&amp;Reset Contour Line</source>
        <translation>Vy&amp;nulovat konturu</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="759"/>
        <source>&amp;End Editing</source>
        <translation>&amp;Konec úprav</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="764"/>
        <source>Move Control Points Independently</source>
        <translation>Nezávisle přesouvat řídicí body</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="765"/>
        <source>Move Control Points Symmetrical</source>
        <translation>Symetricky přesouvat řídicí body</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="768"/>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Otevře polygon nebo ořeže Bézierovou křivku</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="769"/>
        <source>Close this Bezier Curve</source>
        <translation>Zavřít tuto Bźierovou křivku</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="770"/>
        <source>Mirror the Path Horizontally</source>
        <translation>Zrcadlit vodorovně</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="771"/>
        <source>Mirror the Path Vertically</source>
        <translation>Zrcadlit svisle</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="773"/>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Uvolnit horizontálu doleva</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="774"/>
        <source>Shear the Path Vertically Up</source>
        <translation>Uvolnit vertikálu nahoru</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="775"/>
        <source>Shear the Path Vertically Down</source>
        <translation>Uvolnit vertikálu dolů</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="776"/>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotace proti směru hodinových ručiček</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="777"/>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotace po směru hodinových ručiček</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="779"/>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Zvětšit o uvedené %</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="782"/>
        <source>Angle of Rotation</source>
        <translation>Úhel rotace</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="785"/>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivovat úpravy obrysové čáry</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="786"/>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Nastavit obrysovou čáru na původní obrys rámu</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="772"/>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Uvolnit horizontálu doprava</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="750"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="787"/>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Je-li zaškrtnuto, používají se souřadnice relativní ke stránce, jinak jsou relativní k objektu.</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="778"/>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Zmenšit o uvedené %</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="780"/>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Zmenšit o zobrazenou hodnotu</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="781"/>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Zvětšit o zobrazenou hodnotu</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="783"/>
        <source>% to Enlarge or Shrink By</source>
        <translation>% zvětšení či zmenšení</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="784"/>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Hodnota zvětšení či zmenšení</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="753"/>
        <source>to Canvas</source>
        <translation>na plochu</translation>
    </message>
    <message>
        <location filename="../frameedit.cpp" line="754"/>
        <source>to Page</source>
        <translation>na stránku</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="275"/>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Tento dokument nevypadá jako OpenOffice Draw soubor.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="90"/>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Import OpenOffice.org &amp;Draw...</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="105"/>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Import souborů OpenOffice Draw</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="106"/>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importuje do aktuálního dokumentu soubory OpenOffice.org Draw a převede vektory na objekty Scribusu.</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="120"/>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenDocument 1.0 Draw</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="132"/>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenOffice.org 1.x Draw</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="188"/>
        <source>This file contains some unsupported features</source>
        <translation>Tento soubor obsahuje nějaké nepodporované vlastnosti</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="60"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použít jméno dokumentu jako předponu stylů odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="67"/>
        <source>Do not ask again</source>
        <translation>Neptat se znovu</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="77"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="39"/>
        <source>OpenDocument Importer Options</source>
        <translation>Vlastnosti importu OpenDocument</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="48"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Přepsat existující styly novými</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="53"/>
        <source>Merge Paragraph Styles</source>
        <translation>Sloučit styly odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="55"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sloučí styly podle jejich vlastností. Výsledkem bude několik stylů se specifickými vlastnostmi, přestože původní dokument obsahoval styly pojmenované jinak.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="62"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Přidat název dokumentu do jména stylu.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="70"/>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Nastavit tyto vlastnosti jako implicitní a neptat se na ně při každém importu OASIS OpenDocument formátu.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="45"/>
        <source>Overwrite Paragraph Styles</source>
        <translation>Přepsat styly odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtdia.cpp" line="79"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <location filename="../plugins/fileloader/oldscribusformat/oldscribusformat.cpp" line="59"/>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oldscribusformat/oldscribusformat.cpp" line="75"/>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokument</translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <location filename="../styleselect.cpp" line="117"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="118"/>
        <source>Linewidth</source>
        <translation>Šířka čáry</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <location filename="../pdfopts.cpp" line="46"/>
        <source>Save as PDF</source>
        <translation>Uložit jako PDF</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="52"/>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Výstup do souboru:</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="73"/>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="76"/>
        <source>Output one file for eac&amp;h page</source>
        <translation>&amp;Každá strana svůj soubor</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="89"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="149"/>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="149"/>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF soubory (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="101"/>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Umožňuje exportovat individuálně pojmenované PDF soubory pro každou stránku v dokumentu. Čísla stránek jsou přidána automaticky. Vhodné zejména pro efektní komerční tisky.</translation>
    </message>
    <message>
        <location filename="../pdfopts.cpp" line="102"/>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Tlačítko pro uložení bude zakázané, pokud se pokusíte exportovat PDF/X-3 a bude chybět informační řetězec z karty PDF/X-3.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <location filename="../pdflib.cpp" line="117"/>
        <source>Saving PDF</source>
        <translation>Ukládám PDF</translation>
    </message>
    <message>
        <location filename="../pdflib.cpp" line="121"/>
        <source>Exporting Master Pages:</source>
        <translation>Export vzorových stran:</translation>
    </message>
    <message>
        <location filename="../pdflib.cpp" line="121"/>
        <source>Exporting Pages:</source>
        <translation>Export stran:</translation>
    </message>
    <message>
        <location filename="../pdflib.cpp" line="121"/>
        <source>Exporting Items on Current Page:</source>
        <translation>Export objektů aktuální strany:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <location filename="../preview.cpp" line="61"/>
        <source>Print Preview</source>
        <translation>Ukázka před tiskem</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="491"/>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="275"/>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Zobrazí práhlednost a průhledné objekty dokumentu. Vyžaduje Ghostscript 7.07 nebo novější</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="276"/>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Vytvoří náhled tisku simulací CMYK inkoustů místo RGB barev</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="290"/>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Povolí/zakáže C (Cyan) složku</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="291"/>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Povolí/zakáže M (Magenta) složku</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="292"/>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Povolí/zakáže Y (Yellow) složku</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="293"/>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Povolí/zakáže B (Black) složku</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="101"/>
        <source>Display Trans&amp;parency</source>
        <translation>Zobrait &amp;průhlednost</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="125"/>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Zobrazit CMYK</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="136"/>
        <source>&amp;C</source>
        <translation>&amp;C</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="141"/>
        <source>&amp;M</source>
        <translation>&amp;M</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="146"/>
        <source>&amp;Y</source>
        <translation>&amp;Y</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="151"/>
        <source>&amp;K</source>
        <translation>&amp;K</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="112"/>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Under Color Removal</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="180"/>
        <source>Separation Name</source>
        <translation>Jméno separace</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="192"/>
        <source>Cyan</source>
        <translation>Azurová (Cyan)</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="198"/>
        <source>Magenta</source>
        <translation>Purpurová (Magenta)</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="204"/>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="210"/>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="235"/>
        <source>Scaling:</source>
        <translation>Zvětšení:</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="251"/>
        <source>Print...</source>
        <translation>Tisk...</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="277"/>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Způsob, jak odstranit některé odstíny šedé, které jsou tvořeny tyrkysovou, žlutou a fialovou, a použít místo nich černou. UCR ovlivní části obrázku, které jsou neutrální a/nebo obsahují tmavé tóny blízké šedé. Můžete tak vylepšit tisk některých obrázků, je ale nutné to vyzkoušet v praxi a trochu experimentovat. UCR snižuje riziko přesycení v případě CMY inkoustů.</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="278"/>
        <source>Resize the scale of the page.</source>
        <translation>Změnit poměr stránky.</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="248"/>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="827"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="96"/>
        <source>Enable &amp;Antialiasing</source>
        <translation>Povolit &amp;vyhlazování</translation>
    </message>
    <message>
        <location filename="../preview.cpp" line="274"/>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Umožňuje hezčí náhled fontů Type 1, TrueType, OpenType, EPS, PDF a vektorové grafiky, ovšem za cenu zpomalení</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <location filename="../pslib.cpp" line="1014"/>
        <source>Processing Master Pages:</source>
        <translation>Zpracování vzorových stran:</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1014"/>
        <source>Exporting Pages:</source>
        <translation>Export stran:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <location filename="../pageitem.cpp" line="372"/>
        <source>Image</source>
        <translation>Obrázek</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="376"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="380"/>
        <source>Line</source>
        <translation>Čára</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="384"/>
        <source>Polygon</source>
        <translation>Mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="388"/>
        <source>Polyline</source>
        <translation>Lomená čára</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="392"/>
        <source>PathText</source>
        <translation>Text na křivky</translation>
    </message>
    <message>
        <location filename="../pageitem.cpp" line="3069"/>
        <source>Copy of</source>
        <translation>Kopie</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <location filename="../pageitemattributes.cpp" line="17"/>
        <source>Relates To</source>
        <translation>Vztahuje se k</translation>
    </message>
    <message>
        <location filename="../pageitemattributes.cpp" line="17"/>
        <source>Is Parent Of</source>
        <translation>Je rodičem</translation>
    </message>
    <message>
        <location filename="../pageitemattributes.cpp" line="17"/>
        <source>Is Child Of</source>
        <translation>Je dítětem</translation>
    </message>
    <message>
        <location filename="../pageitemattributes.cpp" line="17"/>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <location filename="../pageitemattributesbase.ui" line="16"/>
        <source>Page Item Attributes</source>
        <translation>Atributy objektu</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="36"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="41"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="46"/>
        <source>Value</source>
        <translation>Hodnota</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="51"/>
        <source>Parameter</source>
        <translation>Parametr</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="56"/>
        <source>Relationship</source>
        <translation>Vztah</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="61"/>
        <source>Relationship To</source>
        <translation>Vztah k</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="87"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="90"/>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="98"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="101"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="109"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="112"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="120"/>
        <source>C&amp;lear</source>
        <translation>&amp;Vymazat</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="123"/>
        <source>Alt+L</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="148"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="../pageitemattributesbase.ui" line="165"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <location filename="../pagelayout.cpp" line="259"/>
        <source>First Page is:</source>
        <translation>První strana je:</translation>
    </message>
    <message>
        <location filename="../pagelayout.cpp" line="188"/>
        <source>Document Layout</source>
        <translation>Vzhled dokumentu</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <location filename="../seiten.cpp" line="502"/>
        <source>Double sided</source>
        <translation>Dvojstrany</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="510"/>
        <source>Middle Right</source>
        <translation>Střední pravá</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="541"/>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Přetáhnutím stran nebo vzorových stran na koš je smažete</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="542"/>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Všechny vaše vzorové strany. Přetažením vzorové strany na plochu vytvoříte stranu novou</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="657"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="799"/>
        <source>Arrange Pages</source>
        <translation>Uspořádat strany</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="800"/>
        <source>Available Master Pages:</source>
        <translation>Dostupné vzorové strany:</translation>
    </message>
    <message>
        <location filename="../seiten.cpp" line="801"/>
        <source>Document Pages:</source>
        <translation>Strany dokumentu:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <location filename="../pageselector.cpp" line="247"/>
        <source>%1 of %2</source>
        <translation>%1 of %2</translation>
    </message>
    <message>
        <location filename="../pageselector.cpp" line="210"/>
        <source>%1 of %1</source>
        <translation>%1 of %1</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Quarto</source>
        <translation>Quarto</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Foolscap</source>
        <translation>Kancelářský papír</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Letter</source>
        <translation>Letter</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Government Letter</source>
        <translation>Government Letter</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Ledger</source>
        <translation>Ledger</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Executive</source>
        <translation>Executive</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Post</source>
        <translation>Post</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Crown</source>
        <translation>Crown</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Large Post</source>
        <translation>Large Post</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Demy</source>
        <translation>Demy</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Medium</source>
        <translation>Medium</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Royal</source>
        <translation>Royal</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Elephant</source>
        <translation>Elephant</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Double Demy</source>
        <translation>Double Demy</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>Quad Demy</source>
        <translation>Quad Demy</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>STMT</source>
        <translation>STMT</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>A</source>
        <translation>A</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>B</source>
        <translation>B</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>D</source>
        <translation>D</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="229"/>
        <source>E</source>
        <translation>E</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <location filename="../picsearch.cpp" line="17"/>
        <source>Result</source>
        <translation>Výsledek</translation>
    </message>
    <message>
        <location filename="../picsearch.cpp" line="31"/>
        <source>Search Results for: </source>
        <translation>Hledat výsledek pro:</translation>
    </message>
    <message>
        <location filename="../picsearch.cpp" line="56"/>
        <source>Preview</source>
        <translation>Náhled</translation>
    </message>
    <message>
        <location filename="../picsearch.cpp" line="64"/>
        <source>Select</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <location filename="../picsearch.cpp" line="71"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <location filename="../picstatus.cpp" line="79"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="79"/>
        <source>Path</source>
        <translation>Cesta</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="79"/>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="79"/>
        <source>Print</source>
        <translation>Tisk</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="79"/>
        <source>Status</source>
        <translation>Stav</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="208"/>
        <source>Goto</source>
        <translation>Jít na</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="152"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="204"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="205"/>
        <source>Missing</source>
        <translation>Chybí</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="206"/>
        <source>Search</source>
        <translation>Hledat</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="207"/>
        <source>Cancel Search</source>
        <translation>Zrušit hledání</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="64"/>
        <source>Manage Pictures</source>
        <translation>Správa obrázků</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="282"/>
        <source>Scribus - Image Search</source>
        <translation>Hledání obrázků</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="265"/>
        <source>The search failed: %1</source>
        <translation>Chyba hledání: %1</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="283"/>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>6ádný soubor jménem &quot;%1&quot; nenalezen.</translation>
    </message>
    <message>
        <location filename="../picstatus.cpp" line="236"/>
        <source>Select a base directory for search</source>
        <translation>Zvolit základní adresář</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="58"/>
        <source>Save as &amp;Image...</source>
        <translation>Uložit jako &amp;obrázek...</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="76"/>
        <source>Export As Image</source>
        <translation>Exportovat jako obrázek</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="77"/>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exportuje vybrané strany jako bitmapové obrázky.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <location filename="../pluginmanager.cpp" line="70"/>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Nelze najít modul</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="59"/>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>neznámá chyba</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="95"/>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Nelze najít symbol (%1)</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="171"/>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Modul: nahrávám %1</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="201"/>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>chyba inicializace</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="207"/>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>neznámý typ modulu</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="213"/>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 načten</translation>
    </message>
    <message>
        <location filename="../pluginmanager.cpp" line="217"/>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Modul: %1 chyba nahrávání: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="24"/>
        <source>Plugin Manager</source>
        <translation>Správce modulů</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="34"/>
        <source>Plugin</source>
        <translation>Modul</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="36"/>
        <source>How to run</source>
        <translation>Jak spustit</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="38"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="40"/>
        <source>Load it?</source>
        <translation>Nahrát?</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="42"/>
        <source>Plugin ID</source>
        <translation>ID modulu</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="44"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="109"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="102"/>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="../pluginmanagerprefsgui.cpp" line="80"/>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Aby se změny projevily, musíte znovu spustit program.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <location filename="../polyprops.cpp" line="16"/>
        <source>Polygon Properties</source>
        <translation>Vlastnosti mnohoúhelníků</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <location filename="../polygonwidget.cpp" line="32"/>
        <source>Corn&amp;ers:</source>
        <translation>Ro&amp;hy:</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="45"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="60"/>
        <source>Apply &amp;Factor</source>
        <translation>Použít &amp;faktor</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="69"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="73"/>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="102"/>
        <source>Number of corners for polygons</source>
        <translation>Počet rohů mnohoúhelníků</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="104"/>
        <source>Degrees of rotation for polygons</source>
        <translation>Stupně rotace mnohoúhelníků</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="105"/>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplikovat konvexnost/konkávnost na mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="106"/>
        <source>Sample Polygon</source>
        <translation>Ukázkový mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../polygonwidget.cpp" line="108"/>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Záporná hodnota nastaví mnohoúhelník jako konkávní (nebo s hranami do hvězdy), kladná konvexní</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <location filename="../prefs.cpp" line="75"/>
        <source>Preferences</source>
        <translation>Nastavení</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="199"/>
        <source>General</source>
        <translation>Všeobecné</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="331"/>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="334"/>
        <source>Guides</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="336"/>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="338"/>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="576"/>
        <source>Display</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="84"/>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="142"/>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="216"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="69"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="242"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="243"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="285"/>
        <source>Margin Guides</source>
        <translation>Vodítka okrajů</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="292"/>
        <source>Autosave</source>
        <translation>Automatické uložení</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="303"/>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="124"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="864"/>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="633"/>
        <source>External Tools</source>
        <translation>Externí nástroje</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="615"/>
        <source>Image Processing Tool</source>
        <translation>Nástroj na úpravu obrázků</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="712"/>
        <source>Default font size for the menus and windows</source>
        <translation>Velikost písma v menu a oknech</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="714"/>
        <source>Default unit of measurement for document editing</source>
        <translation>Implicitní měrná jednotka dokumentu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="715"/>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Počet řádek, o které Scribus posune text při pohybu kolečka myši</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="716"/>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Počet současně otevřených dokumentů, které jsou vidět v Soubor menu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="717"/>
        <source>Default documents directory</source>
        <translation>Implicitní adresář na dokumenty</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="719"/>
        <source>Default Scripter scripts directory</source>
        <translation>Implicitní adresář na skripty</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="722"/>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Implicitní velikost strany, standardní nebo vlastní rozměr</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="723"/>
        <source>Default orientation of document pages</source>
        <translation>Implicitní orientace stran dokumentu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="724"/>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="725"/>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="729"/>
        <source>Time period between saving automatically</source>
        <translation>Časový interval automatického ukládání</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="735"/>
        <source>Color for paper</source>
        <translation>Barva papíru</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="736"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplnit plochu za hranicemi strany barvou okrajů</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="737"/>
        <source>Set the default zoom level</source>
        <translation>Nastavení implicitní úrovně zvětšení</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="741"/>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat text při vykreslování EPS a PDF na monitoru</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="742"/>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat grafiku při vykreslování EPS a PDF na monitoru</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="112"/>
        <source>&amp;Theme:</source>
        <translation>&amp;Téma:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="132"/>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Skok kolečka myši:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="137"/>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nedávné dokumenty:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="154"/>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenty:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="626"/>
        <source>&amp;Change...</source>
        <translation>Z&amp;měnit...</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="167"/>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC profily:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="170"/>
        <source>C&amp;hange...</source>
        <translation>Změn&amp;it...</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="180"/>
        <source>&amp;Scripts:</source>
        <translation>S&amp;kripty:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="183"/>
        <source>Ch&amp;ange...</source>
        <translation>Změni&amp;t...</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="237"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="246"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="265"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="273"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="499"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="485"/>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="511"/>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="492"/>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="305"/>
        <source>&amp;Interval:</source>
        <translation>&amp;Interval:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="442"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="546"/>
        <source>&amp;Adjust Display Size</source>
        <translation>Přizpůsobení &amp;velikosti obrazovky</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="590"/>
        <source>&amp;Name of Executable:</source>
        <translation>Jmé&amp;no spustitelného souboru (programu):</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="598"/>
        <source>Antialias &amp;Text</source>
        <translation>Vyhlazený &amp;text</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="601"/>
        <source>Antialias &amp;Graphics</source>
        <translation>Vyhlazená &amp;grafika</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="623"/>
        <source>Name of &amp;Executable:</source>
        <translation>Jméno &amp;spustitelného souboru (programu):</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="194"/>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="101"/>
        <source>&amp;Language:</source>
        <translation>&amp;Jazyk:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="191"/>
        <source>Document T&amp;emplates:</source>
        <translation>Šablony &amp;dokumentů:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="253"/>
        <source>Units:</source>
        <translation>Jednotky:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="310"/>
        <source>Undo/Redo</source>
        <translation>Undo/Redo</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="324"/>
        <source>Action history length</source>
        <translation>Délka historie akcí</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="346"/>
        <source>Hyphenator</source>
        <translation>Dělení slov</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="349"/>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="352"/>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="357"/>
        <source>Color Management</source>
        <translation>Správa barev</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="371"/>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="376"/>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položek dokumentu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="382"/>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstříky</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="386"/>
        <source>Keyboard Shortcuts</source>
        <translation>Klávesové zkratky</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="417"/>
        <source>Page Display</source>
        <translation>Zobrazení strany</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="425"/>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="443"/>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="447"/>
        <source>Show Pictures</source>
        <translation>Zobrazit obrázky</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="451"/>
        <source>Show Text Chains</source>
        <translation>Zobrazit řetězení textu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="455"/>
        <source>Show Text Control Characters</source>
        <translation>Zobrazit řídicí znaky textu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="459"/>
        <source>Show Frames</source>
        <translation>Zobrazit rámy</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="463"/>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke straně</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="469"/>
        <source>Scratch Space</source>
        <translation>Pracovní prostor</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="516"/>
        <source>Gaps between Pages</source>
        <translation>Mezera mezi stranami</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="533"/>
        <source>Horizontal:</source>
        <translation>Vodorovná:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="541"/>
        <source>Vertical:</source>
        <translation>Svislá:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="552"/>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Obrazovku přizpůsobíte posunem jezdce na spodním pravítku.</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="608"/>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="610"/>
        <source>Resolution:</source>
        <translation>Rozlišení:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="651"/>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Vždy se ptát před nahrazením písem při načítání dokumentu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="654"/>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Při úpravě stylů je náhled na aktuální styl odstavce viditelný</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="657"/>
        <source>Show Startup Dialog</source>
        <translation>Zobrazovat uvítací dialog</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="661"/>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="667"/>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Vždy použít klasické Lorem Ipsum</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="670"/>
        <source>Count of the Paragraphs:</source>
        <translation>Počet odstavců:</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="682"/>
        <source>Miscellaneous</source>
        <translation>Různé</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="686"/>
        <source>Plugins</source>
        <translation>Moduly</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="707"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobrazit netisknutelné znaky, např. značky pro odstavec v textových rámcích</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="708"/>
        <source>Turns the display of frames on or off</source>
        <translation>Přepíná zobrazení rámců</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="709"/>
        <source>Turns the display of pictures on or off</source>
        <translation>Přepíná zobrazení obrázků</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="720"/>
        <source>Additional directory for document templates</source>
        <translation>Doplňkový adresář pro šablony dokumentů</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="738"/>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Umístěte na obrazovku pravítko a posuňte jezdce, abyste nastavili požadované přiblížení. Scribus pak zobrazí stránky a jejich objekty ve správné velikosti</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="755"/>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa vlevo od dokumentu, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="756"/>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa vpravo od dokumentu, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="757"/>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa nad dokumentem, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="758"/>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Určuje velikost místa pod dokumentem, kam lze vkládat a kde lze vytvářet prvky a přesunovat je na aktivní stránku</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="872"/>
        <source>Locate Ghostscript</source>
        <translation>Umístění Ghostscriptu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="880"/>
        <source>Locate your image editor</source>
        <translation>Umístění editoru obrázků</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="581"/>
        <source>PostScript Interpreter</source>
        <translation>Interpret PostScriptu</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="706"/>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Povolit nebo zakázat zobrazení propojených rámců.</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="710"/>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Vyberte jazyk, ve kterém se má Scribus spustit. Pokud ho nezvolíte, zvolí se na základě proměnných prostředí. Stále jej však budete moci změnit při spouštění Scribusu zadáním parametru na příkazové řádce</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="119"/>
        <source>&amp;Font Size (Menus):</source>
        <translation>&amp;Velikost písma (nabídky):</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="126"/>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Velikost &amp;písma (palety):</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="711"/>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Implicitní dekorace oken a vzhled. Scribus přejímá dostupná témata KDE nebo Qt, pokud je Qt nastaveno pro vyhledávání pluginů KDE.</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="713"/>
        <source>Default font size for the tool windows</source>
        <translation>Implicitní velikost písma pro okna nástrojů</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="718"/>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Defaultní adresář ICC profilů. Nelze měnit, pokud je dokument otevřený. Normálně Scribus hledá v systémových adresářích (Mac OS X, Windows). V Linuxu a Unixu se prohledává adresář $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="728"/>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Pokud je povoleno, Scribus uloží záložní kopii souboru s příponou .bak pokaždé, když uplyne zadaný čas</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="731"/>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Délka historie jednotlivých akcí po krocích. Pokud se rovná nule, ukládá se neomezené množství akcí.</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="746"/>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Cesta ke grafickému editoru. Pokud používáte GIMP a váš systém ho obsahuje, doporučujeme použít &apos;gimp-remote&apos;, protože se obrázek načte v instanci, která je už spuštěná.</translation>
    </message>
    <message>
        <location filename="../prefs.cpp" line="740"/>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation>Cesta pro interpreter Ghostscript. Nezapoměňte, že ve Windows je nutné použít program gswin32c.exe, NE gswin32.exe, což by mohlo vést k zamrznutí Scribusu.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <location filename="../prefsdialogbase.cpp" line="136"/>
        <source>&amp;Defaults</source>
        <translation>&amp;Výchozí</translation>
    </message>
    <message>
        <location filename="../prefsdialogbase.cpp" line="135"/>
        <source>Save...</source>
        <translation>Uložit...</translation>
    </message>
    <message>
        <location filename="../prefsdialogbase.cpp" line="146"/>
        <source>Save Preferences</source>
        <translation>Uložit nastavení</translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <location filename="../prefsmanager.cpp" line="1680"/>
        <source>Postscript</source>
        <translation>Postsript</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="593"/>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Převést stará nastavení Scribusu?</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="595"/>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus nalezl soubory s nastavením pro Scribus 1.2.
Chcete je převést na novou verzi Scribusu?</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1680"/>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1345"/>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Nelze otevřít soubor s nastavením &quot;%1&quot; pro zápis: %2</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1357"/>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Zápis do souboru s nastavením &quot;%1&quot; selhal:  chybový kód QIODevice %2</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1371"/>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Nelze otevřít soubor s nastavením &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1381"/>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Nelze číst XML nastavení z &quot;%1&quot;: %2, řádek %3, sloupec %4</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF 1.3</source>
        <translation type="obsolete">PDF 1.3</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF 1.4</source>
        <translation type="obsolete">PDF 1.4</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1952"/>
        <source>Error Writing Preferences</source>
        <translation>Chyba zápisu nastavení</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1958"/>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Nelze uložit nastavení: &lt;br&gt;%1&lt;br&gt; Zkontrolujte prosím oprávnění k souboru a adresáři a také volné místo dostupné na disku.</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1971"/>
        <source>Error Loading Preferences</source>
        <translation>Chyba při načítání nastavení</translation>
    </message>
    <message>
        <location filename="../prefsmanager.cpp" line="1976"/>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Nelze načíst nastavení:&lt;br&gt;%1&lt;br&gt;Použije se implicitní nastavení.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <location filename="scribus/marginwidget.cpp" line="317"/>
        <source>Magazine</source>
        <translation>Časopis</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="318"/>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="319"/>
        <source>Golden Mean</source>
        <translation>Zlatý řez</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="320"/>
        <source>Nine Parts</source>
        <translation>Nine Parts</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="316"/>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="323"/>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Můžete si vybrat předdefinovaný vzhled strany. &apos;Žádný&apos; ponechá okraje, jak jsou, &apos;Gutenbreg&apos; nastaví okraje klasicky, &apos;časopis&apos; nastaví okraje na stejnou hodnotu. Nejdůležitější je hodnota vlevo/uvnitř.</translation>
    </message>
    <message>
        <location filename="scribus/marginwidget.cpp" line="315"/>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Žádný</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="49"/>
        <source>&amp;Open...</source>
        <translation>&amp;Otevřít...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="50"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="51"/>
        <source>Save &amp;As...</source>
        <translation>Uložit j&amp;ako...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="53"/>
        <source>&amp;Exit</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="54"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="56"/>
        <source>&amp;Run</source>
        <translation>Spustit sk&amp;ript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="57"/>
        <source>Run As &amp;Console</source>
        <translation>Spustit jako &amp;konzole</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="58"/>
        <source>&amp;Save Output...</source>
        <translation>Uložit &amp;výstup...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="59"/>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="97"/>
        <source>Scribus Python Console</source>
        <translation>Python konzole</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="103"/>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Odvozeno z běžné konzole Pythonu, takže obsahuje jistá omezení, např. co se mezer týče. Více najdete v manuálu Scribusu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="128"/>
        <source>Script Console</source>
        <translation>Konzole skriptů</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="129"/>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Sem napište příkazy. Výběr je zpracován jako skript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="130"/>
        <source>Output of your script</source>
        <translation>Výstup vašeho skriptu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="206"/>
        <source>Python Scripts (*.py)</source>
        <translation>Python skripty (*.py);</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="209"/>
        <source>Save the Python Commands in File</source>
        <translation>Uložit příkazy Pythonu do souboru</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="234"/>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="225"/>
        <source>Text Files (*.txt)</source>
        <translation>Textové soubory (*.txt)</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="228"/>
        <source>Save Current Output</source>
        <translation>Uložit aktuální výstup</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/pconsole.cpp" line="174"/>
        <source>Open Python Script File</source>
        <translation>Otevřít soubor se skriptem v Pythonu</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <location filename="../translationdummy.cpp" line="130"/>
        <source>Hu&amp;e:</source>
        <translation>&amp;Odstín:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="131"/>
        <source>&amp;Sat:</source>
        <translation>&amp;Sytost:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="132"/>
        <source>&amp;Val:</source>
        <translation>&amp;Hod:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="133"/>
        <source>&amp;Red:</source>
        <translation>Če&amp;rvená:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="134"/>
        <source>&amp;Green:</source>
        <translation>Ze&amp;lená:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="135"/>
        <source>Bl&amp;ue:</source>
        <translation>Mo&amp;drá:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="136"/>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Alfa kanál:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="137"/>
        <source>&amp;Basic colors</source>
        <translation>&amp;Základní barvy</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="138"/>
        <source>&amp;Custom colors</source>
        <translation>&amp;Vlastní barvy</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="139"/>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definovat vlastní barvy &gt;&gt;</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="140"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="141"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="142"/>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Přidat k vlastním barvám</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="143"/>
        <source>Select color</source>
        <translation>Výběr barvy</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <location filename="../translationdummy.cpp" line="14"/>
        <source>Copy or Move a File</source>
        <translation>Kopírovat nebo přesunout soubor</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="15"/>
        <source>Read: %1</source>
        <translation>Číst: %1</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="16"/>
        <source>Write: %1</source>
        <translation>Zapsat: %1</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="17"/>
        <source>File &amp;name:</source>
        <translation>Jmé&amp;no souboru:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="18"/>
        <source>File &amp;type:</source>
        <translation>&amp;Typ souboru:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="19"/>
        <source>One directory up</source>
        <translation>O adresář výš</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="20"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="21"/>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="22"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="23"/>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="24"/>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="25"/>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="26"/>
        <source>Attributes</source>
        <translation>Atributy</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="27"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="28"/>
        <source>Look &amp;in:</source>
        <translation>Nahlédn&amp;i do:</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="29"/>
        <source>Back</source>
        <translation>Zpět</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="30"/>
        <source>Create New Folder</source>
        <translation>Vytvořit nový adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="31"/>
        <source>List View</source>
        <translation>Pohled seznam</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="32"/>
        <source>Detail View</source>
        <translation>Podrobný pohled</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="33"/>
        <source>Preview File Info</source>
        <translation>Zobrazit informace o souboru</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="34"/>
        <source>Preview File Contents</source>
        <translation>Ukázat obsah souboru</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="35"/>
        <source>Read-write</source>
        <translation>Číst-psát</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="36"/>
        <source>Read-only</source>
        <translation>Jen číst</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="37"/>
        <source>Write-only</source>
        <translation>Jen psát</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="38"/>
        <source>Inaccessible</source>
        <translation>Nepřístupný</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="39"/>
        <source>Symlink to File</source>
        <translation>Symbolický odkaz na soubor</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="40"/>
        <source>Symlink to Directory</source>
        <translation>Symbolický odkaz na adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="41"/>
        <source>Symlink to Special</source>
        <translation>Symbolický odkaz na speciální</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="42"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="43"/>
        <source>Dir</source>
        <translation>Adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="44"/>
        <source>Special</source>
        <translation>Speciální</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="45"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="46"/>
        <source>Save As</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="47"/>
        <source>&amp;Open</source>
        <translation>&amp;Otevřít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="48"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="49"/>
        <source>&amp;Rename</source>
        <translation>&amp;Přejmenovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="50"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="51"/>
        <source>R&amp;eload</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="52"/>
        <source>Sort by &amp;Name</source>
        <translation>Seřadit podle &amp;jména</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="53"/>
        <source>Sort by &amp;Size</source>
        <translation>Seřadit podle &amp;velikosti</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="54"/>
        <source>Sort by &amp;Date</source>
        <translation>Seřadit podle &amp;data</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="55"/>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nesetříděno</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="56"/>
        <source>Sort</source>
        <translation>Řazení</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="57"/>
        <source>Show &amp;hidden files</source>
        <translation>Zobrazit s&amp;kryté soubory</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="58"/>
        <source>the file</source>
        <translation>soubor</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="59"/>
        <source>the directory</source>
        <translation>adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="60"/>
        <source>the symlink</source>
        <translation>symbolický odkaz</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="61"/>
        <source>Delete %1</source>
        <translation>Smazat %1</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="62"/>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Opravdu chcete smazat %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="63"/>
        <source>&amp;Yes</source>
        <translation>&amp;Ano</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="64"/>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="65"/>
        <source>New Folder 1</source>
        <translation>Nový adresář 1</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="66"/>
        <source>New Folder</source>
        <translation>Nový adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="67"/>
        <source>New Folder %1</source>
        <translation>Nový adresář %1</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="68"/>
        <source>Find Directory</source>
        <translation>Najít adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="69"/>
        <source>Directories</source>
        <translation>Adresáře</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="70"/>
        <source>Save</source>
        <translation>Uložit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="71"/>
        <source>Error</source>
        <translation>Chyba</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="72"/>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1(new line)
Soubor nenalezen.(new line)
Zkontrolujte cestu a název souboru.</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="73"/>
        <source>All Files (*.*)</source>
        <translation>Včechny soubory (*.*)</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="74"/>
        <source>Select a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="75"/>
        <source>Directory:</source>
        <translation>Adresář:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <location filename="../translationdummy.cpp" line="147"/>
        <source>&amp;Font</source>
        <translation>&amp;Písmo</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="148"/>
        <source>Font st&amp;yle</source>
        <translation>S&amp;tyl písma</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="149"/>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="150"/>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="151"/>
        <source>Stri&amp;keout</source>
        <translation>Přešk&amp;rtnuté</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="152"/>
        <source>&amp;Underline</source>
        <translation>Po&amp;dtržené</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="153"/>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="154"/>
        <source>Sample</source>
        <translation>Příklad</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="155"/>
        <source>Scr&amp;ipt</source>
        <translation>Skr&amp;ipt</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="156"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="157"/>
        <source>Apply</source>
        <translation>Použít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="158"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="159"/>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="160"/>
        <source>Select Font</source>
        <translation>Vybrat písmo</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <location filename="../translationdummy.cpp" line="79"/>
        <source>Clear</source>
        <translation>Vymazat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="80"/>
        <source>Select All</source>
        <translation>Vybrat vše</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="81"/>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="82"/>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="83"/>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="84"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="85"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <location filename="../translationdummy.cpp" line="99"/>
        <source>Line up</source>
        <translation>Vyrovnat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="100"/>
        <source>Customize...</source>
        <translation>Upravit...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <location filename="../translationdummy.cpp" line="9"/>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;O Qt&lt;/h3&gt;&lt;p&gt;Tento program používá Qt verze %1.&lt;/p&gt;&lt;p&gt;Qt je C++ knihovna určená pro vývoj multiplatformních nejen GUI aplikací.&lt;/p&gt;&lt;p&gt;Qt zajišťuje přenositelnost jediného zdrojového kódu na MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, a všechny hlavní varianty komerčních Unixů.&lt;br&gt;Qt je také k dispozici pro jednoúčelová zařízení (embedded devices).&lt;/p&gt;&lt;p&gt;Qt je produktem firmy Trolltech. Více informací získáte na &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="142"/>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="143"/>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Opravdu chcete přepsat soubor:
%1 ?</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="368"/>
        <source>Initializing...</source>
        <translation>Inicializace...</translation>
    </message>
    <message>
        <location filename="../fileloader.cpp" line="1869"/>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="152"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../scwinprint.cpp" line="108"/>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="150"/>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG obrázky (*.svg *.svgz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="152"/>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG obrázky (*.svg);;Všechny obrázky (*)</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="144"/>
        <source>Yes</source>
        <translation>Ano</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="145"/>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="206"/>
        <source>Save as Image</source>
        <translation>Uložit jako obrázek</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="207"/>
        <source>Error writing the output file(s).</source>
        <translation>Chyba při zápisu souboru.</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="127"/>
        <source>Export successful.</source>
        <translation>Úspěšný export.</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="189"/>
        <source>File exists. Overwrite?</source>
        <translation>Soubor existuje. Přepsat?</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="190"/>
        <source>exists already. Overwrite?</source>
        <translation>už existuje. Přepsat?</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="194"/>
        <source>Yes all</source>
        <translation>Ano všem</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="216"/>
        <source>Newsletters</source>
        <translation>Věstníky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="217"/>
        <source>Brochures</source>
        <translation>Brožury</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="218"/>
        <source>Catalogs</source>
        <translation>Katalogy</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="219"/>
        <source>Flyers</source>
        <translation>Letáky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="220"/>
        <source>Signs</source>
        <translation>Pokyny</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="221"/>
        <source>Cards</source>
        <translation>Karty</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="222"/>
        <source>Letterheads</source>
        <translation>Dopisy</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="223"/>
        <source>Envelopes</source>
        <translation>Obálky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="224"/>
        <source>Business Cards</source>
        <translation>Vizitky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="225"/>
        <source>Calendars</source>
        <translation>Kalendáře</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="226"/>
        <source>Advertisements</source>
        <translation>Inzeráty</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="227"/>
        <source>Labels</source>
        <translation>Štítky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="228"/>
        <source>Menus</source>
        <translation>Nabídky (Menu)</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="229"/>
        <source>Programs</source>
        <translation>Programy</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="230"/>
        <source>PDF Forms</source>
        <translation>PDF formuláře</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="231"/>
        <source>PDF Presentations</source>
        <translation>PDF prezentace</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="232"/>
        <source>Magazines</source>
        <translation>Časopisy</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="233"/>
        <source>Posters</source>
        <translation>Plakáty</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="234"/>
        <source>Announcements</source>
        <translation>Oznámení</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="235"/>
        <source>Text Documents</source>
        <translation>Textové dokumenty</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="236"/>
        <source>Folds</source>
        <translation>Složky</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satemplate.cpp" line="257"/>
        <source>Own Templates</source>
        <translation>Vlastní šablony</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="136"/>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Všechny podporované formáty (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="137"/>
        <source>All Files (*)</source>
        <translation>Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="552"/>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus skripty</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="547"/>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Vykonat skript...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="553"/>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Použité skripty</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="548"/>
        <source>Show &amp;Console</source>
        <translation>Ukázat &amp;konzoli</translation>
    </message>
    <message>
        <location filename="../gtaction.cpp" line="73"/>
        <source>Importing text</source>
        <translation>Importovat text</translation>
    </message>
    <message>
        <location filename="../gtgettext.cpp" line="98"/>
        <source>All Supported Formats</source>
        <translation>Všechny podporované formáty</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/htmlim/htmlim.cpp" line="40"/>
        <source>HTML Files</source>
        <translation>HTML soubory</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/htmlim/htmlim.cpp" line="45"/>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/txtim/txtim.cpp" line="20"/>
        <source>Text Files</source>
        <translation>Textové soubory</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvim.cpp" line="12"/>
        <source>Comma Separated Value Files</source>
        <translation>Soubory CSV (čárkou oddělěné hodnoty)</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvim.cpp" line="58"/>
        <source>CSV_data</source>
        <translation>CSV_data</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/csvim/csvim.cpp" line="62"/>
        <source>CSV_header</source>
        <translation>CSV_hlavička</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="810"/>
        <source>Font %1 is broken, discarding it</source>
        <translation>Písmo %1 je poškozené. Bude vyřazeno</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/htmlim/htmlreader.cpp" line="619"/>
        <source>
External Links
</source>
        <translation>
Odkazy ven
</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/textfilter.cpp" line="25"/>
        <source>Text Filters</source>
        <translation>Textové filtry</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="237"/>
        <source>Media Cases</source>
        <translation>Obaly médií</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="46"/>
        <source>Albanian</source>
        <translation>Albánština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="47"/>
        <source>Basque</source>
        <translation>Baskitština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="48"/>
        <source>Bulgarian</source>
        <translation>Bulharština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="50"/>
        <source>Catalan</source>
        <translation>Katalánština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="51"/>
        <source>Chinese</source>
        <translation>Čínština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="55"/>
        <source>Czech</source>
        <translation>Čeština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="57"/>
        <source>Danish</source>
        <translation>Dánština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="58"/>
        <source>Dutch</source>
        <translation>Holandština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="172"/>
        <source>English</source>
        <translation>Angličtina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="62"/>
        <source>English (British)</source>
        <translation>Angličtina (Britská)</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="63"/>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="65"/>
        <source>German</source>
        <translation>Němčina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="68"/>
        <source>Finnish</source>
        <translation>Finština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="69"/>
        <source>French</source>
        <translation>Francouzština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="72"/>
        <source>Galician</source>
        <translation>Galština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="73"/>
        <source>Greek</source>
        <translation>Řečtina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="74"/>
        <source>Hungarian</source>
        <translation>Maďarština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="75"/>
        <source>Indonesian</source>
        <translation>Indonéský</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="77"/>
        <source>Italian</source>
        <translation>Italština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="79"/>
        <source>Korean</source>
        <translation>Korejština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="81"/>
        <source>Lithuanian</source>
        <translation>Litevština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="84"/>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norština (Bokmaal)</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="86"/>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norština (Nnyorsk)</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="88"/>
        <source>Norwegian</source>
        <translation>Norština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="90"/>
        <source>Polish</source>
        <translation>Polština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="93"/>
        <source>Russian</source>
        <translation>Ruština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="100"/>
        <source>Swedish</source>
        <translation>Švédština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="95"/>
        <source>Spanish</source>
        <translation>Španělština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="96"/>
        <source>Spanish (Latin)</source>
        <translation>Španělština (Latin)</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="97"/>
        <source>Slovak</source>
        <translation>Slovenština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="98"/>
        <source>Slovenian</source>
        <translation>Slovinština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="99"/>
        <source>Serbian</source>
        <translation>Srbština</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="549"/>
        <source>&amp;About Script...</source>
        <translation>&amp;O skriptu...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="520"/>
        <source>About Script</source>
        <translation>O skriptu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="25"/>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat velikost písma z netextového rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="51"/>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat písmo z netextového rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="77"/>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat velikost textu z netextového rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="95"/>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat počet sloupců z netextového rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="113"/>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat řádkování z netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="131"/>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat odstup sloupců z netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="181"/>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat text z netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="257"/>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="347"/>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="414"/>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Hodnota zarovnání je mimo povolený rozsah. Použijte jednu z předdefinovaných konstant scribus.ALIGN*.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="607"/>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Pozice výběru mimo povolené hranice</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="850"/>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt není zřetězený textový rámec, proto nelze řetězení zrušit.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="855"/>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektu na posledním místě nelze zrušit zřetězení. Použijte místo něj předchozí rámec.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.cpp" line="211"/>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Jednotka mimo povolený rozsah. Použijte jednu z předdefinovanýcj konstant scribus.UNIT*.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="25"/>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Cíl není obrázkový rámec.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.cpp" line="236"/>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Úhel rohu nesmí být negativní číslo.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="60"/>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze získat barvu s prázdným jménem.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="82"/>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze změnit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="117"/>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze vytvořit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="152"/>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze smazat barvu s prázdným jménem.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="196"/>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze nahradit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwim.cpp" line="43"/>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.o Writer dokumenty</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetprop.cpp" line="42"/>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Barva nenalezena - Python chyba</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/configuration.cpp" line="126"/>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Volitelná uživatelská konfigurace:</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/configuration.cpp" line="128"/>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardní konfigurace:</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="119"/>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky. Čekejte prosím...</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="138"/>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky. Hotovo.</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="44"/>
        <source>Afrikaans</source>
        <translation>Afrikánština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="104"/>
        <source>Turkish</source>
        <translation>Turečtina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="105"/>
        <source>Ukranian</source>
        <translation>Ukrajinština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="106"/>
        <source>Welsh</source>
        <translation>Welština</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="73"/>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Jméno souboru musí být řetězec znaků.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="96"/>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Nelze smazat nastavení typu obrázku.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="100"/>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Typ obrázku musí být řetězec.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="125"/>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Atribut allTypes je pouze ke čtení</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="184"/>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Chyba při exportu obrázku</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="205"/>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="166"/>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena v dokumentu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdcolor.cpp" line="177"/>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Barva nenalezena v implicitních barvách.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="262"/>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Nelze změnit velikost o 0%.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="371"/>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Specifikovaný objekt není rám obrázku.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="500"/>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Písmo nenalezeno.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="100"/>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Nelze vykreslit prázdný náhled.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="360"/>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Nelze zadat prázdné jméno vrstvy.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="399"/>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Vrstva nenalezena.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="365"/>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Nelze odstranit poslední vrstvu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="415"/>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Nelze vytvořit vrstvu beze jména.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="354"/>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Index mimo povolené hranice.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="422"/>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit zarovnání textu netextovému rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="447"/>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Velikost písma je mimo povolený interval - &lt;1, 512&gt;.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="456"/>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit velikost písma netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="484"/>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit písmo netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="517"/>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Velikost řádkování mimo hranice, musí být větší než 0.1.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="525"/>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit hodnotu řádkování netextovému rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="543"/>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Velikost mezisloupcové mezery mimo povolený rozsah. Musí být kladné číslo.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="551"/>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit mezisloupcovou mezeru netextovému rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="569"/>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Počet sloupců mimo povolený rozsah. Musí být &gt; 1.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="577"/>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit počet sloupců netertovému rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="612"/>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Nelze vybrat text v netextovém rámu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="649"/>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze smazat text z netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="688"/>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nasatvit výplň textu netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="722"/>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit textový obrys u netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="761"/>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit stín písma netextového rámu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="799"/>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Řetězit lze pouye textové rámy.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="804"/>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Cílový rámec musí být prázdný.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="809"/>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je zřetězený.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="814"/>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je zřetězený.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="819"/>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Zdroj a cíl jsou stejný objekt.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="844"/>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze zrušit zřetězení netextového rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="896"/>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Netextový rám nelze převést na křivky.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="960"/>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rám nemůže být poznámkou</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="992"/>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rám neobsahuje požadované informace</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/odtim/odtim.cpp" line="44"/>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument dokumenty (OpenOffice 2)</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="53"/>
        <source>Croatian</source>
        <translation>Chorvatsky</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="91"/>
        <source>Portuguese</source>
        <translation>Portugalsky</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="92"/>
        <source>Portuguese (BR)</source>
        <translation>Portugalština (Braz.)</translation>
    </message>
    <message>
        <location filename="../main_win32.cpp" line="249"/>
        <source>Scribus Crash</source>
        <translation>Zhroucení Scribusu</translation>
    </message>
    <message>
        <location filename="../main_nix.cpp" line="118"/>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus spadl díky signálu č. %1</translation>
    </message>
    <message>
        <location filename="../main_win32.cpp" line="251"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="80"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../page.cpp" line="78"/>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <location filename="../page.cpp" line="207"/>
        <source>Master Page </source>
        <translation>Vzorová strana </translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="154"/>
        <source>4A0</source>
        <translation>4A0</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="160"/>
        <source>2A0</source>
        <translation>2A0</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="250"/>
        <source>Comm10E</source>
        <translation>Comm10E</translation>
    </message>
    <message>
        <location filename="../pagesize.cpp" line="256"/>
        <source>DLE</source>
        <translation>DLE</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="37"/>
        <source>Could not open output file %1</source>
        <translation>Nelze otevřít výstupní soubor %1</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="53"/>
        <source>Output stream not writeable</source>
        <translation>Do výstupního proudu nelze zapisovat</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="76"/>
        <source>Verification of settings failed: %1</source>
        <translation>Selhala kontrola nastavení: %1</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="276"/>
        <source>Could not open input file %1</source>
        <translation>Nelze otevřít vstupní soubor %1</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="654"/>
        <source>Unable to read settings XML:</source>
        <translation>Nelze načíst nastavení XML:</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="292"/>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (řádek %2, sloupec %3)

Načíst nastavení PDF
</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="311"/>
        <source>Unable to read settings XML: %1</source>
        <translation>Nelze načíst nastavení XML:. %1</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="311"/>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>nulový kořenový uzel

Načíst nastavení PDF</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="432"/>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; neplatné

Načíst nastavení PDF</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="445"/>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>nalezeno %1 &lt;%2&gt; uzlů, potřeba 1.</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="460"/>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>neočekávaný nulový &lt;%2&gt; uzel</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="468"/>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>uzel &lt;%1&gt; není element</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="488"/>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; neobsahuje vlastnost `value&apos;</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="517"/>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>hodnota elementu &lt;%1&gt; musí být `true&apos; nebo `false&apos;</translation>
    </message>
    <message>
        <location filename="../pdfoptionsio.cpp" line="655"/>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;lpiSettingsEntry&gt; neobsahuje vlastnost `name&apos;</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="153"/>
        <source>Freetype2 library not available</source>
        <translation>Knihovna Freetype2 není k dispozici</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="157"/>
        <source>Font %1 is broken, no embedding</source>
        <translation>Font %1 je poškozený, nebude vložen</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="285"/>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Font %1 je poškozený (chyba při čtení), nebude vložen</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="385"/>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Font %1 je poškozený (FreeType2), ignoruje se</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="63"/>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Font %1 je poškozený (není Face), ignoruje se</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="873"/>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Font %1 má poškozený glyf %2 (znak %3)</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="447"/>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Font %1 je poškozený a bude ignorován</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="538"/>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Font %1 nelze načíst, nebude vložen</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="817"/>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Nelze načíst font %1 - neznámý typ fontu</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1025"/>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Font %1 načten z %2(%3)</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1041"/>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Font %1(%2) je duplicitou %3</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1097"/>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Načítám font %1 (nalezen pomocí fontconfigu)</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1102"/>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Nelze načíst font FreeType2 - nelze nalézt soubor s fontem</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="56"/>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Font %1 je poškozený (FreeType), bude ignorován</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="91"/>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Font %1 obsahuje neplatný glyf %2 (znak %3), bude ignorován</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="170"/>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>rozbaluji face %1 z fontu %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="183"/>
        <source>memcpy header: %1 %2 %3</source>
        <translation>hlavička memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="192"/>
        <source>table &apos;%1&apos;</source>
        <translation>tabulka &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="193"/>
        <source>memcpy table: %1 %2 %3</source>
        <translation>tabulka memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <location filename="../scfonts_ttf.cpp" line="196"/>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="603"/>
        <source>Scribus Development Version</source>
        <translation>Vývojová verze Scribus</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> mm</source>
        <translation> mm</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> in</source>
        <translation> in</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> p</source>
        <translation> p</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> cm</source>
        <translation> cm</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="122"/>
        <source> c</source>
        <translation> c</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>in</source>
        <translation>in</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>p</source>
        <translation>p</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="133"/>
        <source>c</source>
        <translation>c</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="177"/>
        <source>Points (pt)</source>
        <translation>Body (pt)</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="178"/>
        <source>Millimeters (mm)</source>
        <translation>Milimetry (mm)</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="179"/>
        <source>Inches (in)</source>
        <translation>Palce (in)</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="180"/>
        <source>Picas (p)</source>
        <translation>Pika (p)</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="181"/>
        <source>Centimeters (cm)</source>
        <translation>Centimetry (cm)</translation>
    </message>
    <message>
        <location filename="../units.cpp" line="182"/>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <location filename="../util.cpp" line="651"/>
        <source>File exists</source>
        <translation>Soubor existuje</translation>
    </message>
    <message>
        <location filename="../util.cpp" line="653"/>
        <source>&amp;Replace</source>
        <translation>&amp;Nahradit</translation>
    </message>
    <message>
        <location filename="../util.cpp" line="1225"/>
        <source>page</source>
        <comment>page export</comment>
        <translation>strana</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1416"/>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftemplate.cpp" line="115"/>
        <source>Document Template: </source>
        <translation>Šablona dokumentu: </translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.cpp" line="150"/>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Dokument nelze otevřít.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.cpp" line="176"/>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Dokument nelze uložit.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="38"/>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argumentem musí být název objektu stránky, nebo instance PyCObject</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="275"/>
        <source>Property not found</source>
        <translation>Vlastnost nenalezena</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="216"/>
        <source>Child not found</source>
        <translation>Potomek nenalezen</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="321"/>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Nelze konvertovat výsledek typu &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="453"/>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Vlastnost typu &apos;%1&apos; není podporována</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="473"/>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Nelze konvertovat &apos;%1&apos; na vlastnost typu &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdgetsetprop.cpp" line="482"/>
        <source>Types matched, but setting property failed.</source>
        <translation>Typy odpovídají, ale nastavení vlastnosti selhalo.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="197"/>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Nelze seskupit méně než dvě položky</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="222"/>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Nelze seskupit méně než dvě položky</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmani.cpp" line="230"/>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>K seskupení je nutný výběr nebo seznam argumentů pro položky</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdmisc.cpp" line="134"/>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Nelze uložit pixmapu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="414"/>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Objekt s požadovaným jménem již existuje.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="172"/>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň dva body (čtyři hodnoty).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="252"/>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat sudý počet hodnot.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="247"/>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň tři body (šest hodnot).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="324"/>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat alespoň čtyři body (osm hodnot).</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="329"/>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Seznam bodů musí obsahovat násobky šesti hodnot.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="423"/>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekt nenalezen.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="531"/>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Styl nenalezen.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdobj.cpp" line="558"/>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit styl netextovému rámci.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="39"/>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Nelze uložit EPS.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="102"/>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Číslo stránky mimo rozsah.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="241"/>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument není v seznamu: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="203"/>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nečíselné hodnoty: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="252"/>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument obsahuje nečíselné hodnoty: musí být seznam plovoucích hodnot.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.cpp" line="125"/>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Šířka čáry mimo rozsah, musí být 0 &lt;= line_width &lt;= 12.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.cpp" line="146"/>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Odstín čáry mimo rozsah, musí být 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.cpp" line="167"/>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Odstín výplně mimo rozsah, musí být 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdsetprop.cpp" line="264"/>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Styl čáry nenalezen.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdtext.cpp" line="920"/>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Jen textové rámce mohou být testována na přetečení</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/objimageexport.cpp" line="78"/>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Název souboru nesmí být prázdný řetězec.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="551"/>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptplugin.cpp" line="702"/>
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
        <location filename="../fileloader.cpp" line="548"/>
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1178"/>
        <source>Black</source>
        <translation>Černá (Black)</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1180"/>
        <source>Cyan</source>
        <translation>Azurová (Cyan)</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1182"/>
        <source>Magenta</source>
        <translation>Purpurová (Magenta)</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1184"/>
        <source>Yellow</source>
        <translation>Žlutá (Yellow)</translation>
    </message>
    <message>
        <location filename="../plugins/colorwheel/colorwheel.cpp" line="58"/>
        <source>Color Wheel</source>
        <translation>Kruhový výběr barev</translation>
    </message>
    <message>
        <location filename="../plugins/fontpreview/fontpreviewplugin.cpp" line="57"/>
        <source>Font Preview</source>
        <translation>Náhled písem</translation>
    </message>
    <message>
        <location filename="../plugins/myplugin/myplugin.cpp" line="45"/>
        <source>My Plugin</source>
        <translation>Můj modul</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftemplate.cpp" line="70"/>
        <source>New From Template</source>
        <translation>Nový ze šablony</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="68"/>
        <source>Export As Image</source>
        <translation>Exportovat jako obrázek</translation>
    </message>
    <message>
        <location filename="../plugins/psimport/importpsplugin.cpp" line="66"/>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS Import</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satemplate.cpp" line="63"/>
        <source>Save As Template</source>
        <translation>Uložit jako šablonu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptplugin.cpp" line="129"/>
        <source>Scripter</source>
        <translation>Skripter</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="81"/>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="89"/>
        <source>SVG Export</source>
        <translation>SVG Export</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="88"/>
        <source>SVG Import</source>
        <translation>SVG Import</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="98"/>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Import OpenOffice.org Draw</translation>
    </message>
    <message>
        <location filename="../main_win32.cpp" line="226"/>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus se zhroutil kvůli následující výjimce: %1</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="801"/>
        <source>Creating Font Cache</source>
        <translation>Vytvářím cache písem</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="827"/>
        <source>New Font found, checking...</source>
        <translation>Nalezena nová písma, ověřuji...</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="864"/>
        <source>Modified Font found, checking...</source>
        <translation>Nalezena změněná písma, ověřuji...</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1200"/>
        <source>Reading Font Cache</source>
        <translation>Načítám cache písem</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1247"/>
        <source>Writing updated Font Cache</source>
        <translation>Zapisuji aktualizovanou cache písem</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="1264"/>
        <source>Searching for Fonts</source>
        <translation>Hledám písma</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="604"/>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Používáte vývojovou verzi Scribusu 1.3.x. Dokument, se kterým pracujete, byl vytvořen ve Scribusu 1.2.3 nebo starším. Pokud jej teď uložíte, v těchto starších verzích jej nebude možné načíst, pokud ovšem nepoužijete Soubor-Uložit jako. Opravdu chcete pokračovat?</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3821"/>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Změny ve vašem dokumentu nebyly uloženy, ovšem vy požadujete jejich vrácení. Opravdu chcete pokračovat?</translation>
    </message>
    <message>
        <location filename="../util.cpp" line="652"/>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Soubor jménem &apos;%1&apos; již existuje.&lt;br/&gt;Chcete jej nahradit souborem, který právě ukládáte?</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmddoc.cpp" line="44"/>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder je větší, než je dovoleno.</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oldscribusformat/oldscribusformat.cpp" line="40"/>
        <source>Old .sla format support</source>
        <translation>Podpora starého formátu .sla</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="67"/>
        <source>German (Trad.)</source>
        <translation>Německy</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1004"/>
        <source>Exporting PostScript File</source>
        <translation>Export PostScript souboru</translation>
    </message>
    <message>
        <location filename="../pslib.cpp" line="1006"/>
        <source>Printing File</source>
        <translation>Tisk souboru</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3137"/>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Snažíte se importovat více stránek, než je počet stránek dostupných v aktuálním dokumentu, počítáno od aktivní stránky.&lt;/p&gt;Vyberte některou možnost:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Vytvořit&lt;/b&gt; chybějící stránky&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importovat&lt;/b&gt; stránky do poslední stránky&lt;/li&gt;&lt;b&gt;Zrušit&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3138"/>
        <source>C&amp;reate</source>
        <translation>&amp;Vytvořit</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3139"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="102"/>
        <source>Thai</source>
        <translation>thajština</translation>
    </message>
    <message>
        <location filename="../plugins/barcodegenerator/barcode.cpp" line="29"/>
        <source>Barcode Generator</source>
        <translation>Generátor čárových kódů</translation>
    </message>
    <message>
        <location filename="../plugins/fileloader/oodraw/oodrawimp.cpp" line="166"/>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/docim/docim.cpp" line="56"/>
        <source>Word Documents</source>
        <translation>Word dokumenty</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/pdbim/pdbim.cpp" line="34"/>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB dokumenty</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/pdbim/pdbim.cpp" line="77"/>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_data</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/pdbim/pdbim.cpp" line="100"/>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Import PDB</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/pdbim/pdbim.cpp" line="92"/>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Nelze otevřít soubor %1</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="82"/>
        <source>Luxembourgish</source>
        <translation>lucemburština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="45"/>
        <source>Arabic</source>
        <translation>arabština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="64"/>
        <source>Estonian</source>
        <translation>estonština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="78"/>
        <source>Japanese</source>
        <translation>japonština</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/cmdpage.cpp" line="92"/>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Zadané jméno vzorové strany nesouhlasí s žádnou existující.</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="76"/>
        <source>Icelandic</source>
        <translation>islandština</translation>
    </message>
    <message>
        <location filename="../scimage.cpp" line="3725"/>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 může být poškozený: chybí značky pro rozlišení</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/pdbim/pdbim.cpp" line="101"/>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Tento soubor není PDB dokument. Pokud si myslíte, že je, nahlaste nám, prosím, chybu.</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="49"/>
        <source>Breton</source>
        <translation>bretonština</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="60"/>
        <source>English (American)</source>
        <translation>americká angličtina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="61"/>
        <source>English (Australian)</source>
        <translation>australská angličtina</translation>
    </message>
    <message>
        <location filename="../scimage.cpp" line="4180"/>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation>%1 může být poškozený: chybějící nebo poškozené značky</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="66"/>
        <source>German (Swiss)</source>
        <translation>švýcarská němčina</translation>
    </message>
    <message>
        <location filename="../langmgr.cpp" line="52"/>
        <source>Chinese (Trad.)</source>
        <translation>čínština (trad.)</translation>
    </message>
    <message>
        <location filename="../plugins/pixmapexport/export.cpp" line="175"/>
        <source>Insufficient memory for this image size.</source>
        <translation>Nedostatek paměti pro tak velký obrázek.</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="348"/>
        <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
        <translation>Písmo %1 má poškozenou metriku v souboru %2, metriku ignoruji</translation>
    </message>
    <message>
        <location filename="../scfonts.cpp" line="354"/>
        <source>Valid metrics were found for font %1, using metrics in file %2</source>
        <translation>Nalezeny platné metriky pro písmo %1, používám metriku ze souboru %2</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <location filename="../translationdummy.cpp" line="89"/>
        <source>Clear</source>
        <translation>Vymazat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="90"/>
        <source>Select All</source>
        <translation>Vybrat vše</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="91"/>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="92"/>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="93"/>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="94"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="95"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <location filename="../translationdummy.cpp" line="104"/>
        <source>System Menu</source>
        <translation>Systémová nabídka</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="105"/>
        <source>Shade</source>
        <translation>Schovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="106"/>
        <source>Unshade</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="107"/>
        <source>Normalize</source>
        <translation>Normalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="108"/>
        <source>Minimize</source>
        <translation>Minimalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="109"/>
        <source>Maximize</source>
        <translation>Maximalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="110"/>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <location filename="../translationdummy.cpp" line="114"/>
        <source>&amp;Restore</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="115"/>
        <source>&amp;Move</source>
        <translation>&amp;Přesunout</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="116"/>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="117"/>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="118"/>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="119"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="120"/>
        <source>Stay on &amp;Top</source>
        <translation>Zůs&amp;tat navrchu</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="121"/>
        <source>Minimize</source>
        <translation>Minimalizovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="122"/>
        <source>Restore Down</source>
        <translation>Obnovit původní</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="123"/>
        <source>Close</source>
        <translation>Zavřít</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="124"/>
        <source>Sh&amp;ade</source>
        <translation>&amp;Schovat</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="125"/>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <location filename="../translationdummy.cpp" line="126"/>
        <source>&amp;Unshade</source>
        <translation>Z&amp;obrazit</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <location filename="../reformdoc.cpp" line="82"/>
        <source>Document Setup</source>
        <translation>Nastavení dokumentu</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="175"/>
        <source>Margin Guides</source>
        <translation>Vodítka okrajů</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="316"/>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="324"/>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="332"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="340"/>
        <source>&amp;Right:</source>
        <translation>&amp;Pravý:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="100"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="79"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="133"/>
        <source>Portrait</source>
        <translation>Na výšku</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="134"/>
        <source>Landscape</source>
        <translation>Na šířku</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="113"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="132"/>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientace:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="140"/>
        <source>&amp;Width:</source>
        <translation>Šíř&amp;ka:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="151"/>
        <source>&amp;Height:</source>
        <translation>&amp;Výška:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="163"/>
        <source>&amp;Unit:</source>
        <translation>&amp;Jednotka:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="219"/>
        <source>Autosave</source>
        <translation>Automatické uložení</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="230"/>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="232"/>
        <source>&amp;Interval:</source>
        <translation>&amp;Interval:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="236"/>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="239"/>
        <source>Document Information</source>
        <translation>Informace o dokumentu</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="242"/>
        <source>Guides</source>
        <translation>Vodítka</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="247"/>
        <source>Page Display</source>
        <translation>Zobrazení strany</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="255"/>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="272"/>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="273"/>
        <source>Alt+U</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="277"/>
        <source>Show Pictures</source>
        <translation>Zobrazit obrázky</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="281"/>
        <source>Show Text Chains</source>
        <translation>Zobrazit řetězení textu</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="285"/>
        <source>Show Text Control Characters</source>
        <translation>Zobrazit řídicí znaky textu</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="289"/>
        <source>Show Frames</source>
        <translation>Zobrazit rámy</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="293"/>
        <source>Rulers relative to Page</source>
        <translation>Pravítka relativně ke straně</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="299"/>
        <source>Minimum Scratch Space</source>
        <translation>Minimální pracovní prostor</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="346"/>
        <source>Gaps between Pages</source>
        <translation>Mezera mezi stranami</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="363"/>
        <source>Horizontal:</source>
        <translation>Vodorovná:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="371"/>
        <source>Vertical:</source>
        <translation>Svislá:</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="375"/>
        <source>Display</source>
        <translation>Zobrazit</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="378"/>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="381"/>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="389"/>
        <source>Hyphenator</source>
        <translation>Dělení slov</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="392"/>
        <source>Fonts</source>
        <translation>Písma</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="395"/>
        <source>Preflight Verifier</source>
        <translation>Předtisková kontrola</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="400"/>
        <source>PDF Export</source>
        <translation>PDF export</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="405"/>
        <source>Document Item Attributes</source>
        <translation>Vlastnosti položky v dokumentu</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="411"/>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstříky</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="421"/>
        <source>Color Management</source>
        <translation>Správa barev</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="429"/>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Zobrazit v textových rámcích netisknutelné znaky jako např. znaky konce odstavců</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="430"/>
        <source>Turns the display of frames on or off</source>
        <translation>Vypne nebo zapne zobrazení rámců</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="431"/>
        <source>Turns the display of pictures on or off</source>
        <translation>Vypne nebo zapne zobrazení obrázků</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="432"/>
        <source>Color for paper</source>
        <translation>Barva papíru</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="433"/>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplnit plochu za hranicemi strany barvou okrajů</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="968"/>
        <source>Adjusting Colors</source>
        <translation>Přizpůsobení barev</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="428"/>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Povolí nebo zakáže zobrazení propojených textových rámců.</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="168"/>
        <source>Apply size settings to all pages</source>
        <translation>Použít nastavení velikosti pro všechny strany</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="415"/>
        <source>Sections</source>
        <translation>Sekce</translation>
    </message>
    <message>
        <location filename="../reformdoc.cpp" line="436"/>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Použít změny ve velikosti stránky na všechny existující stránky dokumentu</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <location filename="../plugins/scriptplugin/runscriptdialog.cpp" line="18"/>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python skripty (*.py);; All Files (*)</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/runscriptdialog.cpp" line="22"/>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Spustit jako Python rozšíření</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <location filename="../stylemanagerbase.ui" line="25"/>
        <source>Style Manager</source>
        <translation>Správce stylů</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="156"/>
        <source>Column 1</source>
        <translation>Sloupec 1</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="229"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="232"/>
        <source>Alt+A</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="257"/>
        <source>C&amp;lone</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="260"/>
        <source>Alt+L</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="302"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="305"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="333"/>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="407"/>
        <source>O&amp;K</source>
        <translation>O&amp;K</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="410"/>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="438"/>
        <source>A&amp;pply</source>
        <translation>&amp;Použít</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="441"/>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="463"/>
        <source>Ca&amp;ncel</source>
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <location filename="../stylemanagerbase.ui" line="466"/>
        <source>Alt+N</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <location filename="../smlinestyle.cpp" line="195"/>
        <source>Properties</source>
        <translation>Vlastnosti</translation>
    </message>
    <message>
        <location filename="../smlinestyle.cpp" line="202"/>
        <source>Lines</source>
        <translation>Čáry</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <location filename="../story.cpp" line="1890"/>
        <source>Style of current paragraph</source>
        <translation>Styl aktuálního odstavce</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1877"/>
        <source>Style Settings</source>
        <translation>Nastavení stylu</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <location filename="../story.cpp" line="1665"/>
        <source>Color of text fill</source>
        <translation>Barva výplně písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1666"/>
        <source>Saturation of color of text fill</source>
        <translation>Sytost barvy výplně písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1646"/>
        <source>Fill Color Settings</source>
        <translation>Nastavení výplně</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <location filename="../story.cpp" line="1725"/>
        <source>Color of text stroke</source>
        <translation>Barva obrysu písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1726"/>
        <source>Saturation of color of text stroke</source>
        <translation>Sytost barvy obrysu písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1706"/>
        <source>Stroke Color Settings</source>
        <translation>Nastavení barvy obrysu</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <location filename="../story.cpp" line="1925"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1935"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1949"/>
        <source>Font of selected text</source>
        <translation>Písmo vybraného textu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1950"/>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1951"/>
        <source>Scaling width of characters</source>
        <translation>Změna šířky znaků písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1919"/>
        <source>Font Settings</source>
        <translation>Nastavení písma</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1952"/>
        <source>Scaling height of characters</source>
        <translation>Upravuji výšku znaků</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <location filename="../story.cpp" line="1763"/>
        <source>Character Settings</source>
        <translation>Nastavení znaků</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1789"/>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="1771"/>
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="81"/>
        <source>Save Page as &amp;SVG...</source>
        <translation>Uložit stranu jako &amp;SVG...</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="96"/>
        <source>Exports SVG Files</source>
        <translation>Export SVG souborů</translation>
    </message>
    <message>
        <location filename="../plugins/svgexplugin/svgexplugin.cpp" line="97"/>
        <source>Exports the current page into an SVG file.</source>
        <translation>Export aktuální strany do SVG souboru.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="80"/>
        <source>Import &amp;SVG...</source>
        <translation>Import &amp;SVG...</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="95"/>
        <source>Imports SVG Files</source>
        <translation>Import SVG souborů</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="96"/>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importuje většinu SVG souborů do aktuálního dokumentu,
přičemž konvertuje vektorová data na objekty Scribusu.</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="110"/>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <location filename="../plugins/svgimplugin/svgplugin.cpp" line="177"/>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG obsahuje některé nepodporované vlastnosti</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="141"/>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="142"/>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Aplikovat nezlomitelnou mezeru na:</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="143"/>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Vybrané rámce</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="144"/>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Aktuální strana</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="145"/>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>Všechny &amp;objekty</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="153"/>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Upraveny pouze vybrané rámce.</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="154"/>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Upravena pouze aktuální strana.</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/vlnadialog.cpp" line="155"/>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Upraven celý dokument.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="55"/>
        <source>User settings</source>
        <translation>Uživatelské nastavení</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="60"/>
        <source>System wide configuration</source>
        <translation>Systémová konfigurace</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="79"/>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="80"/>
        <source>&amp;Reset</source>
        <translation>&amp;Obnovit</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="82"/>
        <source>Save user configuration</source>
        <translation>Uložit uživatelskou konfiguraci</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="83"/>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Načíst systémové nastavení a odstranit uživatelské</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="84"/>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Upravit vlastní nastavení. Pokud je uložíte, má přednost před systémovým nastavením</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="108"/>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="99"/>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Uživatelské nastavení již existuje. Opravdu je chcete přepsat?</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="109"/>
        <source>Cannot write file %1.</source>
        <translation>Nelze uložit soubor %1.</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="116"/>
        <source>User settings saved</source>
        <translation>Uživatelské nastavení uloženo</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="127"/>
        <source>System wide configuration reloaded</source>
        <translation>Systémové nastavení načteno</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/swprefsgui.cpp" line="141"/>
        <source>Cannot open file %1</source>
        <translation>Nelze otevřít soubor %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <location filename="../plugins/saveastemplateplugin/satemplate.cpp" line="52"/>
        <source>Save as &amp;Template...</source>
        <translation>Uložit jako ša&amp;blonu...</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satemplate.cpp" line="71"/>
        <source>Save a document as a template</source>
        <translation>Uložit dokument jako šablonu</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satemplate.cpp" line="73"/>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Uložit dokument jako šablonu. Dobrá cesta, jak si usnadnit práci s dokumenty, které mají mít jednotný vzhled</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <location filename="../scplugin.cpp" line="49"/>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Trvalý</translation>
    </message>
    <message>
        <location filename="../scplugin.cpp" line="51"/>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Akce</translation>
    </message>
    <message>
        <location filename="../scplugin.cpp" line="47"/>
        <source>Load/Save/Import/Export</source>
        <translation>Načíst/Uložit/Import/Export</translation>
    </message>
    <message>
        <location filename="../scplugin.cpp" line="55"/>
        <source>Unknown</source>
        <translation>Neznámý</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <location filename="../sctoolbar.cpp" line="262"/>
        <source>Top</source>
        <translation>Nahoře</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="263"/>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="264"/>
        <source>Bottom</source>
        <translation>Dole</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="265"/>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="266"/>
        <source>Allow Docking To...</source>
        <translation>Umožnit dokování...</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="274"/>
        <source>Horizontal</source>
        <translation>Vodorovně</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="275"/>
        <source>Vertical</source>
        <translation>Svisle</translation>
    </message>
    <message>
        <location filename="../sctoolbar.cpp" line="276"/>
        <source>Floating Orientation...</source>
        <translation>Prostorová orientace...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <location filename="../scwinprint.cpp" line="315"/>
        <source>Printing...</source>
        <translation>Tisknu...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <location filename="../scribusXml.cpp" line="970"/>
        <source>Copy #%1 of </source>
        <translation>Kopie č. %1 z</translation>
    </message>
    <message>
        <location filename="../scribusXml.cpp" line="1801"/>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <location filename="../plugins/colorwheel/cwdialog.cpp" line="75"/>
        <source>Document Colors</source>
        <translation>Barvy v dokumentu</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <location filename="../scribusdoc.cpp" line="1231"/>
        <source>New Layer</source>
        <translation>Nová vrstva</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="2138"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="133"/>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="272"/>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="5682"/>
        <source>Do you really want to clear all your text?</source>
        <translation>Opravdu chcete vyčistit celý text?</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="5725"/>
        <source>Cannot Delete In-Use Item</source>
        <translation>Nelze smazat používaný objekt</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="5725"/>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku, nelze tedy nic smazat</translation>
    </message>
    <message>
        <location filename="../scribusdoc.cpp" line="565"/>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Při otevírání ICC profilů došlo k chybě, správa barev není aktivní.</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <location filename="../scribus.cpp" line="299"/>
        <source>Initializing Plugins</source>
        <translation>Inicializace zásuvných modulů</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="301"/>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Inicializace klávesových skratek</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="303"/>
        <source>Reading Preferences</source>
        <translation>Načítám nastavení</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="308"/>
        <source>Initializing Story Editor</source>
        <translation>Inicializace zápisníku</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="316"/>
        <source>Reading ICC Profiles</source>
        <translation>Inicializece ICC profilů</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="321"/>
        <source>Initializing Hyphenator</source>
        <translation>Inicializace dělení slov</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="326"/>
        <source>Reading Scrapbook</source>
        <translation>Načítám zápisník</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="328"/>
        <source>Setting up Shortcuts</source>
        <translation>Nastavení klávesových zkratek</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="406"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="416"/>
        <source>Edit</source>
        <translation>Upravit</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="436"/>
        <source>Searching for Fonts</source>
        <translation>Hledám písma</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="442"/>
        <source>There are no fonts found on your system.</source>
        <translation>Nenalezena zádná písma.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="443"/>
        <source>Exiting now.</source>
        <translation>Ukončuji.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3295"/>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="447"/>
        <source>Font System Initialized</source>
        <translation>Inicializován systém písem</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8933"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8934"/>
        <source>Open &amp;Recent</source>
        <translation>Otevřít &amp;předchozí</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8935"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8936"/>
        <source>&amp;Export</source>
        <translation>&amp;Export</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8937"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8939"/>
        <source>St&amp;yle</source>
        <translation>St&amp;yl</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8940"/>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8941"/>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8942"/>
        <source>&amp;Shade</source>
        <translation>&amp;Stín</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8943"/>
        <source>&amp;Font</source>
        <translation>&amp;Písmo</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8944"/>
        <source>&amp;Effects</source>
        <translation>&amp;Efekty</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8945"/>
        <source>&amp;Item</source>
        <translation>O&amp;bjekt</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="751"/>
        <source>Preview Settings</source>
        <translation>Nastavení ukázky</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="756"/>
        <source>Level</source>
        <translation>Hladina</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="762"/>
        <source>Send to La&amp;yer</source>
        <translation>Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8949"/>
        <source>&amp;PDF Options</source>
        <translation>&amp;Možnosti PDF</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8950"/>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8951"/>
        <source>C&amp;onvert To</source>
        <translation>&amp;Převést na</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8952"/>
        <source>I&amp;nsert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8953"/>
        <source>Character</source>
        <translation>Znaky</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8954"/>
        <source>Quote</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8955"/>
        <source>Space</source>
        <translation>Mezery</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8957"/>
        <source>&amp;Page</source>
        <translation>Str&amp;ana</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8958"/>
        <source>&amp;View</source>
        <translation>Ná&amp;hled</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8960"/>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8961"/>
        <source>&amp;Windows</source>
        <translation>&amp;Okna</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8962"/>
        <source>&amp;Help</source>
        <translation>Nápo&amp;věda</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8963"/>
        <source>&amp;Alignment</source>
        <translation>&amp;Zarovnání</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8974"/>
        <source>Ready</source>
        <translation>Připravený</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3759"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3102"/>
        <source>Importing Pages...</source>
        <translation>Importuji strany...</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3133"/>
        <source>Import Page(s)</source>
        <translation>Importovat stranu(y)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3174"/>
        <source>Import done</source>
        <translation>Import ukončen</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3178"/>
        <source>Found nothing to import</source>
        <translation>Nenalezeno nic k importu</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3295"/>
        <source>File %1 is not in an acceptable format</source>
        <translation>Soubor %1 není v přijatelném formátu</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3315"/>
        <source>Loading...</source>
        <translation>Načítám...</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3459"/>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Některé ICC profily použité tímto dokumentem nejsou nainstalované:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3462"/>
        <source> was replaced by: </source>
        <translation> zaměněn za: </translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3521"/>
        <source>(converted)</source>
        <translation>(konvertováno)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3719"/>
        <source>All Supported Formats</source>
        <translation>Všechny podporované formáty</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3750"/>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7399"/>
        <source>Cannot write the file: 
%1</source>
        <translation>Nelze uložit soubor: 
%1</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3900"/>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3903"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3906"/>
        <source>Save As</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3935"/>
        <source>Saving...</source>
        <translation>Ukládám...</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7146"/>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus detekoval nějaké chyby. Zvažte použití předtiskové kontroly pro jejich odstranění</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7286"/>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorovat</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7286"/>
        <source>&amp;Abort</source>
        <translation>&amp;Přerušit</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4154"/>
        <source>Printing...</source>
        <translation>Tisknu...</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7236"/>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4250"/>
        <source>Printing failed!</source>
        <translation>Tisk se nepodařil!</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4368"/>
        <source>Cannot Cut In-Use Item</source>
        <translation>Nelze odstranit používaný objekt</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4368"/>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku. proto nelze nic vyříznout</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4879"/>
        <source>About Qt</source>
        <translation>O Qt</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4884"/>
        <source>Scribus Manual</source>
        <translation>Scribus manuál</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7253"/>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4903"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové soubory (*.txt);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="4988"/>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="5056"/>
        <source>Name:</source>
        <translation>Název:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="5056"/>
        <source>Convert Page to Master Page</source>
        <translation>Převést stranu na vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6155"/>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6155"/>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6206"/>
        <source>&amp;Shade:</source>
        <translation>&amp;Stín:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6206"/>
        <source>Shade</source>
        <translation>Stín</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6430"/>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7047"/>
        <source>The following programs are missing:</source>
        <translation>Chybí následující program(y):</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7049"/>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: nelze použít EPS obrázky nebo Náhled před tiskem</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7185"/>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7205"/>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus detekoval nějaké chyby.
Zvažte použití předtiskové kontrol pro jejich odstranění.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7253"/>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS soubory (*.eps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7285"/>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Detekovány chyby.
Zvažte použití předtiskové kontroly pro jejich odstranění</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7376"/>
        <source>-Page%1</source>
        <translation>-Strana%1</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7733"/>
        <source>Some objects are locked.</source>
        <translation>Některé objekty jsou zamčeny.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7735"/>
        <source>&amp;Lock All</source>
        <translation>&amp;Zamknout vše</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7736"/>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odemknout vše</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8695"/>
        <source>Information</source>
        <translation>Informace</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8695"/>
        <source>The program %1 is already running!</source>
        <translation>Program %1 už běží!</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8735"/>
        <source>The program %1 is missing!</source>
        <translation>Program %1 nenalezen!</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8857"/>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Vybraná barva v barevné paletě dokumentu neexistuje. Zadejte prosím její nové jméno.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8860"/>
        <source>Color Not Found</source>
        <translation>Barva nenalezena</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8864"/>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Barva, kterou jste vybrali, již existuje. Pojmenujte prosím novou barvu jinak.</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8946"/>
        <source>&amp;Level</source>
        <translation>&amp;Hladina</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8947"/>
        <source>Send to Layer</source>
        <translation>Přesunout do vrstvy</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8948"/>
        <source>Previe&amp;w Settings</source>
        <translation>Nas&amp;tavení náhledu</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8959"/>
        <source>&amp;Tools</source>
        <translation>&amp;Nástroje</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8970"/>
        <source>X-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8971"/>
        <source>Y-Pos:</source>
        <translation>Y-Poz:</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="873"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mezery &amp;&amp; Zalomení</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="886"/>
        <source>Ligature</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="5056"/>
        <source>New Master Page %1</source>
        <translation>Nová vzorová strana %1</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="6890"/>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Počet kopií: %1
Vodorovné posunutí: %2
Svislé posunutí: %3</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7051"/>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: nelze použít EPS obrázky nebo PS Náhled před tiskem</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="7093"/>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript chybí: PS Náhled před tiskem nelze použít</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="9127"/>
        <source>Do you really want to replace your existing image?</source>
        <translation>Opravdu chcete zaměnit existující obrázek?</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8938"/>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="8956"/>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;tura</translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="242"/>
        <source>Scribus </source>
        <translation>Scribus </translation>
    </message>
    <message>
        <location filename="../scribus.cpp" line="3919"/>
        <source>Your document was saved to a temporary file and could not be moved: 
%1</source>
        <translation>Váš dokument byl uložen jako dočasný soubor, ale nelze jej přesunout: 
%1</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <location filename="../scribusapp.cpp" line="197"/>
        <source>Invalid argument: </source>
        <translation>Nesprávný argument: </translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="199"/>
        <source>File %1 does not exist, aborting.</source>
        <translation>Soubor %1 neexistuje, končím.</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="397"/>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Použití: scribus [volba ...] [soubor]</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="398"/>
        <source>Options:</source>
        <translation>Volby:</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="400"/>
        <source>Print help (this message) and exit</source>
        <translation>Zobrazit nápovědu (tuto zprávu) a skončit</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="402"/>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Používá xx jako zkratku pro jazyk, např. &apos;en&apos; nebo &apos;de&apos;</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="404"/>
        <source>List the currently installed interface languages</source>
        <translation>Zobrazit dostupné jazykové verze prostředí</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="406"/>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Zobrazit na konzoli informace o načítání písem</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="408"/>
        <source>Do not show the splashscreen on startup</source>
        <translation>Nezobrazovat při startu uvítací obrazovku</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="410"/>
        <source>Output version information and exit</source>
        <translation>Zobrazit informace o verzi a skončit</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="412"/>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Použít pořadí tlačítek zprava doleva (tedy Zrušit/Ne/Ano místo Ano/Ne/Zrušit)</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="413"/>
        <source>filename</source>
        <translation>jméno souboru</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="414"/>
        <source>Use filename as path for user given preferences</source>
        <translation>Jméno souboru pro uživatelské nastavení</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="434"/>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Nainstalované jazykové verze v prostředí Scribusu:</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="442"/>
        <source>To override the default language choice:</source>
        <translation>Změna nastavení jazyka:</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="443"/>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx nebo scribus --lang xx, kde xx je kód jazyka.</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="448"/>
        <source>Scribus Version</source>
        <translation>Verze Scribusu</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="457"/>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Open Source Desktop Publishing</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="465"/>
        <source>Homepage</source>
        <translation>Webová stránka</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="466"/>
        <source>Documentation</source>
        <translation>Dokumentace</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="467"/>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="468"/>
        <source>Issues</source>
        <translation>Vydání</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="419"/>
        <source>Display a console window</source>
        <translation>Zobrazit okno konzole</translation>
    </message>
    <message>
        <location filename="../scribusapp.cpp" line="416"/>
        <source>Show location ICC profile information on console while starting</source>
        <translation>Zobrazit při startu konzole informace o ICC profilu</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <location filename="../scribusview.cpp" line="231"/>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="232"/>
        <source>Layer</source>
        <translation>Vrstva</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1186"/>
        <source>Copy Here</source>
        <translation>Kopírovat sem</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1187"/>
        <source>Move Here</source>
        <translation>Přesunout sem</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1188"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1827"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1909"/>
        <source>Picture</source>
        <translation>Obrázek</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1911"/>
        <source>File: </source>
        <translation>Soubor:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1915"/>
        <source>Original PPI: </source>
        <translation>Původní PPI:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1919"/>
        <source>Actual PPI: </source>
        <translation>Aktuální PPI:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1964"/>
        <source>Linked Text</source>
        <translation>Řeťezený text</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1966"/>
        <source>Text Frame</source>
        <translation>Textový rám</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1969"/>
        <source>Text on a Path</source>
        <translation>Text na křivce</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1972"/>
        <source>Paragraphs: </source>
        <translation>Odstavce:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1979"/>
        <source>Words: </source>
        <translation>Slova:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1986"/>
        <source>Chars: </source>
        <translation>Znaky:</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Print: </source>
        <translation type="obsolete">Tisk:</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2000"/>
        <source>Enabled</source>
        <translation>Povoleno</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2002"/>
        <source>Disabled</source>
        <translation>Zakázáno</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2008"/>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2022"/>
        <source>Preview Settings</source>
        <translation>Nastavení náhledu</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2064"/>
        <source>&amp;PDF Options</source>
        <translation>&amp;Možnosti PDF</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2087"/>
        <source>Send to La&amp;yer</source>
        <translation>Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2115"/>
        <source>Le&amp;vel</source>
        <translation>&amp;Hladina</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2162"/>
        <source>Conve&amp;rt to</source>
        <translation>K&amp;onverze na</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2172"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="5284"/>
        <source>Linking Text Frames</source>
        <translation>Řetězení textových rámů</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="5285"/>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Pokoušíte se odkazovat na vyplněný rámec, nebo na rámec samotný.</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="9829"/>
        <source>Cannot Convert In-Use Item</source>
        <translation>Nelze konvertovat používaný objekt</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="9829"/>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Objekt %1 je momentálně upravován v zápisníku, proto bude přeskočeno jeho konvertování na křivky</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="8697"/>
        <source>Page %1 to %2</source>
        <translation>Strana %1 až %2</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1924"/>
        <source>Colorspace: </source>
        <translation>Barevný prostor: </translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1930"/>
        <source>Unknown</source>
        <translation>Neznámý</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1936"/>
        <source>RGB</source>
        <translation>RGB</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1939"/>
        <source>CMYK</source>
        <translation>CMYK</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1942"/>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="2175"/>
        <source>Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <location filename="../scribusview.cpp" line="1997"/>
        <source>Export: </source>
        <translation>Export: </translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <location filename="../scribuswin.cpp" line="95"/>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <location filename="../scribuswin.cpp" line="95"/>
        <source>has been changed since the last save.</source>
        <translation>od posledního uložení byl dokument změněný.</translation>
    </message>
    <message>
        <location filename="../scribuswin.cpp" line="96"/>
        <source>&amp;Discard</source>
        <translation>&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <location filename="../plugins/scriptplugin/scriptplugin.cpp" line="140"/>
        <source>Embedded Python scripting support.</source>
        <translation>Zabudovaná podpora pro skripty v Pythonu.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptplugin.cpp" line="192"/>
        <source>Scripter</source>
        <translation>Skripter</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="574"/>
        <source>Script error</source>
        <translation>Chyba ve skriptu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="356"/>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Jestliže běžel skript distribuovaný s programem, informujte nás o chybě na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; - děkujeme vám.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="358"/>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Tato chybová zpráva je ve vaší systémové schránce. Použijte Ctrl+V, tím ji
můžete zkopírovat do chybového hlášení.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="425"/>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Došlo k vnitřní chybě při provádění příkazu, který jste zadali. Detaily vypsány na stderr. </translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="497"/>
        <source>Examine Script</source>
        <translation>Prozkoumat skript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="576"/>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Chybná inicializace Python modulu. Detaily chyby byly vypsány na standardní chybový výstup. </translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="497"/>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Python skripty (*.py);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="510"/>
        <source>Documentation for:</source>
        <translation>Dokumentace:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="515"/>
        <source>Script</source>
        <translation>Skript</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scriptercore.cpp" line="515"/>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> neobsahuje žádný docstring!</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="132"/>
        <source>Scripter Preferences</source>
        <translation>Nastavení skripteru</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="133"/>
        <source>Enable Extension Scripts</source>
        <translation>Povolit Python rozšíření</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="35"/>
        <source>Extensions</source>
        <translation>Rozšíření</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="37"/>
        <source>Console</source>
        <translation>Konzole</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="134"/>
        <source>Startup Script:</source>
        <translation>Skript při startu:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="135"/>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Chyba:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="136"/>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Komentáře:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="137"/>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Klíčová slova:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="138"/>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Operátory:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="139"/>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Čísla:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="140"/>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Řetězce:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="141"/>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Základní texty:</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="164"/>
        <source>Select Color</source>
        <translation>Vyberte barvu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="50"/>
        <source>Change...</source>
        <translation>Změnit...</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/scripterprefsgui.cpp" line="189"/>
        <source>Locate Startup Script</source>
        <translation>Umístění start skriptu</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <location filename="../seiten.cpp" line="74"/>
        <source>Show Page Previews</source>
        <translation>Zobrazit náhledy stran</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <location filename="../search.cpp" line="644"/>
        <source>Search/Replace</source>
        <translation>Hledat/Nahradit</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="55"/>
        <source>Search for:</source>
        <translation>Hledat:</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="160"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="163"/>
        <source>Paragraph Style</source>
        <translation>Styl odstavce</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="166"/>
        <source>Font</source>
        <translation>Písmo</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="169"/>
        <source>Font Size</source>
        <translation>Velikost písma</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="172"/>
        <source>Font Effects</source>
        <translation>Efekty písma</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="175"/>
        <source>Fill Color</source>
        <translation>Barva výplně</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="178"/>
        <source>Fill Shade</source>
        <translation>Odstín výplně</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="181"/>
        <source>Stroke Color</source>
        <translation>Barva obrysu</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="184"/>
        <source>Stroke Shade</source>
        <translation>Odstín obrysu</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="93"/>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="93"/>
        <source>Center</source>
        <translation>Na střed</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="93"/>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="93"/>
        <source>Block</source>
        <translation>Do bloku</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="93"/>
        <source>Forced</source>
        <translation>Vynucené</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="208"/>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="153"/>
        <source>Replace with:</source>
        <translation>Nahradit čím:</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="516"/>
        <source>Search finished</source>
        <translation>Hledání hotovo</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="250"/>
        <source>&amp;Whole Word</source>
        <translation>&amp;Celé slovo</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="254"/>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorovat velikost znaků</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="261"/>
        <source>&amp;Search</source>
        <translation>&amp;Hledat</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="264"/>
        <source>&amp;Replace</source>
        <translation>&amp;Nahradit</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="267"/>
        <source>Replace &amp;All</source>
        <translation>Nahradit &amp;vše</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="272"/>
        <source>&amp;Close</source>
        <translation>&amp;Zavřít</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="270"/>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <location filename="../search.cpp" line="645"/>
        <source>Search finished, found %1 matches</source>
        <translation>Hledání dokončeno, nalezeno %1</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <location filename="../selfield.cpp" line="20"/>
        <source>Select Fields</source>
        <translation>Zvolit pole</translation>
    </message>
    <message>
        <location filename="../selfield.cpp" line="27"/>
        <source>Available Fields</source>
        <translation>Dostupná pole</translation>
    </message>
    <message>
        <location filename="../selfield.cpp" line="61"/>
        <source>Selected Fields</source>
        <translation>Zvolená pole</translation>
    </message>
    <message>
        <location filename="../selfield.cpp" line="53"/>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <location filename="../selfield.cpp" line="55"/>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <location filename="../shadebutton.cpp" line="17"/>
        <source>Other...</source>
        <translation>Jiný...</translation>
    </message>
    <message>
        <location filename="../shadebutton.cpp" line="48"/>
        <source>Shade</source>
        <translation>Stín</translation>
    </message>
    <message>
        <location filename="../shadebutton.cpp" line="48"/>
        <source>&amp;Shade:</source>
        <translation>&amp;Stín:</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <location filename="../styleselect.cpp" line="150"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="149"/>
        <source>X-Offset</source>
        <translation>X-posun</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="151"/>
        <source>Y-Offset</source>
        <translation>Y-posun</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="73"/>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Předložky a zkratky (nezlomitelná mezera)...</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="154"/>
        <source>Short Words</source>
        <translation>Předložky a zkratky</translation>
    </message>
    <message>
        <location filename="../plugins/short-words/shortwords.cpp" line="96"/>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Speciální modul, který doplní nezlomitelné mezery před nebo za předložky a zkratky a další &quot;krátká slova&quot;. Dostupný v následujících jazycích: </translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <location filename="../story.cpp" line="146"/>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="95"/>
        <source>Edit Styles...</source>
        <translation>Úprava stylů...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <location filename="../spalette.cpp" line="64"/>
        <source>No Style</source>
        <translation>Bez stylu</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <location filename="../editformats.cpp" line="186"/>
        <source>Edit Styles</source>
        <translation>Upravit styly</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="287"/>
        <source>Copy of %1</source>
        <translation>Kopie %1</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="333"/>
        <source>New Style</source>
        <translation>Nový styl</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="433"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="431"/>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="433"/>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="205"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="208"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="213"/>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikovat</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="217"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../editformats.cpp" line="202"/>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <location filename="../story.cpp" line="2511"/>
        <source>Story Editor</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2428"/>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2430"/>
        <source>Current Paragraph:</source>
        <translation>Aktuální odstavec:</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2435"/>
        <source>Words: </source>
        <translation>Slov:</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2436"/>
        <source>Chars: </source>
        <translation>Znaků:</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2433"/>
        <source>Totals:</source>
        <translation>Celkem:</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2434"/>
        <source>Paragraphs: </source>
        <translation>Odstavců:</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="3673"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="3709"/>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové soubory (*.txt);;Všechny soubory (*)</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="3709"/>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2545"/>
        <source>Do you want to save your changes?</source>
        <translation>Opravdu chcete uložit změny?</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2392"/>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2394"/>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Znovu nahrát původní text z rámu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2395"/>
        <source>&amp;Save to File...</source>
        <translation>Uložit do &amp;souboru...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2396"/>
        <source>&amp;Load from File...</source>
        <translation>Nahrá&amp;t ze souboru...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2397"/>
        <source>Save &amp;Document</source>
        <translation>Uložit &amp;dokument</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2398"/>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Aktualizovat textový rám a skončit</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2399"/>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Skončit &amp;bez aktualizace textového rámu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2403"/>
        <source>Cu&amp;t</source>
        <translation>Vyjmou&amp;t</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2404"/>
        <source>&amp;Copy</source>
        <translation>&amp;Kopírovat</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2405"/>
        <source>&amp;Paste</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2406"/>
        <source>C&amp;lear</source>
        <translation>Vy&amp;mazat</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2410"/>
        <source>&amp;Update Text Frame</source>
        <translation>Ak&amp;tualizovat textový rám</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2391"/>
        <source>&amp;File</source>
        <translation>&amp;Soubor</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2401"/>
        <source>&amp;Edit</source>
        <translation>Ú&amp;pravy</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2402"/>
        <source>Select &amp;All</source>
        <translation>Vybr&amp;at vše</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2408"/>
        <source>&amp;Edit Styles...</source>
        <translation>Upravit &amp;styly...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2407"/>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hledat/Nahradit...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2409"/>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Náhled písem...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2421"/>
        <source>&amp;Background...</source>
        <translation>&amp;Pozadí...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2422"/>
        <source>&amp;Display Font...</source>
        <translation>&amp;Zobrazit písmem...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2420"/>
        <source>&amp;Settings</source>
        <translation>&amp;Nastavení</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2423"/>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Chytrý výběr textu</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2417"/>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Vložit znak...</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2393"/>
        <source>Clear All Text</source>
        <translation>Vymazat text</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2495"/>
        <source>Story Editor - %1</source>
        <translation>Zápisník - %1</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="3100"/>
        <source>Do you really want to lose all your changes?</source>
        <translation>Opravdu chcete zrušit všechny své změny?</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="3133"/>
        <source>Do you really want to clear all your text?</source>
        <translation>Opravdu chcete smazat celý text?</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2413"/>
        <source>&amp;Insert</source>
        <translation>&amp;Vložit</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2414"/>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2415"/>
        <source>Quote</source>
        <translation>Uvozovky</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2229"/>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mezery &amp;&amp; Zalomení</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2241"/>
        <source>Ligature</source>
        <translation>Ligatury</translation>
    </message>
    <message>
        <location filename="../story.cpp" line="2416"/>
        <source>Space</source>
        <translation>Mezera</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <location filename="../styleselect.cpp" line="41"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="52"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="51"/>
        <source>Displacement</source>
        <translation>Posunití</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="53"/>
        <source>Linewidth</source>
        <translation>Šířka čáry</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <location filename="../stylemanager.cpp" line="102"/>
        <source>More than one item selected</source>
        <translation>Zvoleno více než jedna položka</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <location filename="../styleselect.cpp" line="310"/>
        <source>Small Caps</source>
        <translation>Kapitálky</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="311"/>
        <source>Subscript</source>
        <translation>Dolní index</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="312"/>
        <source>Superscript</source>
        <translation>Horní index</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="309"/>
        <source>All Caps</source>
        <translation>Verzálky</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="307"/>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podtrhnout text. Stiskněte na chvíli tlačítko, abyste nastavili šířku čáry a možnosti posunutí.</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="308"/>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podtrhnout pouze slova. Stiskněte na chvíli tlačítko, abyste nastavili šířku čáry a možnosti posunutí.</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="313"/>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Proškrtnutí. Stiskněte na chvíli tlačítko, abyste nastavili šířku čáry a možnosti posunutí.</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="314"/>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation>Obrys. Stiskněte na chvíli tlačítko, abyste změnili šířku obrysové čáry.</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="315"/>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Stínovaný text. Stiskněte na chvíli tlačítko, abyste nastavili odstup stínu od písma.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="40"/>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použít jméno dokumentu jako předponu stylů odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="47"/>
        <source>Do not ask again</source>
        <translation>Neptat se znovu</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="57"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="19"/>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Nastavení importu OpenOffice.org Writer</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="28"/>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Přepsat existující styly novými</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="33"/>
        <source>Merge Paragraph Styles</source>
        <translation>Sloučit styly odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="35"/>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sloučí styly podle jejich vlastností. Výsledkem bude několik stylů se specifickými vlastnostmi, přestože původní dokument obsahoval styly pojmenované jinak.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="42"/>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Přidat název dokumentu do jména stylu.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="50"/>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Nastavit tyto vlastnosti jako implicitní a neptat se na ně při každém importu OASIS OpenDocument formátu.</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="25"/>
        <source>Overwrite Paragraph Styles</source>
        <translation>Přepsat styly odstavců</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/sxwim/sxwdia.cpp" line="59"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <location filename="../tocindexprefs.cpp" line="105"/>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="107"/>
        <source>At the beginning</source>
        <translation>Na začátku</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="109"/>
        <source>At the end</source>
        <translation>Na konci</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="111"/>
        <source>Not Shown</source>
        <translation>Nezobrazeno</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="83"/>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstřík</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="98"/>
        <source>Table Of Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="84"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="85"/>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="86"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="87"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="88"/>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rámec s obsahem bude umístěn</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="89"/>
        <source>Page Numbers Placed:</source>
        <translation>Čísla stran umístěna:</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="90"/>
        <source>Item Attribute Name:</source>
        <translation>Jméno atributu objektu:</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="91"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Vlastnost objektu, která bude u rámců použita jako základ pro vytváření záznamů</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="92"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Umístit čísla stran na začátek řádků, nebo na konec, nebo vůbec</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="93"/>
        <source>List Non-Printing Entries</source>
        <translation>Vypsat netisknuté položky</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="94"/>
        <source>Include frames that are set to not print as well</source>
        <translation>Začlenit rámy, které mají vypnutý tisk</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="95"/>
        <source>The paragraph style used for the entry lines</source>
        <translation>Odstavcový styl použitý pro vstupní řádky</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="96"/>
        <source>Paragraph Style:</source>
        <translation>Styl odstavce:</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="97"/>
        <source>Destination Frame:</source>
        <translation>Cílový rám:</translation>
    </message>
    <message>
        <location filename="../tocindexprefs.cpp" line="247"/>
        <source>Table of Contents %1</source>
        <translation>Obsah %1</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <location filename="../tocindexprefsbase.ui" line="16"/>
        <source>Table of Contents and Indexes</source>
        <translation>Obsah a rejstřík</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="31"/>
        <source>Table Of Contents</source>
        <translation>Obsah</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="68"/>
        <source>&amp;Add</source>
        <translation>&amp;Připojit</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="71"/>
        <source>Alt+A</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="79"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="82"/>
        <source>Alt+D</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="110"/>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rám s obsahem bude umístěn</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="118"/>
        <source>Page Numbers Placed:</source>
        <translation>Čísla stran umístěna:</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="126"/>
        <source>Item Attribute Name:</source>
        <translation>Jméno atributu objektu:</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="134"/>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Vlastnost objektu, která bude nastavena u rámců jako základ při vytváření nových položek</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="142"/>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Umístit čísla stránek na začátek řádku, nebo na konec, nebo vůbec</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="150"/>
        <source>List Non-Printing Entries</source>
        <translation>Vypsat netisknuté položky</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="153"/>
        <source>Include frames that are set to not print as well</source>
        <translation>Začlenit rámy, které mají vypnutý tisk</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="161"/>
        <source>The paragraph style used for the entry lines</source>
        <translation>Odstavcový styl použitý pro vstupní řádky</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="169"/>
        <source>Paragraph Style:</source>
        <translation>Styl odstavce:</translation>
    </message>
    <message>
        <location filename="../tocindexprefsbase.ui" line="177"/>
        <source>Destination Frame:</source>
        <translation>Cílový rám:</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <location filename="../tabcheckdoc.cpp" line="38"/>
        <source>Ignore all errors</source>
        <translation>Ignorovat všechny chyby</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="41"/>
        <source>Automatic check before printing or exporting</source>
        <translation>Automaticky kontrolovat před tiskem nebo exportem</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="44"/>
        <source>Check for missing glyphs</source>
        <translation>Kontrolovat chybějící znaky</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="47"/>
        <source>Check for objects not on a page</source>
        <translation>Kontrolovat objekty mimo strany</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="50"/>
        <source>Check for overflow in text frames</source>
        <translation>Kontrolovat přetečení textu</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="53"/>
        <source>Check for transparencies used</source>
        <translation>Kontrolovat transparentnost</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="56"/>
        <source>Check for missing images</source>
        <translation>Kontrolovat chybějící obrázky</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="59"/>
        <source>Check image resolution</source>
        <translation>Kontrolovat rozlišení obrázků</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="67"/>
        <source>Lowest allowed resolution</source>
        <translation>Nejnižší povolené rozlišení</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="72"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="76"/>
        <source>Check for placed PDF Files</source>
        <translation>Kontrolovat umístěné PDF soubory</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="79"/>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrolovat PDF anotace a formuláře</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="84"/>
        <source>Add Profile</source>
        <translation>Přidat profil</translation>
    </message>
    <message>
        <location filename="../tabcheckdoc.cpp" line="86"/>
        <source>Remove Profile</source>
        <translation>Odstranit profil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <location filename="../tabguides.cpp" line="35"/>
        <source>Common Settings</source>
        <translation>Běžná nastavení</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="46"/>
        <source>Placing in Documents</source>
        <translation>Umístění v dokumentech</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="50"/>
        <source>In the Background</source>
        <translation>V pozadí</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="53"/>
        <source>In the Foreground</source>
        <translation>V popředí</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="60"/>
        <source>Snapping</source>
        <translation>Jako magnet</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="66"/>
        <source>Snap Distance:</source>
        <translation>Vzdálenost pro magnet:</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="73"/>
        <source>Grab Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="80"/>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="86"/>
        <source>Show Guides</source>
        <translation>Zobrazit vodítka</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="213"/>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="110"/>
        <source>Show Margins</source>
        <translation>Zobrazit okraje</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="134"/>
        <source>Show Page Grid</source>
        <translation>Zobrazit mřížku strany</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="146"/>
        <source>Major Grid</source>
        <translation>Hlavní mřížka</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="193"/>
        <source>Spacing:</source>
        <translation>Vzdálenost:</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="175"/>
        <source>Minor Grid</source>
        <translation>Vedlejší mřížka</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="204"/>
        <source>Show Baseline Grid</source>
        <translation>Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="229"/>
        <source>Baseline Settings</source>
        <translation>Nastavení účaří</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="241"/>
        <source>Baseline &amp;Grid:</source>
        <translation>Pomocná &amp;mřížka:</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="247"/>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Vzdálenost pomocné mřížky:</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="257"/>
        <source>Guides are not visible through objects on the page</source>
        <translation>Vodítka nejsou viditelná skrz objekty na stránce</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="258"/>
        <source>Guides are visible above all objects on the page</source>
        <translation>Vodítka jsou viditelná nad všemi objekty na stránce</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="259"/>
        <source>Distance between the minor grid lines</source>
        <translation>Vzdálenost mezi čarami vedlejší mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="260"/>
        <source>Distance between the major grid lines</source>
        <translation>Vzdálenost mezi čarami hlavní mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="261"/>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Vzdálenost, od které se objekt přitáhne k vodítkům</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="262"/>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Poloměr oblasti, kterou Scribus považuje za oblast daného objektu</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="263"/>
        <source>Color of the minor grid lines</source>
        <translation>Barva čar vedlejší mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="264"/>
        <source>Color of the major grid lines</source>
        <translation>Barva čar hlavní mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="265"/>
        <source>Color of the guide lines you insert</source>
        <translation>Barva vodítek, které vkládáte manuálně</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="266"/>
        <source>Color for the margin lines</source>
        <translation>Barva okrajových čar</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="267"/>
        <source>Color for the baseline grid</source>
        <translation>Barva pomocné mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="268"/>
        <source>Turns the basegrid on or off</source>
        <translation>Přepne zobrazení základního rastru</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="269"/>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Vzdálenost mezi čarami pomocné mřížky</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="270"/>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Vzdálenost prvního účaří od horního okraje </translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="271"/>
        <source>Turns the gridlines on or off</source>
        <translation>Přepne zobrazení rastru</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="272"/>
        <source>Turns the guides on or off</source>
        <translation>Přepne zobrazení vodítek</translation>
    </message>
    <message>
        <location filename="../tabguides.cpp" line="273"/>
        <source>Turns the margins on or off</source>
        <translation>Přepne zobrazení okrajů</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <location filename="../tabmanager.cpp" line="23"/>
        <source>Manage Tabulators</source>
        <translation>Správa tabelátorů</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <location filename="../tabpdfoptions.cpp" line="242"/>
        <source>Export Range</source>
        <translation>Exportovat rozsah</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="248"/>
        <source>&amp;All Pages</source>
        <translation>Všechny str&amp;any</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="252"/>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Vybrat strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="258"/>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotace:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="286"/>
        <source>File Options</source>
        <translation>Nastavení souboru</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="294"/>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilita:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="324"/>
        <source>&amp;Binding:</source>
        <translation>Vaz&amp;ba:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="328"/>
        <source>Left Margin</source>
        <translation>Levý okraj</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="329"/>
        <source>Right Margin</source>
        <translation>Pravý okraj</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="334"/>
        <source>Generate &amp;Thumbnails</source>
        <translation>Vytvořit &amp;náhledy</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="337"/>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Uložit &amp;propojené textové rámy jako PDF články</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="340"/>
        <source>&amp;Include Bookmarks</source>
        <translation>Vče&amp;tně záložek</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="397"/>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="355"/>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Rozlišení pro EPS grafiku:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="361"/>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primovat textovou a vektorovou grafiku</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="372"/>
        <source>Automatic</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="375"/>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="383"/>
        <source>Maximum</source>
        <translation>Maximální</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="384"/>
        <source>High</source>
        <translation>Vysoká</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="385"/>
        <source>Medium</source>
        <translation>Střední</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="386"/>
        <source>Low</source>
        <translation>Nízká</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="387"/>
        <source>Minimum</source>
        <translation>Minimální</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="404"/>
        <source>&amp;General</source>
        <translation>&amp;Všeobecné</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="431"/>
        <source>Embedding</source>
        <translation>Vkládání</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="442"/>
        <source>Available Fonts:</source>
        <translation>Dostupná písma:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="463"/>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="466"/>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt;</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="475"/>
        <source>Fonts to embed:</source>
        <translation>Písma k vložení:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="543"/>
        <source>&amp;Fonts</source>
        <translation>&amp;Písma</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="548"/>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Povolit efekty &amp;prezentace</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1630"/>
        <source>Page</source>
        <translation>Strana</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="587"/>
        <source>Show Page Pre&amp;views</source>
        <translation>Zobrazit &amp;náhledy stran</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="590"/>
        <source>Effects</source>
        <translation>Efekty</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="598"/>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Doba zobrazení:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="600"/>
        <source>Effec&amp;t Duration:</source>
        <translation>Tr&amp;vání efektu:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="602"/>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Typ efektu:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="604"/>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Přesouvání řádků:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="606"/>
        <source>F&amp;rom the:</source>
        <translation>O&amp;d:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="608"/>
        <source>D&amp;irection:</source>
        <translation>&amp;Směr:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="618"/>
        <source> sec</source>
        <translation>s</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>No Effect</source>
        <translation>Bez efektu</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Blinds</source>
        <translation>Pruhy</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Box</source>
        <translation>Rám</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Dissolve</source>
        <translation>Rozpuštění</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Glitter</source>
        <translation>Lesk</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Split</source>
        <translation>Rozdělit</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="625"/>
        <source>Wipe</source>
        <translation>Setřít</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="633"/>
        <source>Horizontal</source>
        <translation>Vodorovně</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="634"/>
        <source>Vertical</source>
        <translation>Svisle</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="639"/>
        <source>Inside</source>
        <translation>Zevnitř</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="640"/>
        <source>Outside</source>
        <translation>Zvenku</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="645"/>
        <source>Left to Right</source>
        <translation>Zleva doprava</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="645"/>
        <source>Top to Bottom</source>
        <translation>Shora dolů</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="645"/>
        <source>Bottom to Top</source>
        <translation>Zdola nahoru</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="645"/>
        <source>Right to Left</source>
        <translation>Zprava doleva</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="646"/>
        <source>Top-left to Bottom-Right</source>
        <translation>Zleva nahoře na doprava dolů</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="653"/>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Po&amp;užít efekt na všechny strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="656"/>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtra</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="785"/>
        <source>&amp;Use Encryption</source>
        <translation>Použít ši&amp;frování</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="788"/>
        <source>Passwords</source>
        <translation>Hesla</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="796"/>
        <source>&amp;User:</source>
        <translation>&amp;Uživatel:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="798"/>
        <source>&amp;Owner:</source>
        <translation>&amp;Vlastník:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="812"/>
        <source>Settings</source>
        <translation>Nastavení</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="820"/>
        <source>Allow &amp;Printing the Document</source>
        <translation>&amp;Povolit tisk dokumentu</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="823"/>
        <source>Allow &amp;Changing the Document</source>
        <translation>Povolit z&amp;měny dokumentu</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="826"/>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Povolit &amp;kopírování textu a grafiky</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="829"/>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Povolit přidávání &amp;anotací a polí formulářů</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="838"/>
        <source>S&amp;ecurity</source>
        <translation>&amp;Bezpečnost</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="842"/>
        <source>General</source>
        <translation>Všeobecné</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="848"/>
        <source>Output &amp;Intended For:</source>
        <translation>&amp;Plánovaný výstup na:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="851"/>
        <source>Screen / Web</source>
        <translation>Monitor / Web</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="852"/>
        <source>Printer</source>
        <translation>Tiskárna</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="853"/>
        <source>Grayscale</source>
        <translation>Odstíny šedé</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="872"/>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Použít vlastní nastavení reprodukce</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="875"/>
        <source>Rendering Settings</source>
        <translation>Nastavení reprodukce</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="890"/>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Frekvence:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="898"/>
        <source>&amp;Angle:</source>
        <translation>Úhe&amp;l:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="907"/>
        <source>S&amp;pot Function:</source>
        <translation>Funkce &amp;bodu:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="911"/>
        <source>Simple Dot</source>
        <translation>Prostá tečka</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="912"/>
        <source>Line</source>
        <translation>Čára</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="913"/>
        <source>Round</source>
        <translation>Kruh</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="914"/>
        <source>Ellipse</source>
        <translation>Elipsa</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="921"/>
        <source>Solid Colors:</source>
        <translation>Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="954"/>
        <source>Use ICC Profile</source>
        <translation>Použít ICC profil</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="962"/>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="965"/>
        <source>Rendering-Intent:</source>
        <translation>Účel generování:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="941"/>
        <source>Perceptual</source>
        <translation>Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="941"/>
        <source>Relative Colorimetric</source>
        <translation>Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="941"/>
        <source>Saturation</source>
        <translation>Sytost</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="941"/>
        <source>Absolute Colorimetric</source>
        <translation>Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="948"/>
        <source>Images:</source>
        <translation>Obrázky:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="958"/>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Nepoužívat vložené ICC profily</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1042"/>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1047"/>
        <source>PDF/X-3 Output Intent</source>
        <translation>Výstup do PDF/X-3</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1077"/>
        <source>&amp;Info String:</source>
        <translation>&amp;Info text:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1079"/>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Výstupní profil:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1083"/>
        <source>Trim Box</source>
        <translation>Vlastní formát stránky</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1110"/>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1178"/>
        <source>Show page previews of each page listed above.</source>
        <translation>Ukázat náhled každé strany uvedené v seznamu nahoře.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1181"/>
        <source>Type of the display effect.</source>
        <translation>Typ efektu.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1182"/>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Směr efektu Přesouvání řádků nebo Rozdělit.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1183"/>
        <source>Starting position for the box and split effects.</source>
        <translation>Startovní pozice efektu Rám nebo Rozdělit.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1184"/>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Směr efektu Lesk nebo Setřít.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1185"/>
        <source>Apply the selected effect to all pages.</source>
        <translation>Použít vybraný efekt na všechny strany.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1205"/>
        <source>Export all pages to PDF</source>
        <translation>Export všech stran do PDF</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1206"/>
        <source>Export a range of pages to PDF</source>
        <translation>Exportovat rozsah stran do PDF</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1214"/>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Vytvořit PDF články, což umožňuje navigaci odkazy v PDF.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1222"/>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (body na palec) pro export obrázků.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1225"/>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Zvolit heslo, které musí použít čtenář PDF.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1226"/>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Povolit tisk PDF. Jestliže není zatrženo, tisk není povolen.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1227"/>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Povolit modifikaci PDF. Jestliže není zatrženo, modifikace jsou zakázány.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1232"/>
        <source>Embed a color profile for solid colors</source>
        <translation>Vložit barevný profil plných barev</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1233"/>
        <source>Color profile for solid colors</source>
        <translation>Barevný profil plných barev</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1234"/>
        <source>Rendering intent for solid colors</source>
        <translation>Účel reprodukce plných barev</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1235"/>
        <source>Embed a color profile for images</source>
        <translation>Vložit barevný profil obrázků</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1236"/>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Nepoužívat barevný profil vložený ve zdrojových obrázcích</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1237"/>
        <source>Color profile for images</source>
        <translation>Barevný profil obrázků</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1238"/>
        <source>Rendering intent for images</source>
        <translation>Účel reprodukce obrázků</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1239"/>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Výstupní profil tisku.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1241"/>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Vzdálenost ořezu od horního okraje fyzické strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1242"/>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Vzdálenost ořezu od dolního okraje fyzické strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1243"/>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Vzdálenost ořezu od levého okraje fyzické strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1244"/>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Vzdálenost ořezu od pravého okraje fyzické strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1245"/>
        <source>Mirror Page(s) horizontally</source>
        <translation>Zrcadlit stranu(y) vodorovně</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1246"/>
        <source>Mirror Page(s) vertically</source>
        <translation>Zrcadlit stranu(y) svisle</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="868"/>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="390"/>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Komprese a kvalita:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1228"/>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Umožní kopírování textu nebo grafiky z PDF. Není-li zatrhnuto, text a grafiku nelze kopírovat.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1229"/>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Povolit přidávání anotací a polí do PDF. Pokud není zatrženo, úprava anotací a polí není možná.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1247"/>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="343"/>
        <source>Include La&amp;yers</source>
        <translation>Vložit &amp;vrstvy</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="378"/>
        <source>Compression Metho&amp;d:</source>
        <translation>&amp;Metoda komprese:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="393"/>
        <source>Resa&amp;mple Images to:</source>
        <translation>Změnit ve&amp;likost obrázků na:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1180"/>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Doba, po kterou efekt běží. Kratší doba efekt zrychlí, delší doba jej zpomalí.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1209"/>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vložte seznam identifikátorů oddělený čárkami. Identifikátor může být * pro všechny strany, 1-5 pro rozmezí stran nebo číslo konkrétní strany.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1212"/>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Určuje způsob vazby stran v PDF. Pokud ji nepotřebujete měnit, ponechte předvolenou hodnotu - vlevo.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1213"/>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Vytvoří náhledy každé strany v PDF. Některé prohlížeče je pak používají pro navigaci.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1216"/>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Vložit do dokumentu vaše záložky. Je to praktické při orientaci v dlouhých PDF dokumentech.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1217"/>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Exportovat rozlišení textu a vektorové grafiky. Neovlivní to rozlišení bitmapových obrázků jako třeba fotografií.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1218"/>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Povolí bezztrátovou kompresi textu a grafiky. Pokud nemáte důvod to měnit, nechte zatržené. Ovlivníte tak velikost PDF souboru.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1223"/>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Povolit bezpečnostní vlastnosti v exportovaném PDF. Pokud vyberete PDF 1.3, výsledné PDF bude chráněno 40bitovým šifrováním. Pokud vyberete PDF 1.4, PDF bude chráněno 128bitovým šifrováním. Upozornění: PDF šifrování není tak věrohodné jako GPG nebo PGP šifrování a má svá omezení.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1224"/>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Vyberte hlavní heslo, které povolí nebo zakáže všechny bezpečnostní vlastnosti v exportovaném PDF</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1231"/>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Jedná se o pokročilé nastavení, které není běžně povoleno. Povolte jej pouze v případě, že jej vyžaduje vaše tiskárna a máte přesné instrukce, jak to udělat. Jinak hrozí, že vytvořené PDF nebude možné korektně tisknout a rozhodně jej nebude možné používat na různých systémech.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1240"/>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Povinný řetězec pro PDF/X-3 - jinak se PDF nebude shodovat s formátem PDF/X-3. Doporučujeme, abyste použili název dokumentu.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="678"/>
        <source>Display Settings</source>
        <translation>Nastavení displeje</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="693"/>
        <source>Single Page</source>
        <translation>Jedna strana</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="696"/>
        <source>Continuous</source>
        <translation>Pruběžně</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="699"/>
        <source>Double Page Left</source>
        <translation>Dvojitá strana vlevo</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="702"/>
        <source>Double Page Right</source>
        <translation>Dvojitá strana vpravo</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="706"/>
        <source>Visual Appearance</source>
        <translation>Vzhled</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="717"/>
        <source>Use Viewers Defaults</source>
        <translation>Použít nastavení prohlížeče</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="721"/>
        <source>Use Full Screen Mode</source>
        <translation>Použít celoobrazovkový režim</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="724"/>
        <source>Display Bookmarks Tab</source>
        <translation>Zobrazit kartu Záložky</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="728"/>
        <source>Display Thumbnails</source>
        <translation>Zobrazit náhledy</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="732"/>
        <source>Display Layers Tab</source>
        <translation>Zobrazit kartu Vrstvy</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="735"/>
        <source>Hide Viewers Toolbar</source>
        <translation>Skrýt nástrojový panel prohlížeče</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="738"/>
        <source>Hide Viewers Menubar</source>
        <translation>Skrýt hlavní nabídku prohlížeče</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="741"/>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Přizpůsobit velikost stran oknu prohlížeče</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="748"/>
        <source>Special Actions</source>
        <translation>Zvláštní akce</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="759"/>
        <source>No Script</source>
        <translation>Bez skriptu</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="779"/>
        <source>Viewer</source>
        <translation>Prohlížeč</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="281"/>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Zmenšit na okraje strany</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="373"/>
        <source>Lossy - JPEG</source>
        <translation>Ztrátový - JPEG</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="374"/>
        <source>Lossless - Zip</source>
        <translation>Bezztrátový - ZIP</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="364"/>
        <source>Image Compression Method</source>
        <translation>Metoda komprese obrázku</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="755"/>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript, který se vykoná
při otevření PDF dokumentu:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1177"/>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Povolí prezentační efekty, pokud se použije Adobe&amp;#174; Reader&amp;#174; nebo jiný PDF prohlížeč, který je v celoobrazovkovém režimu podporuje.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1211"/>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Upresňuje kompatibilitu PDF. Běžné je PDF 1.3 s nejširší kompatibilitou. Pokud váš dokument používá průhlednost nebo požadujete 128bitové šifrování, pak použijte PDF 1.4. PDF 1.5 je nutné, pokud si přejete podporu vrstev.  PDF/X-3 je určen pro exportování dokumentů se správou barev v prostoru RGB, hodí se pro komerční tisky a lze jej vybrat pouze tehdy, máte-li aktivní správu barev. Použijte v případě, kdy je to nutné kvůli tiskárně nebo při tisku na čtyřbarevnou barevnou laserovou tiskárnu.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1215"/>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Vrstvy ve vašem dokumentu jsou do PDF exportovány pouze tehdy, je-li jako výstup zvolena PDF verze 1.5.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1221"/>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Změna rozlišení bitmapových obrázků na zvolené DPI. Necháte-li nezatrhnuté, budou se obrázky vykreslovat ve svém přirozeném rozlišení. Pokud volbu zatrhnete, zvýší se paměťová náročnost a zpomalí export.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1230"/>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Barevný model pro výstupní PDF. Vyberte Obrazovka/web pro PDF soubory, které se budou zobrazovat na monitoru a tisknout na běžných inkoustových tiskárnách. Pokud se budou tisknout na CMYK tiskárně, zvolte Tiskárna. Chcete-li PDF v odstínech šedé, zatrhněte Odstíny šedé.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1248"/>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Nezobrazovat v exportovaném souboru objekty, které přesahují okraje</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1179"/>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Doba, po kterou se zobrazí snímek, než se prezentace spustí na zvolené straně. Nastavení na nulu automatickou změnu stránek zakáže.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1219"/>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Metoda komprese pro obrázky. Automatická volba umožní vybrat nejlepší metodu. ZIP je bezztrátový způsob a je dobrý pro obrázky v plných barvách. JPEG je lepší pro vytváření malých PDF souborů s mnoha fotografiemi (s mírnou ztrátou kvality). Ponechejte Automaticky, pokud nemáte na kompresi speciální požadavky.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1220"/>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Úrovně kvality pro metody ztrátové komprese: Minimum (25 %), Nízká (50 %), Střední (75 %), Vysoká (85 %), Maximum (95 %). Uvědomte si, prosím, že úroveň kvality nemá jednoznačný vliv na datovou velikost výsledného obrázku - výsledná velikost a ztráta kvality se liší obrázek od obrázku, a to u každé úrovně komprese. I když vyberete Maximum, u JPEG vždy ke ztrátě dochází.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="484"/>
        <source>&amp;Embed All</source>
        <translation>&amp;Vložit vše</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="499"/>
        <source>Fonts to outline:</source>
        <translation>Písma pro obrys:</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="504"/>
        <source>Outline &amp;All</source>
        <translation>&amp;Obrys pro vše</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="686"/>
        <source>Document Layout</source>
        <translation>Vzhled dokumentu</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1175"/>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>Vložit do PDF dokumentu písma. Vložení písem zachová vzhled dokumentu. Některá písma jako OpenType mohou být pouze podmnožinou, protože je nelze vložit do dokumentů před PDF verzí 1.6.</translation>
    </message>
    <message>
        <location filename="../tabpdfoptions.cpp" line="1176"/>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation>Vloží do PDF všechna písma jako podmnožinu (subset). To znamená, že se vkládají jen použité glyfy, ne celé písmo. Některá písma jako např. OpenType mohou být vložena pouze jako podmnožina, protože je nelze vložit do dokumentů před verzí PDF 1.6.</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <location filename="../tabtools.cpp" line="98"/>
        <source>Font:</source>
        <translation>Písmo:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="510"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="111"/>
        <source>Size:</source>
        <translation>Velikost:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="356"/>
        <source>None</source>
        <translation>Žádný</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="212"/>
        <source>Fill Color:</source>
        <translation>Výplňová barva:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="248"/>
        <source>Stroke Color:</source>
        <translation>Barva obrysu:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="288"/>
        <source>Tab Fill Character:</source>
        <translation>Výplňový znak pro tabulátor:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="294"/>
        <source>Tab Width:</source>
        <translation>Šířka tabulátoru:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="301"/>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="307"/>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="806"/>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="452"/>
        <source>&amp;Line Color:</source>
        <translation>Ba&amp;rva čáry:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="662"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="459"/>
        <source>&amp;Shading:</source>
        <translation>&amp;Stín:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="380"/>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Barva výplně:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="596"/>
        <source>S&amp;hading:</source>
        <translation>Stí&amp;n:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="414"/>
        <source>Line Style:</source>
        <translation>Styl čáry:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="513"/>
        <source>Line &amp;Width:</source>
        <translation>Šíř&amp;ka čáry:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="485"/>
        <source>Line S&amp;tyle:</source>
        <translation>S&amp;tyl čáry:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="503"/>
        <source>Arrows:</source>
        <translation>Šipky:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="505"/>
        <source>Start:</source>
        <translation>Začátek:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="507"/>
        <source>End:</source>
        <translation>Konec:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="526"/>
        <source>&amp;Free Scaling</source>
        <translation>Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="535"/>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vodorovné zvětšení:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="543"/>
        <source>&amp;Vertical Scaling:</source>
        <translation>S&amp;vislé zvětšení:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="556"/>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Změnit velikost &amp;obrázku na rozměr rámu</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="560"/>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Dod&amp;ržet poměr stran</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="589"/>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Barva výplně:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="599"/>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="607"/>
        <source>On Screen Preview</source>
        <translation>Náhled na obrazovce</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="611"/>
        <source>Full Resolution Preview</source>
        <translation>Náhled v plném rozlišení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="614"/>
        <source>Normal Resolution Preview</source>
        <translation>Náhled v normálním rozlišení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="617"/>
        <source>Low Resolution Preview</source>
        <translation>Náhled v nízkém rozlišení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="650"/>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="658"/>
        <source>Ma&amp;ximum:</source>
        <translation>&amp;Maximum:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="664"/>
        <source>&amp;Stepping:</source>
        <translation>&amp;Krokování:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="670"/>
        <source>Text Frame Properties</source>
        <translation>Vlastnosti textových rámců</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="671"/>
        <source>Picture Frame Properties</source>
        <translation>Vlastnosti obrázkových rámců</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="672"/>
        <source>Shape Drawing Properties</source>
        <translation>Vlastnosti kreslení obrysů</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="673"/>
        <source>Magnification Level Defaults</source>
        <translation>Vlastnosti úrovně zvětšení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="674"/>
        <source>Line Drawing Properties</source>
        <translation>Vlastnosti čar</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="675"/>
        <source>Polygon Drawing Properties</source>
        <translation>Vlastnosti mnohoúhelníků</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="676"/>
        <source>Font for new text frames</source>
        <translation>Písmo nového textového rámu</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="677"/>
        <source>Size of font for new text frames</source>
        <translation>Velikost písma nového textového rámu</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="678"/>
        <source>Color of font</source>
        <translation>Barva písma</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="679"/>
        <source>Number of columns in a text frame</source>
        <translation>Počet sloupců v textovém rámu</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="680"/>
        <source>Gap between text frame columns</source>
        <translation>Mezera mezi sloupci textového rámu</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="681"/>
        <source>Sample of your font</source>
        <translation>Ukázka písma</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="682"/>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Obrázkové rámce mohou libovolně měnit rozměry obrázku</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="683"/>
        <source>Horizontal scaling of images</source>
        <translation>Vodorovné zvětšení obrázků</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="684"/>
        <source>Vertical scaling of images</source>
        <translation>Svislé zvětšení obrázků</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="685"/>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Dodržet stejné vodorovné a svislé zvětšení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="686"/>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Obrázky budou deformovány podle rozměrů rámce</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="687"/>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automaticky nastavovaná velikost obrázků dodržuje originální rozměry</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="688"/>
        <source>Fill color of picture frames</source>
        <translation>Barva výplně obrázkových rámců</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="693"/>
        <source>Saturation of color of fill</source>
        <translation>Sytost barvy výplně</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="690"/>
        <source>Line color of shapes</source>
        <translation>Barva čar obrysů</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="691"/>
        <source>Saturation of color of lines</source>
        <translation>Sytost barvy čar</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="692"/>
        <source>Fill color of shapes</source>
        <translation>Barva výplně obrysů</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="694"/>
        <source>Line style of shapes</source>
        <translation>Styl čar obrysů</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="695"/>
        <source>Line width of shapes</source>
        <translation>Tloušťka čar obrysů</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="696"/>
        <source>Minimum magnification allowed</source>
        <translation>Minimální povolené zvětšení (zmenšení)</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="697"/>
        <source>Maximum magnification allowed</source>
        <translation>Maximální povolené zvětšení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="698"/>
        <source>Change in magnification for each zoom operation</source>
        <translation>Změna zvětšení - krok operace lupou</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="699"/>
        <source>Color of lines</source>
        <translation>Barva čar</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="700"/>
        <source>Saturation of color</source>
        <translation>Sytost barvy</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="701"/>
        <source>Style of lines</source>
        <translation>Styl čar</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="702"/>
        <source>Width of lines</source>
        <translation>Šířka čár</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="729"/>
        <source>Custom:</source>
        <translation>Vlastní:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="762"/>
        <source>Custom: </source>
        <translation>Vlastní: </translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="140"/>
        <source>Text Color:</source>
        <translation>Barva textu:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="256"/>
        <source>Shading:</source>
        <translation>Stín:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="176"/>
        <source>Text Stroke:</source>
        <translation>Obrys textu:</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="261"/>
        <source>Dot</source>
        <translation>Tečka</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="262"/>
        <source>Hyphen</source>
        <translation>Pomlčka/spojovník</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="263"/>
        <source>Underscore</source>
        <translation>Podtržení</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="264"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../tabtools.cpp" line="260"/>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Žádné</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <location filename="../tabtypography.cpp" line="25"/>
        <source>Subscript</source>
        <translation>Dolní index</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="140"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="32"/>
        <source>&amp;Displacement:</source>
        <translation>&amp;Posunutí:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="39"/>
        <source>&amp;Scaling:</source>
        <translation>Z&amp;většení:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="46"/>
        <source>Superscript</source>
        <translation>Horní index</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="53"/>
        <source>D&amp;isplacement:</source>
        <translation>Po&amp;sunutí:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="60"/>
        <source>S&amp;caling:</source>
        <translation>Zvě&amp;tšení:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="65"/>
        <source>Underline</source>
        <translation>Podtržené</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="97"/>
        <source>Displacement:</source>
        <translation>Posunutí:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="110"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="105"/>
        <source>Line Width:</source>
        <translation>Šířka čáry:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="90"/>
        <source>Strikethru</source>
        <translation>Přeškrtnuté výš</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="118"/>
        <source>Small Caps</source>
        <translation>Kapitálky</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="126"/>
        <source>Sc&amp;aling:</source>
        <translation>Zvětše&amp;ní:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="134"/>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatic&amp;ké řádkování</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="142"/>
        <source>Line Spacing:</source>
        <translation>Řádkování:</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="146"/>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Posunutí nad účaří</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="147"/>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativní velikost horního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="148"/>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Posunutí pod účaří</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="149"/>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativní velikost dolního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="150"/>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativní velikost kapitálek vůči normální velikosti písma</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="151"/>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Procentuální zvětšení řádkování podle velikosti písma</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="152"/>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Posunutí pod běžné účaří vyjádřené jako procento spodního dotahu znaku</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="155"/>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Šířka čáry vyjádřená jako procento velikosti písma</translation>
    </message>
    <message>
        <location filename="../tabtypography.cpp" line="154"/>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Posunutí nad běžné účaří vyjádřené jako procento horního dotahu znaku</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <location filename="../tabruler.cpp" line="458"/>
        <source>Left</source>
        <translation>Vlevo</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="459"/>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <location filename="" line="7471221"/>
        <source>Full Stop</source>
        <translation type="obsolete">Tečka</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="461"/>
        <source>Comma</source>
        <translation>Čárka</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="462"/>
        <source>Center</source>
        <translation>Na střed</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="509"/>
        <source>Delete All</source>
        <translation>Smazat vše</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="545"/>
        <source>Indentation for first line of the paragraph</source>
        <translation>Odsazení prvního řádku odstavce</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="546"/>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Odsazení celého odstavce zleva</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="548"/>
        <source>Delete all Tabulators</source>
        <translation>Smazat všechny tabelátory</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="478"/>
        <source>&amp;Position:</source>
        <translation>&amp;Pozice:</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="484"/>
        <source>Dot</source>
        <translation>Tečka</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="485"/>
        <source>Hyphen</source>
        <translation>Spojovník</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="486"/>
        <source>Underscore</source>
        <translation>Podtržení</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="487"/>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="488"/>
        <source>Fill Char:</source>
        <translation>Výplňový znak:</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="642"/>
        <source>Custom:</source>
        <translation>Vlastní:</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="678"/>
        <source>Custom: </source>
        <translation>Vlastní: </translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="460"/>
        <source>Period</source>
        <translation>Perioda</translation>
    </message>
    <message>
        <location filename="../tabruler.cpp" line="483"/>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Žádné</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <location filename="../tree.cpp" line="823"/>
        <source>Outline</source>
        <translation>Obrys</translation>
    </message>
    <message>
        <location filename="../tree.cpp" line="824"/>
        <source>Element</source>
        <translation>Prvek</translation>
    </message>
    <message>
        <location filename="../tree.cpp" line="746"/>
        <source>Group </source>
        <translation>Seskupit</translation>
    </message>
    <message>
        <location filename="../tree.cpp" line="703"/>
        <source>Free Objects</source>
        <translation>Volné objekty</translation>
    </message>
    <message>
        <location filename="../tree.cpp" line="655"/>
        <source>Page </source>
        <translation>Strana</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <location filename="../styleselect.cpp" line="79"/>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="90"/>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="89"/>
        <source>Displacement</source>
        <translation>Posunití</translation>
    </message>
    <message>
        <location filename="../styleselect.cpp" line="91"/>
        <source>Linewidth</source>
        <translation>Šířka čáry</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <location filename="../undomanager.cpp" line="705"/>
        <source>Add vertical guide</source>
        <translation>Přidat svislé vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="706"/>
        <source>Add horizontal guide</source>
        <translation>Přidat vodorovné vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="707"/>
        <source>Remove vertical guide</source>
        <translation>Odebrat svislé vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="708"/>
        <source>Remove horizontal guide</source>
        <translation>Odebrat vodorovné vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="709"/>
        <source>Move vertical guide</source>
        <translation>Přesunout svislé vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="710"/>
        <source>Move horizontal guide</source>
        <translation>Přesunout vorodovné vodítko</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="711"/>
        <source>Lock guides</source>
        <translation>Zamknout vodítka</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="712"/>
        <source>Unlock guides</source>
        <translation>Odemknout vodítka</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="713"/>
        <source>Move</source>
        <translation>Přesunout</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="714"/>
        <source>Resize</source>
        <translation>Změnit velikost</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="715"/>
        <source>Rotate</source>
        <translation>Rotovat</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="716"/>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="717"/>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation>W1: %1, H1: %2
W2: %3, H2: %4</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="722"/>
        <source>Selection</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="723"/>
        <source>Group</source>
        <translation>Seskupit</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="724"/>
        <source>Selection/Group</source>
        <translation>Výběr/seskupení</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="725"/>
        <source>Create</source>
        <translation>Vytvořit</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="726"/>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
Š: %3, V: %4</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="727"/>
        <source>Align/Distribute</source>
        <translation>Zarovnat/rozmístit</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="728"/>
        <source>Items involved</source>
        <translation>Zahrnuté objekty</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="729"/>
        <source>Cancel</source>
        <translation>Zrušit</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="730"/>
        <source>Set fill color</source>
        <translation>Nastavit barvu výplně</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="731"/>
        <source>Color1: %1, Color2: %2</source>
        <translation>Barva1: %1, Barva2: %2</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="732"/>
        <source>Set fill color shade</source>
        <translation>Nastavit stín barvy výplně</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="733"/>
        <source>Set line color</source>
        <translation>Nastavit barvu čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="734"/>
        <source>Set line color shade</source>
        <translation>Nastavit stín barvy čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="735"/>
        <source>Flip horizontally</source>
        <translation>Překlopit vodorovně</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="736"/>
        <source>Flip vertically</source>
        <translation>Překlopit svisle</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="737"/>
        <source>Lock</source>
        <translation>Zamknout</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="738"/>
        <source>Unlock</source>
        <translation>Odemknout</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="739"/>
        <source>Lock size</source>
        <translation>Zamknout velikost</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="740"/>
        <source>Unlock size</source>
        <translation>Odemknout velikost</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="743"/>
        <source>Ungroup</source>
        <translation>Zrušit seskupení</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="744"/>
        <source>Delete</source>
        <translation>Smazat</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="745"/>
        <source>Rename</source>
        <translation>Přejmenovat</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="746"/>
        <source>From %1
to %2</source>
        <translation>Od %1
po %2</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="747"/>
        <source>Apply Master Page</source>
        <translation>Použít vzorovou stranu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="748"/>
        <source>Paste</source>
        <translation>Vložit</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="749"/>
        <source>Cut</source>
        <translation>Vyjmout</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="750"/>
        <source>Set fill color transparency</source>
        <translation>Nastavit průhlednost barvy výplně</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="751"/>
        <source>Set line color transparency</source>
        <translation>Nastavit průhlednost barvy čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="752"/>
        <source>Set line style</source>
        <translation>Nastavit styl čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="753"/>
        <source>Set the style of line end</source>
        <translation>Nastavit styl konce čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="754"/>
        <source>Set the style of line join</source>
        <translation>Nastavit styl spoje čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="755"/>
        <source>Set line width</source>
        <translation>Nastavit šířku čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="756"/>
        <source>No style</source>
        <translation>Bez stylu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="757"/>
        <source>Set custom line style</source>
        <translation>Nastavit uživatelský styl čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="758"/>
        <source>Do not use custom line style</source>
        <translation>Nepoužívat uživatelský styl čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="759"/>
        <source>Set start arrow</source>
        <translation>Nastavit šipku pro začátek</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="760"/>
        <source>Set end arrow</source>
        <translation>Nastavit šipku pro konec</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="761"/>
        <source>Create table</source>
        <translation>Vytvořit tabulku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="762"/>
        <source>Rows: %1, Cols: %2</source>
        <translation>Řádky: %1, Sloupce: %2</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="763"/>
        <source>Set font</source>
        <translation>Nastavit písmo</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="764"/>
        <source>Set font size</source>
        <translation>Nastavit velikost písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="765"/>
        <source>Set font width</source>
        <translation>Nastavit šířku písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="766"/>
        <source>Set font height</source>
        <translation>Nastavit výšku písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="767"/>
        <source>Set font fill color</source>
        <translation>Nastavit barvu výplně písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="768"/>
        <source>Set font stroke color</source>
        <translation>Nastavit barvu obrysu písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="769"/>
        <source>Set font fill color shade</source>
        <translation>Nastavit stín barvy výplně písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="770"/>
        <source>Set font stroke color shade</source>
        <translation>Nastavit odstín barvy obrysu písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="771"/>
        <source>Set kerning</source>
        <translation>Nastavit kerning</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="772"/>
        <source>Set line spacing</source>
        <translation>Nastavit řádkování</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="773"/>
        <source>Set paragraph style</source>
        <translation>Nastavit styl odstavce</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="774"/>
        <source>Set language</source>
        <translation>Nastavit jazyk</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="775"/>
        <source>Align text</source>
        <translation>Zarovnání textu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="776"/>
        <source>Set font effect</source>
        <translation>Nastavit efekt písma</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="777"/>
        <source>Image frame</source>
        <translation>Obrázkový rámec</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="778"/>
        <source>Text frame</source>
        <translation>Textový rám</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="779"/>
        <source>Polygon</source>
        <translation>Mnohoúhelník</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="780"/>
        <source>Bezier curve</source>
        <translation>Beziérova křivka</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="781"/>
        <source>Polyline</source>
        <translation>Lomená čára</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="783"/>
        <source>Convert to</source>
        <translation>Konverze na</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="784"/>
        <source>Import SVG image</source>
        <translation>Import SVG obrázku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="785"/>
        <source>Import EPS image</source>
        <translation>Import EPS obrázku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="786"/>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Import souboru OpenOffice.org Draw</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="787"/>
        <source>Scratch space</source>
        <translation>Pracovní prostor</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="788"/>
        <source>Text flows around the frame</source>
        <translation>Text obtéká okolo rámu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="789"/>
        <source>Text flows around bounding box</source>
        <translation>Text obtéká kolem bounding boxu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="790"/>
        <source>Text flows around contour line</source>
        <translation>Text obtéká kolem obrysové čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="791"/>
        <source>No text flow</source>
        <translation>Text neobtéká</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="792"/>
        <source>No bounding box</source>
        <translation>Bez bounding boxu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="793"/>
        <source>No contour line</source>
        <translation>Bez obrysové čáry</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="794"/>
        <source>Page %1</source>
        <translation>Strana %1</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="795"/>
        <source>Set image scaling</source>
        <translation>Nastavit škálování obrázku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="796"/>
        <source>Frame size</source>
        <translation>Velikost rámu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="797"/>
        <source>Free scaling</source>
        <translation>Volná změna velikosti</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="798"/>
        <source>Keep aspect ratio</source>
        <translation>Dodržet poměr stran</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="799"/>
        <source>Break aspect ratio</source>
        <translation>Rozvázat poměr stran</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="818"/>
        <source>Edit contour line</source>
        <translation>Upravit obrysovou čáru</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="801"/>
        <source>Edit shape</source>
        <translation>Upravit obrys</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="802"/>
        <source>Reset contour line</source>
        <translation>Znovu nastavit obrysovou čáru</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="803"/>
        <source>Add page</source>
        <translation>Přidat stranu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="804"/>
        <source>Add pages</source>
        <translation>Přidat strany</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="805"/>
        <source>Delete page</source>
        <translation>Smazat stranu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="806"/>
        <source>Delete pages</source>
        <translation>Smazat strany</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="807"/>
        <source>Add layer</source>
        <translation>Přidat vrstvu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="808"/>
        <source>Delete layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="809"/>
        <source>Rename layer</source>
        <translation>Přejmenovat vrstvu</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="810"/>
        <source>Raise layer</source>
        <translation>Vrstvu nahoru</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="811"/>
        <source>Lower layer</source>
        <translation>Vrstvu dolů</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="812"/>
        <source>Send to layer</source>
        <translation>Přesunout do vrstvy</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="813"/>
        <source>Enable printing of layer</source>
        <translation>Povolit tisk vrstvy</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="814"/>
        <source>Disable printing of layer</source>
        <translation>Zakázat tisk vrstvy</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="815"/>
        <source>Change name of the layer</source>
        <translation>Změnit název vrstvy</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="816"/>
        <source>Get image</source>
        <translation>Vložit obrázek</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="782"/>
        <source>Text on a Path</source>
        <translation>Text na křivky</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="741"/>
        <source>Enable Item Printing</source>
        <translation>Tisk objektu povolen</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="742"/>
        <source>Disable Item Printing</source>
        <translation>Tisk objektu zakázán</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="817"/>
        <source>Multiple duplicate</source>
        <translation>Vícenásobné duplikování</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="718"/>
        <source>Change Image Offset</source>
        <translation>Změnit offset obrázku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="719"/>
        <source>Change Image Scale</source>
        <translation>Změnit měřítko obrázku</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="720"/>
        <source>X1: %1, Y1: %2
X2: %4, Y2: %5</source>
        <translation>X1: %1, Y1: %2
X2: %4, Y2: %5</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="721"/>
        <source>X: %1, Y: %2
X: %4, Y: %5</source>
        <translation>X: %1, Y: %2
X: %4, Y: %5</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="819"/>
        <source>Reset control point</source>
        <translation>Vynulovat řídicí bod</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="820"/>
        <source>Reset control points</source>
        <translation>Vynulovat řídicí body</translation>
    </message>
    <message>
        <location filename="../undomanager.cpp" line="821"/>
        <source>Modify image effects</source>
        <translation>Upravit obrázkové efekty</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <location filename="../undogui.cpp" line="287"/>
        <source>Initial State</source>
        <translation>Původní stav</translation>
    </message>
    <message>
        <location filename="../undogui.cpp" line="300"/>
        <source>Action History</source>
        <translation>Historie akcí</translation>
    </message>
    <message>
        <location filename="../undogui.cpp" line="301"/>
        <source>Show selected object only</source>
        <translation>Ukázat pouze vybrané objekty</translation>
    </message>
    <message>
        <location filename="../undogui.cpp" line="302"/>
        <source>&amp;Undo</source>
        <translation>&amp;Zpět</translation>
    </message>
    <message>
        <location filename="../undogui.cpp" line="303"/>
        <source>&amp;Redo</source>
        <translation>&amp;Vpřed</translation>
    </message>
</context>
<context>
    <name>UndoWidget</name>
    <message>
        <location filename="../undogui.cpp" line="163"/>
        <source>%1: %2</source>
        <comment>undo target: action (f.e. Text frame: Resize)</comment>
        <translation>%1: %2</translation>
    </message>
</context>
<context>
    <name>UpgradeChecker</name>
    <message>
        <location filename="../upgradechecker.cpp" line="83"/>
        <source>Attempting to get the Scribus version update file</source>
        <translation>Pokouším se získat aktualizační soubor Scribusu</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="84"/>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(Z vašeho počítače nebudou odeslána žádná data)</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="104"/>
        <source>Timed out when attempting to get update file.</source>
        <translation>Při pokusu o získání aktualizačního souboru vypršel čas.</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="109"/>
        <source>Error when attempting to get update file: %1</source>
        <translation>Chyba při pokusu o stažení souboru s aktualizací %1</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="139"/>
        <source>File not found on server</source>
        <translation>Soubor nebyl na serveru nalezen</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="141"/>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>Nepodařilo se otevřít soubor s verzí %1 
Chyba %2 na řádce %3, sloupci %4</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="204"/>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>Při hledání aktualizací Scribusu došlo k chybě. Zkontrolujte prosím své připojení k Internetu.</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="208"/>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>Pro vaši verzi Scribusu (%1) nejsou k dispozici žádné aktualizace:</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="211"/>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>Pro vaši verzi Scribusu (%1) je k dispozici jedna nebo více aktualizací:</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="214"/>
        <source>This list may contain development versions.</source>
        <translation>Tento seznam může obsahovat i vývojové verze.</translation>
    </message>
    <message>
        <location filename="../upgradechecker.cpp" line="215"/>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Podrobnosti najdete na www.scribus.net.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <location filename="../useprintermarginsdialog.cpp" line="44"/>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Minimální okraje pro velikost strany %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="16"/>
        <source>Use Printer Margins</source>
        <translation>Použít okraje tiskárny</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="35"/>
        <source>Select &amp;Printer:</source>
        <translation>Vybrat &amp;tiskárnu:</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="61"/>
        <source>Margins</source>
        <translation>Okraje</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="88"/>
        <source>Right:</source>
        <translation>Pravý:</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="132"/>
        <source>&amp;Top:</source>
        <translation>&amp;Horní:</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="143"/>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dolní:</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="154"/>
        <source>&amp;Left:</source>
        <translation>&amp;Levý:</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="219"/>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="222"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="230"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Zrušit</translation>
    </message>
    <message>
        <location filename="../useprintermarginsdialogbase.ui" line="233"/>
        <source>Alt+C</source>
        <translation>Alt+Z</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <location filename="../plugins/scriptplugin/valuedialog.cpp" line="70"/>
        <source>Insert value</source>
        <translation>Vložte hodnotu</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/valuedialog.cpp" line="71"/>
        <source>Enter a value then press OK.</source>
        <translation>Vložte hodnotu a potom stiskněte OK.</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/valuedialog.cpp" line="72"/>
        <source>Enter a value then press OK</source>
        <translation>Vložte hodnotu a potom stiskněte OK</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/valuedialog.cpp" line="74"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../plugins/scriptplugin/valuedialog.cpp" line="75"/>
        <source>Send your value to the script</source>
        <translation>Předá vaši hodnotu skriptu</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <location filename="../werktoolb.cpp" line="41"/>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="119"/>
        <source>Properties...</source>
        <translation>Vlastnosti...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <location filename="../werktoolb.cpp" line="193"/>
        <source>Button</source>
        <translation>Tlačítko</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="193"/>
        <source>Text Field</source>
        <translation>Textové pole</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="193"/>
        <source>Check Box</source>
        <translation>Políčko k zaškrtnuti</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="193"/>
        <source>Combo Box</source>
        <translation>Výběr</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="193"/>
        <source>List Box</source>
        <translation>Seznam</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="201"/>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="202"/>
        <source>Link</source>
        <translation>Spojení</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="124"/>
        <source>PDF Tools</source>
        <translation>PDF nástroje</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="198"/>
        <source>Insert PDF Fields</source>
        <translation>Vložit PDF pole</translation>
    </message>
    <message>
        <location filename="../werktoolb.cpp" line="199"/>
        <source>Insert PDF Annotations</source>
        <translation>Vložit PDF anotace</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <location filename="../gtdialogs.cpp" line="61"/>
        <source>Choose the importer to use</source>
        <translation>Vyberte požadovaný import</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="62"/>
        <source>Automatic</source>
        <translation>Automaticky</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="68"/>
        <source>Import text without any formatting</source>
        <translation>Importovat text bez jakéhokoli formátování</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="75"/>
        <source>Importer:</source>
        <translation>Importer:</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="111"/>
        <source>Encoding:</source>
        <translation>Kódování:</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="67"/>
        <source>Import Text Only</source>
        <translation>Importovat pouze text</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="45"/>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <location filename="../gtdialogs.cpp" line="148"/>
        <source>Choose the importer to use</source>
        <translation>Vyberte požadovaný import</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="158"/>
        <source>Remember association</source>
        <translation>Zapamatovat si asociaci</translation>
    </message>
    <message>
        <location filename="../gtdialogs.cpp" line="160"/>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Zapamatovat si přířazení přípony souboru k importní aplikaci a příště se již nedotazovat, který program použít pro soubor tohoto typu.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="22"/>
        <source>New From Template</source>
        <translation>Nový ze šablony</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="164"/>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="220"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="222"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="224"/>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="226"/>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="228"/>
        <source>Usage</source>
        <translation>Použití</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="230"/>
        <source>Created with</source>
        <translation>Vytvořeno v</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="234"/>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="64"/>
        <source>&amp;Remove</source>
        <translation>Odst&amp;ranit</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="65"/>
        <source>&amp;Open</source>
        <translation>&amp;Otevřít</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="328"/>
        <source>Downloading Templates</source>
        <translation>Získat nové šablony</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="334"/>
        <source>Installing Templates</source>
        <translation>Instalace šablon</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="340"/>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Rozbalte archiv do adresáře šablon &lt;pre&gt;~/.scribus/templates&lt;/pre&gt; odkud budou přístupné pouze vám nebo do &lt;pre&gt;PREFIX/share/scribus/templates&lt;/pre&gt; odkud je uvidí všichni uživatelé.</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="343"/>
        <source>Preparing a template</source>
        <translation>Příprava šablony</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="348"/>
        <source>Removing a template</source>
        <translation>Odstranění šablony</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="353"/>
        <source>Translating template.xml</source>
        <translation>Překlad template.xml</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="232"/>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="331"/>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Šablony získáte na &lt;a href=&quot;http://www.scribus.net/&quot;&gt;www.scribus.net&lt;/a&gt; v sekci Download.</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="345"/>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Ujistěte se, že použité obrázky mohou být použity všude. Také písma musí být zkontrolována. Jestliže písma nesmíte distribuovat, nevkládejte je do šablony. </translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="346"/>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Autor šablony by se měl také ujistit, že se jeho šablona korektně nainstaluje, což mimo jiné znamená, že se správně zachová v cizím systému.</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="350"/>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Jestliže odstraníte šablonu z aplikace, odstraníte pouze záznam z template.xml. Soubory zůstanou na disku. Menu s mazací položkou se zobrazí pouze tehdy, jestliže máte právo měnit soubor template.xml.</translation>
    </message>
    <message>
        <location filename="../plugins/newfromtemplateplugin/nftdialog.cpp" line="355"/>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Zkopírujte existující template.xml a přejmenujte kopii na template.lang.xml (použijte stejný kód, jaký je ve jménu QM souboru jazyka). Např. template.cs.xml bude použito v Českém prostředí. Soubor musí být ve stejném adresáři jako původní.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="20"/>
        <source>Save as Template</source>
        <translation>Uložit jako šablonu</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="45"/>
        <source>Name</source>
        <translation>Jméno</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="51"/>
        <source>Category</source>
        <translation>Kategorie</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="57"/>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="63"/>
        <source>Colors</source>
        <translation>Barvy</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="68"/>
        <source>Description</source>
        <translation>Popis</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="73"/>
        <source>Usage</source>
        <translation>Použití</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="78"/>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="84"/>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="161"/>
        <source>More Details</source>
        <translation>Více detailů</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="96"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="125"/>
        <source>Less Details</source>
        <translation>Méně detailů</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="260"/>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="261"/>
        <source>Letter</source>
        <translation>Letter</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="261"/>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="268"/>
        <source>landscape</source>
        <translation>na šířku</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="273"/>
        <source>portrait</source>
        <translation>na výšku</translation>
    </message>
    <message>
        <location filename="../plugins/saveastemplateplugin/satdialog.cpp" line="289"/>
        <source>custom</source>
        <translation>vlastní</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="25"/>
        <source>Create filter</source>
        <translation>Vytvořit filtr</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="45"/>
        <source>C&amp;lear</source>
        <translation>&amp;Vyčistit</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="48"/>
        <source>&amp;Delete</source>
        <translation>&amp;Smazat</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="55"/>
        <source>Choose a previously saved filter</source>
        <translation>Zvolit předchozí uložený filtr</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="226"/>
        <source>Give a name to this filter for saving</source>
        <translation>Zadejte jméno filtru</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tfdia.cpp" line="100"/>
        <source>Give a name for saving</source>
        <translation>Zadejte jméno filtru</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="102"/>
        <source>Disable or enable this filter row</source>
        <translation>Povolit nebo zakázat tuto část filtru</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="124"/>
        <source>Remove this filter row</source>
        <translation>Odstranit tuto část filtru</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="129"/>
        <source>Add a new filter row</source>
        <translation>Přidat novou část filtru</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="382"/>
        <source>to</source>
        <translation>na</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="198"/>
        <source>and</source>
        <translation>a</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="202"/>
        <source>remove match</source>
        <translation>odstranit vzor</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="203"/>
        <source>do not remove match</source>
        <translation>neodstraňovat vzor</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="213"/>
        <source>words</source>
        <translation>slovy</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="252"/>
        <source>Remove</source>
        <translation>Odstranit</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="253"/>
        <source>Replace</source>
        <translation>Nahradit</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="254"/>
        <source>Apply</source>
        <translation>Použít</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="411"/>
        <source>Value at the left is a regular expression</source>
        <translation>Hodnota vlevo je regulární výraz</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="287"/>
        <source>with</source>
        <translation>čím</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="299"/>
        <source>paragraph style</source>
        <translation>styl odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="303"/>
        <source>all instances of</source>
        <translation>všechny výskyty</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="385"/>
        <source>all paragraphs</source>
        <translation>všechny odstavce</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="386"/>
        <source>paragraphs starting with</source>
        <translation>odstavec začíná</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="387"/>
        <source>paragraphs with less than</source>
        <translation>odstavec s méně než</translation>
    </message>
    <message>
        <location filename="../plugins/gettext/textfilter/tffilter.cpp" line="388"/>
        <source>paragraphs with more than</source>
        <translation>odstavec s více než</translation>
    </message>
</context>
</TS>
