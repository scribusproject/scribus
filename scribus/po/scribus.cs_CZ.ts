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
jsou vyjádřeny v délkových jednotkách dokumentu (viz. konstanty UNIT).
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

Vytvoří nový čtyřúhelník na aktuální stránce a vráti jeho jméno. X, Y, W a H koordináty
jsou dány ve zvolených měrných jednotkách dokumentu (viz. konstanty UNIT*).
&quot;name&quot; musí být unikátní řetězec, protože slouží jako identifikátor. Jestliže není
&quot;name&quot; zadáno, Scribus jméno vytvoří sám.

Může vyvolat výjimku NameExistsError, když se pokusíte zduplikovat jméno.</translation>
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

Vytvoří novou stránku. Jestliže je &quot;where&quot; -1, pak je nová stránka přidána na
konec dokumentu, jinak je vsazena před stránku &quot;where&quot;. Číslování stran je od 1,
a nezáleží na tom, jaké je číslo první stránky. Volitelný parametr &quot;template&quot; udává
jméno šablony, která bude aplikována na novou stránku.

Může vyvolat výjimku IndexError, když se pokusíte založit stránku na špatném místě</translation>
    </message>
    <message>
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
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Zapíše řetězec &quot;string&quot; do stavového řádku Scribusu. Text musí být kódován
UTF8 nebo unicode (např. unicode(&quot;text&quot;, &quot;iso-8859-2&quot;)).</translation>
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
        <translation type="obsolete">newDoc(size, margins, orientation, firstPageNumber, unit, facingPages, firstSideLeft) -&gt; bool

Vytvoří nový dokument a vrátí true v případě úspěchu. Parametry mají tento význam:
    size - Tuple (šířka, výška) rozměrů velikost stránky. Viz. konstanty PAPER_&lt;typ&gt; např. PAPER_A4.
    margins - Tuple (L, P, N, D) popisuje velikost okrajů stránky.
    orientation - Orienrace stránky. Viz. předdefinované konstanty PORTRAIT a LANDSCAPE.
    firstPageNumber - Číslo první stránky dokumentu, které je použito při automatickém číslování stran.
    unit - Nastaví typ měrných jednotek dokumentu. Viz. UNIT_INCHES, UNIT_MILLIMETERS,
             UNIT_PICAS, UNIT_POINTS.
    facingPages - FACINGPAGES, NOFACINGPAGES
    firstSideLeft - FIRSTPAGELEFT, FIRSTPAGERIGHT

Hodnoty výšky, šířky a okrajů jsou uvedeny ve specifikovaných měrných jednotkách dokumentu.
Konstanty PAPER_* jsou v bodech. Jestliže je dokument v jiných jednotkách, musíte provést
patřičnou konverzi.

příklad: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS, FACINGPAGES, FIRSTPAGERIGHT)</translation>
    </message>
    <message>
        <source>register_macro_callable(name, callable, accel=&apos;&apos;)

Create a macro called &quot;name&quot; with the existing callable object &quot;callable&quot;.
The passed callable must not require any arguments when called (it may take
optional arguments, but will not be given any).
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
If the passed callable is a class, it will be rejected. Functions and bound
methods are quite acceptable, as are instances of classes that provide a
__call__ method with no arguments. There is no problem with registering
a callable more than once, nor with registering multiple bound methods
of a single instance.
</source>
        <translation type="obsolete">register_macro_callable(name, callable, accel=&apos;&apos;)

Create a macro called &quot;name&quot; with the existing callable object &quot;callable&quot;.
The passed callable must not require any arguments when called (it may take
optional arguments, but will not be given any).
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
If the passed callable is a class, it will be rejected. Functions and bound
methods are quite acceptable, as are instances of classes that provide a
__call__ method with no arguments. There is no problem with registering
a callable more than once, nor with registering multiple bound methods
of a single instance.</translation>
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

Vrátí jméno písma texttového rámce &quot;name&quot;. Jestliže je v rámu vybraný
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
kódování - použijte např. unicode(text, &apos;iso-8859-2&apos;). Viz. FAQ.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame. Text
must be UTF encoded (see setText() as reference) The first character has an
index of 0. &quot;name&quot; If &quot;name&quot; is not given the currently selected Item is
used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="obsolete">insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Vloží text &quot;text&quot; na pozici &quot;pos&quot; v textovém rámce &quot;name&quot;. Text musí být
v UTF8 kódování (viz. setText()). První znak rámce má pozici 0.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.

Může vyvolat výjimku IndexError, jestliže je &quot;pos&quot; mimo obsah rámce.</translation>
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
        <translation type="unfinished">selectText(start, count, [&quot;name&quot;])

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

Nastaví obrys textu na barvu &quot;color&quot; v textovém rámci &quot;name&quot;.
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

Převede textový rámec &quot;name&quot; na křivky.
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
        <source>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; string with filename

Shows a File Open dialog box with the caption &quot;caption&quot;. Files are filtered
with the filter string &quot;filter&quot;. A default filename or file path can also
supplied, leave this string empty when you don&apos;t want to use it.  A value of
True for haspreview enables a small preview widget in the FileSelect box.  When
the issave parameter is set to True the dialog acts like a &quot;Save As&quot; dialog
otherwise it acts like a &quot;File Open Dialog&quot;. The default for both of the
opional parameters is False.

The filter, if specified, takes the form &apos;comment (*.type *.type2 ...)&apos;.
For example &apos;Images (*.png *.xpm *.jpg)&apos;.

Refer to the Qt-Documentation for QFileDialog for details on filters.

Example: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Example: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)
</source>
        <translation type="obsolete">fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; string with filename

Zobrazí souborové okno/dialog s titulkem &quot;caption&quot;. Soubory mohou být vybrány
podle masky &quot;filter&quot;. Parametr &quot;defaultname&quot; může být použit jako předem nastavená
implicitní cesta. Jestliže je &quot;haspreview&quot; nastaveno na true, bude mít uřivatel
k dispozici náhled vybraných souborů. Jestliže je parametr &quot;issave&quot; nastaven
na true, bude se dialog chovat jako klasický &quot;Uložit jako&quot;.
Maska filtrování souborů může vypadat např. takto: &apos;Obrázky (*.png *.jpg)&apos;.
Více informací neleznete v dokumentaci Qt.
Příklad: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Nebo: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)</translation>
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
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Nastaví informace o dokumentu. Parametry jsou textové řetězce.</translation>
    </message>
    <message>
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
levou stranou nastavte firstPageLeft na FIRSTPAGELEFT, jinak na FIRSTPAGERIGHT.</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Vrátí jméno barvy čar objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

Vrátí tloušťku čáry objektu &quot;name&quot;.
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

Vrátí typ spojení čar objektu &quot;name&quot;.
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

Vrátí typ ukončení čáry objektu &quot;name&quot;.
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

Vrátí styl čáry objektu &quot;name&quot;.
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
Velikost je vyjádřena v aktuálních měrných jednotkách (viz. UNIT_typ).</translation>
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
měrných jednotkách dokumentu (viz. konstanty UNIT).
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
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation type="obsolete">rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Vytvoří obrázkovou bitmapu s náhledem písma &quot;name&quot; s textem &quot;sample&quot;
velikosti &quot;size&quot;. Obrázek je pak uložen jako &quot;filename&quot;. Vrátí true v případě
úspěchu.

Může vyvolat výjimky NotFoundError (nenajde písmo) a ValueError (prázdné
filename nebo sample).</translation>
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
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Vrátí příznak, zda jde vrstva &quot;layer&quot; viditelná nebo není. true znamená
viditelná.

Může vyvolat výjimku NotFoundError (vrstva nenalezena) a
ValueError (nelze přepnout vrstvu).</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Vrátí příznak, zda jde vrstva &quot;layer&quot; tisknutelná nebo není. true znamená
tisknutelná.

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

Vrátí řetězec s kódem jazyka, ve kterém Scribus běží (viz. přepínač --lang xx).</translation>
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
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
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
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

Vytvoří novou čáru na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
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

Vytvoří novou lomenou čáru na aktuální stránce dokumentu a vrátí její jméno.
Koordináty jsou zadávány v současných měrných jednotkách (viz. konstanty UNIT).
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
měrných jednotkách. Viz. konstanty UNIT.</translation>
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
v aktuálních měrných jednotkách (viz. konstanty UNIT).
Např.:
     setHGuides(getHGuides()) + [200.0, 210.0] # prida voditka
     setHGuides([90, 250]) # smaze stara a nastavi nova voditka</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>Viz. getHGuides().</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>Viz. setHGuides().</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Vrátí tuple s rozměry stránky v aktuálních měrných jednotkách. Viz. konstanty UNIT
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
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="obsolete">getPageMargins()

Vrátí nastavení okrajů stránky (levý, pravý, horní, dolní) v aktuálních měrných
jednotkách. Viz. koonstanty UNIT a getPageSize().</translation>
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

Nastaví barvu &quot;color&quot; čar objetu &quot;name&quot;.
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

Nastaví šířku čáry objektu &quot;name&quot; na hodnotu &quot;width&quot;. &quot;width&quot; musí
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

Nastaví stín čar objektu &quot;name&quot; na hodnotu &quot;shade&quot;. &quot;shade&quot; musí
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

Nastaví typ spojení čar objektu &quot;name&quot; na styl &quot;join&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Viz. předdefinované konstanty JOIN_*.</translation>
    </message>
    <message>
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

Nastaví styl čar objektu &quot;name&quot; na definovaný styl &quot;namedStyle&quot;.
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
Po použití P.B. je vhodné jej vynulovat, t.j. použít progressReset(). Viz. dokumentace
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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">defineColor(&quot;name&quot;, c, m, y, k)

Vytvoří novou barvu &quot;name&quot;. Barva je definována čtyřmi složkami modelu CMYK.
Každá složka musí být z intervalu &lt;0; 255&gt;.

Může vyvolat výjimku ValueError, v případě chybného jména barvy.</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">getCornerRadius([&quot;name&quot;]) -&gt; integer

Vrátí poloměr zaoblení rohů objektu &quot;name&quot;. Poloměr je vyjádřen
v bodech. Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="obsolete">getPosition([&quot;name&quot;]) -&gt; (x,y)

Vrátí (x, y) tuple s pozicí objektu &quot;name&quot;.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.
Pozice je vyjádřena v aktuálních měrných jednotkách dokumentu. Viz. konstanty UNIT*.</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">rotateObjectAbs(rot [, &quot;name&quot;])

Nastaví otočení objektu &quot;name&quot; na &quot;rot&quot;. Kladné hodnoty znamenají
otočení po směru hodinových ručiček.
Jestliže není &quot;name&quot; uvedeno, použije vybraný objekt.</translation>
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
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation type="obsolete">selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.</translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation type="obsolete">register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.</translation>
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
        <source>getColorAsRGB(&quot;name&quot;) -&gt; tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">getColorAsRGB(&quot;name&quot;) -&gt; tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.</translation>
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
        <source>isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.</translation>
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
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference). The first character has
an index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot;
is not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="obsolete">insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference). The first character has
an index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot;
is not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.</translation>
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
</context>
<context>
    <name>About</name>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Scribus ver. %1(new line)
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Build=ID:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Příspěvky od:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Windows verze:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Německy:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Francouzsky:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italsky:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Maďarsky:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrajinsky:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bulharsky:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galsky:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turecky:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Litevsky:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polsky:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Česky:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovensky:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Dánsky:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norsky:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Velšsky:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Rusky:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brazilsky:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finsky:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baskitsky:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovinsky:</translation>
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
        <source>English (British):</source>
        <translation type="obsolete">Britská angličtina:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Švédsky:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Webová stránky</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>On-line reference</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Chyby a požadavky na změnu</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Mailová skupina</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Oficiální překlady a překladatelé:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Korejsky:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Srbsky:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Španělsky:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Předchozí přispěvatelé:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katalánsky:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>O Scribusu %1</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Okno ukazuje verzi programu, datum kompilace
a knihovny v něm použité.
Symboly C-C-T znamenají C-littlecms C-CUPS T-TIFF podporu.
Chybějící knihovny jsou zobrazeny jako *</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3 </translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation type="obsolete">Afrikánsky:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation type="obsolete">Portugalština (Braz.):</translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="unfinished"></translation>
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
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Ne</translation>
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
        <translation type="unfinished">&amp;Zavřít</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Otevřít...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Uložit</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">Návrat k u&amp;loženému</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">Ad&amp;resář pro výstup...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Vložit text...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">&amp;Připojit text...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Uložit &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Uložit stranu jako &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">Uložit jako P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">Na&amp;stavení dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Tisk...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">&amp;Konec</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Vpřed</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">V&amp;ymazat obsah</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">Vybr&amp;at vše</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Hledat/Nahradit...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">&amp;Barvy...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">&amp;Styly odstavce...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">Styly ča&amp;r...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Java skripty...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">N&amp;astavení...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Jiný...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">V&amp;levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">Na &amp;střed</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">V&amp;pravo</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">Do &amp;bloku</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished">&amp;Vynucené</translation>
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
        <translation type="unfinished">Po&amp;dtržené</translation>
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
        <translation type="unfinished">&amp;Obrysy</translation>
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
        <translation type="unfinished">&amp;Tabelátory...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">&amp;Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">&amp;Seskupit</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished">&amp;Zrušit seskupení</translation>
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
        <translation type="unfinished">Dát o &amp;vrstvu níž</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">Dát o v&amp;rstvu výš</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">&amp;Poslat do zápisníku</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">Vi&amp;ditelný obrázek</translation>
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
        <translation type="unfinished">Je PDF &amp;záložkou</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">Je PDF &amp;anotací</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">Vlast&amp;nosti anotace</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">Vlastnosti &amp;pole</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">Upravit &amp;obrys...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">Připojit &amp;text ke křivce</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">&amp;Odpojit text od křivky</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">&amp;Kombinovat mnohoúhelníky</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">&amp;Rozdělit mnohoúhelníky</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">&amp;Beziérova křivka</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished">O&amp;brysy</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">&amp;Textový rám</translation>
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
        <translation type="unfinished">&amp;Vložit...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">&amp;Smazat...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Přesunout...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">O&amp;vládání vodítek...</translation>
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
        <translation type="unfinished">M&amp;iniatury</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Zobrazit o&amp;kraje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Zobrazit &amp;rámy</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">Zobrazit &amp;obrázky</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Ukázat &amp;mřížku</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Zobrazit &amp;vodítka</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">&amp;Zobrazit pomocnou mřížku</translation>
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
        <translation type="unfinished">M&amp;agnetická mřížka</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished">&amp;Vlastnosti</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">&amp;Zápisník</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">V&amp;rstvy</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Záložky</translation>
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
        <translation type="unfinished">&amp;Nástroje</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">P&amp;DF nástroje</translation>
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
        <translation type="unfinished">&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">Čá&amp;ra</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Otočit položku</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">Zvětšit nebo zmenšit</translation>
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
        <translation type="unfinished">Upravit obsah rámu</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">Editovat text...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Vytvořit řetězení textu</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Přerušit řetězení textu</translation>
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
        <translation type="unfinished">Editace textu</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Vložit textový rámec</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Vložit tabulku</translation>
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
        <translation type="unfinished">Vložit čáru kreslenou volnou rukou</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">Správa &amp;obrázků</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">&amp;Dělení slov v textu</translation>
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
        <translation type="unfinished">&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">O &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">&amp;Tipy pro nástroje</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">Scribus &amp;manuál...</translation>
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
        <source>Double Turned Comma</source>
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
        <translation type="unfinished">Ukázka pře&amp;d tiskem</translation>
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
        <translation type="unfinished">&amp;Kaskáda</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished">&amp;Dlaždice</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Pokročilé možnosti</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Vytvoří PostScript Level 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Vytviří PostScript Level 2.
Mohou vzniknout velké soubory</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Vytviří PostScript Level 1.
Mohou vzniknout velké soubory</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Zrcadlit stranu(y) &amp;horizontálně</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Zrcadlit stranu(y) &amp;vertikálně</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Aplikovat &amp;ICC profily</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">PostScript Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">PostScript Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">PostScript Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Aplikovat tzv. &amp;Under Color Removal</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">UCR - under color removal - odstranění přebytečné barvy, která by 
způsobila rozpíjení (při přílišné saturaci papíru barvou) nebo trhání 
barvy při několika vrstvém (např. barevném) laserovém tisku.
Viz. např. http://www.typo.cz/_pismo/pis-tech-post.html
a jiné.</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="obsolete">Nastavit velikost média</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="obsolete">Zde můžete nastavit velikost média
(papíru) postscriptového souboru.
Nezadávejte, pokud to nevyžaduje
tiskárna.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Umístit/zarovnat</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Zarovnat</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Vodorovně</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Levé strany</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Na střed</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Pravé strany</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Svisle</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Horní strany</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Spodní strany</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Aplikovat</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Mezi:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">&amp;Zarovnat</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">&amp;Posunutí</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">&amp;Rozmístit rovnoměrně</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">M&amp;ezi:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">&amp;Neměnit</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">&amp;Zarovnat</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">&amp;Posunutí</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">&amp;Rozmístit rovnoměrně</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;Neměnit</translation>
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
        <translation type="unfinished">Zarovnat</translation>
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
        <translation type="unfinished">Strana</translation>
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
        <translation type="unfinished">Varování</translation>
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
        <translation>Tipy pro nástroje:</translation>
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
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Šířka:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tenký</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Široké</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Styl:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Bez přerušení</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Přerušovaný</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podtržené</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Zkosený</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Příloha</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Jiné</translation>
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
        <translation>Text pro tlačítko Dolů</translation>
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
        <translation>Obkreslené</translation>
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
        <translation>Kontrolovat</translation>
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
        <translation>Standardně je označené</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Upravitelné</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">JavaScript</translation>
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
        <translation>Odeslat údaje jako HTML</translation>
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
        <translation>Strana:</translation>
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
        <translation>Stlačení klávesy:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formát</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Hodnota není potvrzena</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Hodnota musí být větší nebo rovná:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>a menší nebo rovná:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Vlastní ověření skriptu:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Vyhodnotit</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Hodnota není vypočítaná</translation>
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
        <translation>Vlastní počítací skript:</translation>
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
        <translation>Potřebujete minimálně ikonu pro Normal, abyste mohl(a) použít ikony pro tlačítka</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Obrázky (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Všechny soubory (*)</translation>
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
        <translation>Vlastnosti poznámky</translation>
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
        <translation>Str&amp;ana:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Poz</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normální</translation>
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
        <translation type="unfinished">Alt+U</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">Všechny str&amp;any</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+V</translation>
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
        <translation type="unfinished">na</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Použít šablonu</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normální</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">Ša&amp;blona:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">&amp;Použít na aktuální stranu</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Použít od &amp;strany:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">po:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Použít na všechny &amp;sudé strany</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Použít na všechny &amp;liché strany</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Zápisníky (*.scd);;Všechny soubory (*)</translation>
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
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Název &quot;%1&quot; není jedinečný.
Vyberte, prosím, jiný.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
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
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
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
        <source>&amp;Small</source>
        <translation>&amp;Malý</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Střední</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Velký</translation>
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
        <translation>Jak změnit:</translation>
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
        <source>Color Management Settings</source>
        <translation type="obsolete">Nastavení správy barev</translation>
    </message>
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
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Implicitní barevný profil na importované obrázky</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Implicitní barevný profil na plné barvy na straně</translation>
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
        <translation>Implicitní účel reprodukce tiskárny. Jestliže víte, proč jej změnit, zvolte
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
        <translation>Výsledek tisku barev bývá rozdílný od toho, jak jsou zobrazeny barvy na monitoru.
Proto jsou důležité kvalitní profily.</translation>
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
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Obrázky:</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <source>OK</source>
        <translation type="obsolete">OK</translation>
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
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Jméno barvy není unikátní</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Nelze vytvořit barvu jménem &quot;%1&quot;.
Název je rezervován pro transparentní barvu</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration Color</source>
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
        <translation type="unfinished">Vybrat znak:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Písmo:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Zavřít</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished">Vložit znak na pozici kurzoru v textu</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Smazat aktuální výběr(y).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished">Zavřete dialog a vraťte se k editaci textu.</translation>
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
        <translation type="unfinished"></translation>
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
        <source>Object is a PDF Annotation or Field</source>
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
        <translation type="unfinished">Strana</translation>
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
        <source>Items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Problems</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Current Profile:</source>
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
        <translation type="obsolete">Postsript</translation>
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
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Vybrat adresář</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varování</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
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
        <translation type="unfinished">Barva</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
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
        <source>Protanopia (red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deuteranopia (green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tritanopia (blue)</source>
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
        <translation type="unfinished">&amp;Barva</translation>
    </message>
    <message>
        <source>Saturation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation component in HSV mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value component in HSV mode</source>
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
        <translation type="unfinished">Nová barva</translation>
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
        <translation type="unfinished">&amp;Aplikovat</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Uložit</translation>
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
        <translation>Vertikální přechod</translation>
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
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Stín:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Barva obrysu</translation>
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
        <source>Color of selected object</source>
        <translation type="obsolete">Barva vybraného objektu</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Volný lineární gradient</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Volný radiální gradient</translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
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
        <translation>Nastavení importu CSV</translation>
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
        <source>None</source>
        <translation>Žádný</translation>
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
        <translation>Nastavení strany</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Všechny strany</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Pouze sudé strany</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Pouze liché strany</translation>
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
        <translation>Strana na arch</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Stran na arch</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Okno zobrazuje různá nastavení CUPS během tisku.
Dostupnost vlastních parametrů závisí na ovladači vaší tiskárny.
Podporu CUPS zjistíte v menu Nápověda &gt; O Scribusu.
Text: C-C-T znamená podporu C-CUPS, C-littlecms a T-TIFF.
Chybějící knihovny jsou zobrazeny jako *</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Smazat barvu</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Nahradit čím:</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Smazat barvu:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Smazat strany</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Smazat od:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>po:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Chybějící písmo</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Písmo %1 není nainstalované.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Použít</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">místo</translation>
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
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Da&amp;lší informace</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Osoba nebo organizace, která je zodpovědná za obsah dokumentu.
Tato položka může být vložena v dokumentu jako reference nebo také jako metadata v PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Název dokumentu.
Tato položka může být vložena v dokumentu jako reference nebo také jako metadata v PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Popis obsahu dokumentu.
Zapište sem abstrakt, ten bude vložen do PDF soubotu při exportu</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Shrnutí obsahu dokumentu klíčovými slovy.
Klíčová slova budou vložena do PDF, podle nich bude PDF soubor indexován a vyhledáván automatizovanými nástroji</translation>
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
        <translation>Datum přiřazený k události v životním cyklu dokumentu. Použijte RRRR-MM-DD formát podle ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Povaha dokumentu. Např. kategorie, finkce atd</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Elektronická reprezentace dokumentu.
Např. RFC2045,RFC2046 t.j. MIME typy</translation>
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
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Jazyk, ve kterém je dokument napsán. Obvykle kód jazyka ISO-639
volitelně s kódem země ISO-3166 např. cs-CZ, en-GB</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Jméno</translation>
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
        <translation type="unfinished">&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+V</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopírovat</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+N</translation>
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
        <translation>Možnosti</translation>
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
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscriptové soubory (*.ps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Azurová (Cyan)</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Purpurová (Magenta)</translation>
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
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Tisk do</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Možnosti...</translation>
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
        <translation>Tisnout &amp;vše</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Ti&amp;sknout aktuální stranu</translation>
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
        <source>Print &amp;Normal</source>
        <translation type="obsolete">&amp;Normální tisk</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Tisk &amp;separací</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Tisknout &amp;barevně, jestliže je to možné</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Tis&amp;knout v odstínech šedi</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">Pok&amp;ročilé možnosti...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Tisk</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Použít alternativní správce tisku, jako jsou např. kprinter nebo gtklp.
Správce tisku nastaví dodatečné volby</translation>
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
        <translation type="unfinished">Strana</translation>
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
        <translation type="unfinished">Nastavit velikost média</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Barva</translation>
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
        <translation type="unfinished">Pokročilé možnosti</translation>
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
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">UCR - under color removal - odstranění přebytečné barvy, která by 
způsobila rozpíjení (při přílišné saturaci papíru barvou) nebo trhání 
barvy při několika vrstvém (např. barevném) laserovém tisku.
Viz. např. http://www.typo.cz/_pismo/pis-tech-post.html
a jiné.</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="obsolete">Editace makra: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Správa maker</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="obsolete">Soubor %1 už existuje.
Opravdu jej chcete přepsat?</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="obsolete">Toto makro už bylo upraveno.
Opravdu chcete zahodit předchozí změny?</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="obsolete">Následuje plná historie:

%1
</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="obsolete">Chyba během kompilace makra. Makro nebude uloženo.
Chyba byla:
%1
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Nové makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;V makro-editoru můžete vytvářet nová makra, upravovat a mazat existující.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="obsolete">Zdrojový kód:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="obsolete">Editace makra:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="obsolete">Jméno editovaného makra.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Jméno aktuálně upravovaného makra.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+Z</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zahodit změny o ukončit editaci.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zahodíte všechny změny. Jestliže nechcete uložit makro, ale nechcete přijít ozměny v kódu, použijte &apos;Uložit jako...&apos;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Uložit změny a ukončit úpravu.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Uložit změny a ukončit úpravy. Jestliže bude makro chybné, editace nekončí.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zde je zdrojový kód makra.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="obsolete">&amp;Nahrát zdrojový kód...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Nahradit dosavadní zdrojový kód obsahem souboru.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Nahradit dosavadní zdrojový kód obsahem souboru.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="obsolete">Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Uložit zdrojový kód do souboru.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="obsolete">Uloží zdrojový kód do textového souboru, který můžete upravovat externími nástroji, a potom nahrát pomocí &apos;Nahrát zdrojový kód...&apos;.</translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation type="obsolete">Python zdrojové kódy (*.py)</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Uložit soubor</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation type="obsolete">Uložit zdroj makra</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Otevřít soubor</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation type="obsolete">Vybrat soubor, který bude nahrán</translation>
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
        <source>Vertical Spaces</source>
        <translation type="obsolete">Vertikální mezery</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Řádkování</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Název stylu není jedinečný</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Efekt:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádné</translation>
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
        <translation>Barva obrysu písma</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Nastaví mnohem větší první písmeno odstavce. Stylistický efekt</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Specifikace velikosti kapitálek podle počtu řádek</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Zarovnat text k pomocné mřížce</translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Písmo:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">&amp;Zarovnání:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">&amp;Iniciály</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>Čá&amp;ry:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Barva &amp;obrysu:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Zarovnat k pomocné &amp;mřížce</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Řád&amp;kování:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">&amp;Nad:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">&amp;Pod:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Vzdálenosti</translation>
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
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Java skripty (*.js);;Všechny soubory (*)</translation>
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
        <translation type="unfinished">Barva:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Stín:</translation>
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
        <translation type="unfinished">Invertovat</translation>
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
        <translation type="unfinished">Zrušit</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>Všechny str&amp;any</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Z&amp;rušit</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Změnit výstupní adresář</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Výstupní adresář - místo, kde budou uloženy vaše obrázky.
Jména exportovaných souborů budou ve tvaru &apos;jmenodokumentu-cislostrany.typ&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Export pouze aktuální strany</translation>
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
        <translation>Možnosti</translation>
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
        <translation>&amp;Aktuální strana</translation>
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
        <translation>Uložit interval stran</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Uložit všechny strany</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Rozlišení obrázků
Použijte 72 dpi, jestliže je obrázek určen na web nebo monitor</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normální</translation>
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
        <translation type="unfinished">Sytost</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Barva</translation>
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
        <translation type="unfinished">Rozpuštění</translation>
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
        <translation type="unfinished">Neprůsvitnost:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Pozadí</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="unfinished">Vrstvy</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Cesty</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Barvy</translation>
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
        <source>Color Sets</source>
        <translation>Množiny barev</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktuálí množina barev:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Vybrat jméno</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nová barva</translation>
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
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Přidat barvy z již existujícího dokumentu</translation>
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
        <source>&amp;Append</source>
        <translation type="obsolete">Připoji&amp;t</translation>
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
        <source>&amp;Remove Unused</source>
        <translation>&amp;Odstranit nepoužité</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Uložit množinu barev</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Jméno:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádná</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Import</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Všeobecné nastavení písma</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Dostupná písma</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Substituce písem</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Dodatečné cesty</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postsript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ano</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Název písma</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Použít písmo</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Obsažen v:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Podmnožina glyphů</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Cesta k souboru s písmem</translation>
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
        <translation>&amp;Substituce písem</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Název písma</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished">Použít písmo</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished">Obsažen v:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Podmnožina glyphů</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished">Cesta k souboru s písmem</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+Z</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Náhled písem</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Přidat zvolené písmo do Styl, Písmo menu</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Zavřít náhled písem</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation type="obsolete">Velikost písma</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Název písma</translation>
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
        <translation type="unfinished">Podmnožina glyphů</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Náhled písem</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Hledat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="unfinished">&amp;Zavřít</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+Z</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Přidat zvolené písmo do Styl, Písmo menu</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Zavřít náhled písem</translation>
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
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation type="unfinished">Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
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
        <source>&amp;Append</source>
        <translation type="unfinished">Připoji&amp;t</translation>
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
        <translation>Pozice:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Zde můžete přidat, změnit nemo odstranit Požadované barvy (Stop-colors).</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Ovládání vodítek</translation>
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
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation type="unfinished">Str&amp;ana</translation>
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
        <translation type="unfinished">&amp;Zavřít</translation>
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
        <translation>Scribus nápověda</translation>
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
        <source>S&amp;earch</source>
        <translation type="obsolete">Hl&amp;edat</translation>
    </message>
    <message>
        <source>Ctrl+F</source>
        <translation type="obsolete">Ctrl+F</translation>
    </message>
    <message>
        <source>F3</source>
        <translation type="obsolete">F3</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>neznámý</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
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
        <translation type="unfinished">&amp;Tisk...</translation>
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
        <source>New Bookmark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Soubor</translation>
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
        <translation type="unfinished">Ú&amp;pravy</translation>
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
        <translation type="unfinished">&amp;Záložky</translation>
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
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Nastavení dělení slov</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Návrhy &amp;dělění</translation>
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
        <translation>&amp;Maximální počet po sobě následujících dělění:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished">Import &amp;EPS/PS...</translation>
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
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Vložit stranu</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Vkládání</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>před stranu</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranu</translation>
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
        <source>Template (Right Page):</source>
        <translation type="obsolete">Šablona (pravá strana):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Vkládání</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Strana(y)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">Šab&amp;lona (levá strana):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">Ša&amp;blona:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Vložit</translation>
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
        <translation type="unfinished">Velikost strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vlastní</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientace:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Na šířku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Výška:</translation>
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
        <translation>Vložit tabulku</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Počet řádků:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Počet sloupců:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
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
        <source>New Script</source>
        <translation>Nový skript</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editace Java skriptů</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
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
        <source>Do you really want to delete this Script?</source>
        <translation>Opravdu chcete smazat tento skript?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Správa klávesových zkratek</translation>
    </message>
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
        <source>Warning</source>
        <translation>Varování</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Tato sekvence kláves je už používána</translation>
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
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Vrstvy</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Přidat novou vrstvu</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Smazat vrstvu</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Vrstvu dopředu</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Vrstvu dozadu</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nová vrstva</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Opravdu chcete také smazat všechny objekty z této hladiny?</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Jméno</translation>
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
        <translation>Editace stylů čar</translation>
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
        <source>Warning</source>
        <translation>Varování</translation>
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
        <source>&amp;Append</source>
        <translation type="obsolete">Připoji&amp;t</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <source>Do you really want to delete this Style?</source>
        <translation>Opravdu chcete smazat tento styl?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Import</translation>
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
        <translation type="unfinished">Autor:</translation>
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
        <translation type="unfinished">Alt+Z</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="obsolete">Předaný objekt nelze nahrát</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="obsolete">Správa maker</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="obsolete">Zobrazí okno, ve kterém editujete makra.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="obsolete">Vytvářet, upravovat a mazat makra</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="obsolete">&amp;Makro</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Správa maker</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="obsolete">Nelze otevřít soubor: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="obsolete">Úprava makra</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zadané jméno už používá jiné makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Chyba při vytváření makra. Správce maker nebyl schopen vytvořit makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Makro &apos;%1&apos; obsahuje drobnou chybu.
Chyba: %2
Kompletní chybový výpis:

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Makro &apos;%1&apos; neproběhlo bez chyby.
Chyba: %2
Kompletní chybový výpis:

%3
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Nové makro</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation type="obsolete">Jméno nového makra: </translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Správa maker</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="obsolete">Přejmenování makra se nezdařilo, protože nové jméno už používá makro jiné.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="obsolete">Správa maker</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Všechny změny provedené v tomto dialogu jsou nevratné.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nové</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Vytvořit nové makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Vytvořit nové makro. Bude vyžadováno jméno makra.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="obsolete">Makro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Upravit</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="obsolete">Klávesová zkratka</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Popis</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Tabulka definovaných maker.&lt;/p&gt;
&lt;p&gt;Jméno: Jméno makra.&lt;/p&gt;
&lt;p&gt;Edit: Příznak, zda bylo makro upravováno.&lt;/p&gt;
&lt;p&gt;Klávesová zkratka: Sekvence kláves, která makro spustí.&lt;/p&gt;
&lt;p&gt;Popis: Python &quot;docstring&quot;, t.j. dokumentační řetězec, makra.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="obsolete">&amp;Přejmenovat</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="obsolete">Alt+P</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="obsolete">Přejmenovat vybrané makro.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Přejmenovat vybrané makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Upravit...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Úprava zdrojového kódu vybraného makra (je-li dostupný).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Smazat zvolené makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Smazat vybrané makro. Operace je neměnná.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="obsolete">Nastavit &amp;klávesovou zkratku</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Nastavit sekvenci kláves, která spustí vybrané makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Nastavit sekvenci kláves, která spustí vybrané makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="obsolete">&amp;Export</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="obsolete">Export makra do souboru.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Export makra do souboru. Výsledkem je Python skript, který může být nahrán pomocí &apos;Nahrát rozšíření...&apos;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="obsolete">Smazat &amp;vše</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+V</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="obsolete">Smazat všechna makra.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Smazat všechna makra. Tato operace je nevratná.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Import</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="obsolete">Import makra ze souboru.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Import makra z Python souboru.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation type="obsolete">Zavřít dialogové okno</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation type="obsolete">Návrat do Scribusu</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Upravit vybrané makro.&lt;/p&gt;
&lt;p&gt;Jestliže je tlačítko šedivé, t.j. nelze stisknout, makro nemá žádný obsah.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation type="obsolete">Scribus makra (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Otevřít soubor</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation type="obsolete">Vybrat soubor s makrem.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Uložit soubor</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation type="obsolete">Uložit všechna makra</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation type="obsolete">Přejmenovat makro</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation type="obsolete">Vložte nové jméno: </translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation type="obsolete">Nastavit zkratku makra</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation type="obsolete">Vložte novou zkratku: </translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ne</translation>
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
        <translation type="unfinished">Velikost strany</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vlastní</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientace:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Na výšku</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Na šířku</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Výška:</translation>
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
        <translation type="unfinished">Vodítka okrajů</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Dolní:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Pravý:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Levý:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Vzdálenost mezi horním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Vzdálenost mezi dolním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Vzdálenost mezi levým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Vzdálenost mezi pravým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Uvnitř:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">&amp;Vnější:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
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
        <source>Duplicates the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varování</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Ano</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Jméno:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopie %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Název:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normální</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Vícenásobné duplikování</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
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
        <translation type="obsolete">Chyba ve skriptu</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Jestliže běžel skript distribuovaný s programem, informujte nás o chybě na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; - děkujeme vám.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Ukázat &amp;konzoli</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Skrýt &amp;konzoli</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Tato chybová zpráva je ve vaší systémové schránce. Použijte Ctrl+V, tím ji
můžete zkopírovat do chybového hlášení.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Změnit...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Import</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
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
        <source>Import Template</source>
        <translation type="obsolete">Importovat šablonu</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importovat stranu(y)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Z dokumenu:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importovat stranu(y):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>od 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Vytvořit stranu(y)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">před stranu</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">za stranu</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">na konec</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>od %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation type="unfinished"></translation>
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
        <source>&amp;Import Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
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
        <translation type="unfinished">&amp;Import</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Chybějící písmo</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">Písmo %1 není nainstalované.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Použít</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">místo</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Přesunout strany</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopírovat stranu</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Posun stran:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>po:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">před stranu</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">za stranu</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">na konec</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Obrys:</translation>
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
        <source>Custom Spacing</source>
        <translation type="obsolete">Vlastní proklad</translation>
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
        <source>Manual Kerning</source>
        <translation type="obsolete">Ruční vyrovnání znaků (Kerning)</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Název &quot;%1&quot; není jedinečný.
Vyberte, prosím, jiný.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Stín:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Jméno vybraného objektu</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horizontální umístění středu otáčení</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikální umístění středu otáčení</translation>
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
        <translation>Povolit nebo zakázat tisk objektu</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Písmo vybraného textu nebo objektu</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Změna šířky znaků písma</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva obrysu písma</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva výplně písma</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Sytost barvy obrysu písma</translation>
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
        <translation>Změna nastavení bodů čáry</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Styl čáry</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Šířka (tloušťka) čáry</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Typy spojení čar</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Typy ukončení čáry</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Styl čáry aktuálního objektu</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Výběr obrys rámu...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Úprava obrysu rámu...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Nastavní poloměru zakulacení rohů</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Počet sloupců v textovém rámu</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Vzdálenost mezi sloupci</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Vzdálenost textu od horního obrysu rámu</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Vzdálenost textu od spodního obrysu rámu</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Vzdálenost textu od levého obrysu rámu</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Vzdálenost textu od pravého obrysu rámu</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Nastavení tabelátorů textového rámu...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Možnost nastavit jiné rozměry obrázku než rámce</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodorovné posunutí obrázku vůči rámu</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Svislé posunutí obrázku vůči rámu</translation>
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
        <translation>Obrázek změní velikost podle rozměru rámu</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Obrázek si zachová své proporce</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Čáry buňky tabulky</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Horní čára</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Levá čára</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Pravá čára</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Dolní čára</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Dodržet poměr stran</translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Povolit obtékání textu v nižších objektech kolem zvoleného obrysu</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Přepínat mezi mezisloupcovou mezerou a šířkou sloupce</translation>
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
        <translation>Čá&amp;ra</translation>
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
        <translation>Upravit &amp;obrys...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Zakolatit
&amp;rohy:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
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
        <translation>Te&amp;xt obtéká okolo rámu</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Použít &amp;celostránkový rámec</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Použít &amp;konturu</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Velikost písma:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">Vyrovnání zna&amp;ků (kerning):</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Řád&amp;kování:</translation>
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
        <translation>Změni&amp;t velikost na rozměr rámu</translation>
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
        <translation>&amp;Typ čáry:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Šíř&amp;ka čáry:</translation>
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
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Použít obrys celého rámu místo obysu objektu</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Použít druhou čáru, založenou na obrysu rámu, při řetězení textu</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Jazyk dělení slov</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Psaní zprava doleva</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation type="obsolete">&amp;Proklad:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
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
        <source>Start Arrow:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>End Arrow:</source>
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
        <translation>Šířka čáry:</translation>
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
        <source> pt </source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Plná čára</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Čárkovaná čára</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Tečkovaná čára</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Čerchovaná čára</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Dvojitě čerchovaná čára</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Název &quot;%1&quot; není jedinečný.
Vyberte, prosím, jiný.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Upravit šablony</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varování</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopie %1</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nová šablona</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normální</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Název:</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">Připoji&amp;t</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nový</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Ano</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Jméno:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Opravdu chcete smazat tuto šablonu?</translation>
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
        <source>New Document</source>
        <translation>Nový dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Vlastní</translation>
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
        <translation>Vodítka okrajů</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Body (pts)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Palce (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pika (p)</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Vodítka sloupců</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetry (mm)</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Velikost strany dokumentu - buď standardní nebo volitelný rozměr</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientace stran dokumentu</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Výška stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Povolit rozložení po jedné nebo po více stranách</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">První strana je levou stranou dokumentu</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi horním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi dolním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi levým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi pravým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Číslo první strany dokumentu</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Implicitní měrná jednotka dokumentu</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Vytvářet automaticky textové rámce, jsou-li přidány nové strany</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Počet sloupců v automaticky vytvořených textových rámcích</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Vzdálenost mezi automaticky vytvořenými textovými rámci</translation>
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
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">P&amp;rotilehlé stránky (Dvoustrany)</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Prv&amp;ní strana vlevo</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Levý:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Pravý:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Horní:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Dolní:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Čí&amp;slo první strany:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Implicitní jednotka:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Automatické  textové rámce</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Odstup:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>S&amp;loupce:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Uvnitř:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Vnější:</translation>
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
        <source>Open Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="unfinished"></translation>
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
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Včechny soubory (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Otevřít</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="unfinished">No&amp;vé ze šablony...</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Levá strana</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Pravá strana</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Přesunout řídící body</translation>
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
        <translation>Vynulovat řídící body</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Vynulovat tento řídící bod</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Je-li zatrženo, jsou souřadnice relativní vzhledem
ke straně, v opačném případě k objektu.</translation>
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
        <translation>Upravit &amp;konturu</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Vy&amp;nulovat konturu</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Konec úprav</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Nezávisle přesouvat řídící body</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Symetricky přesouvat řídící body</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Otevře polygon nebo ořeže Bézierovou křivku</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Zavřít tuto Bźierovou křivku</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Zrcadlit horizontálně</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Zrcadlit vertikálně</translation>
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
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Zmenšit o uvedené %</translation>
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
        <source>% to Enlarge or Reduce By</source>
        <translation>% o které se bude zvětšovat nebo zmenšovat</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivace úprav kontury</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Nastavit konturu na obrys rámu</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Uvolnit horizontálu doprava</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Import OpenOffice.org &amp;Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="obsolete">Nastavení importu OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Aktualizovat styly odstavce</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Má být existující Scribus styl změněn, jestliže v importovaném dokumentu
existuje styl se stejným jménem nebo se nemá změnit</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="obsolete">Zabalit styly odstavce</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="obsolete">Setřídit styly odstavce podle atributů.
Výsledkem je méně stylů, ale mohou vzniknout problémy
s jejich nastavením. Používejte, jestliže víte, že text nemusíte
příliš upravovat.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Použít jméno dokumentu jako předponu stylů odstavce</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Má importer přidávat jméno dokumentu před
jméno stylu odstavce ve Scribusu</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Neptat se znovu</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Má si importer vždy pamatovat nastavenou hodnotu
a nikdy znovu nevyžadovat potvrzení</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Vlastnosti importu OpenDocument</translation>
    </message>
    <message>
        <source>Update paragraph Styles</source>
        <translation type="obsolete">Aktualizovat styly odstavce</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">Konzole skriptů</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Exportovat rozsah</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Nastavení souboru</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Levý okraj</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Pravý okraj</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete">dpi</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Všeobecné</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Vkládání</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Dostupná písma:</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Písma k vložení:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Strana</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Efekty</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">s</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Bez efektu</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Pruhy</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Rám</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Rozpuštění</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Lesk</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Rozdělit</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Setřít</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Vodorovně</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Svisle</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Zevnitř</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Zvenku</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Zleva doprava</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Shora dolů</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Zdola nahoru</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Zprava doleva</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Hesla</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Nastavení</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Tiskárna</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Účel generování:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Sytost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Obrázky:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Nepoužívat vložené ICC profily</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Výstup do PDF/X-3</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Vlastní formát stránky</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Monitor / Web</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Zleva nahoře doprava dolů</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Nastavení obrázků</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automaticky</translation>
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
        <translation type="obsolete">Žádné</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Maximální</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Vysoká</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Střední</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Nízká</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Minimální</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Všeobecné</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Písma</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Export všech stran do PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Export rozsahu stránek do PDF</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Určuje kompatibilitu PDF. Implicitně Acrobat 4.0, který poskytuje kompatibilitu nejširší.
Vyberte Acrobat 5.0, jestliže dokument používá vlastnosti PDF 1.4, jako je např. průhlednost nebo 128 bitové šifrování.
Export do PDF/X-3 použijte při komerčním tisku. Je dostupně při fungující správě barev.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Určuje vazbu stran v PDF. Jestliže nepotřebujete speciální
nastavení, nechte implicitní volbu.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Vytvoří náhledy všech stran v PDF.
Některé prohlížeče náhledy použijí k navigaci.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Vytvořit PDF články, což umožňuje navigaci odkazy v PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Vložit záložky obsažené v dokumentu.
Ty jsou užitečné k navigaci v obsáhlých PDF dokumentech.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Rozlišení textu a vektorové grafiky při exportu. Neovlivňuje 
rozlišení bitmapových obrázků, jako jsou např. fotografie.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Komprese textu a grafiky.
Pokud nemáte pádný důvod, nechte povolenu. Redukuje velikost PDF souboru.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Způsob komprese obrázků.
Automaticky - Scribus sám vybere vhodnou metodu.
ZIP - vhodné na obrázky s plnými barvami.
JPEG - vytvoří menší PDF, je-li použito mnoho fotografií (drobná ztráta kvality).
Pokud nepotřebujete speciální kompresi, nechte nastaveno na Automaticky.
</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Snížit kvalitu bitmapových obrázků na zvolené DPI.
Jestliže volbu nepovolíte, bude jim ponecháno jejich původní rozlišení.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (Body na palec) exportu obrázků.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Vložit písma do PDF. Vložením písem zajistíte
stejný vzhled dokumentu v jiných prohlížečích.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Povolit prezentační efekty v celoobrazovkovém režimu Acrobat Readeru.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Ukázat náhled každé strany uvedené v seznamu nahoře.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Jak dlouho je strana zobrazena, než je spuštěn přechod na dalsší.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Jak dlouho běží prezentační efekt.
Menší hodnota efekt zrychlí, větší zpomalí.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Typ efektu.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Směr efektu Přesouvání řádků nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Startovní pozice efektu Rám nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Směr efektu Lesk nebo Setřít.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Použít vybraný efekt na všechny strany.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Povolit bezpečnostní vlastnosti exportovaného PDF.
Jestliže máte zvolen Acrobat 4.0, bude použito šifrování 40 bity.
Jestliže máte zvolen Acrobat 5.0, použiji 128 bitů.
Poznámka: PDF šifrování není tak spolehlivé jako GPG nebo PGP a má svá omezení.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Zvolte heslo, které povolí nebo zakáže všechny
bezpečnostní vlastnosti exportovaného PDF</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Barevný model výstupu PDF.
Zvolte Monitor/Web na PDF, které je publikováno na monitoru nebo na typické kancelářské tiskárně.
Zvolte Tiskárna, bude-li PDF tištěno na CMYK tiskárně.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Vložit barevný profil plných barev</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Barevný profil plných barev</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Účel reprodukce plných barev</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Vložit barevný profil obrázků</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Nepoužívat barevný profil vložený ve zdrojových obrázcích</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Barevný profil obrázků</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Účel reprodukce obrázků</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Výstupní profil tisku.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Volitelný text, jehož vyplněním bude PDF nebo PDF/X-3 splňovat
specifikaci. Doporučujeme použít titul dokumentu.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Vzdálenost ořezu od horního okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Vzdálenost ořezu od dolního okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Vzdálenost ořezu od levého okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Vzdálenost ořezu od pravého okraje fyzické strany</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Úrovňe komprese: Minimální (25%), Nízká (50%), Střední (75%), Vysoká (85%), Maximální (95%)</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Zvolit heslo, které musí použít čtenář PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Povolit tisk PDF. Jestliže není zatrženo, tisk není povolen.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Povolit modifikaci PDF. Jestliže není zatrženo, modifikace jsou zakázány.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Povolit kopírování textu a grafiky z PDF.
Jestliže není zatrženo, text a grafika nelze kopírovat.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Povolit přidávání poznámek a polí formulářů do PDF.
Jestliže není zatrženo, přidávání je zakázáno.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Vytvořit PDF soubor</translation>
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
        <source>&amp;All Pages</source>
        <translation type="obsolete">Všechny str&amp;any</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Vybrat strany</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">&amp;Kompatibilita:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">Vaz&amp;ba:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Vytvořit &amp;náhledy</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Uložit &amp;propojené textové rámy jako PDF články</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">Vče&amp;tně záložek</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Rozlišení:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">Z&amp;působ:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">K&amp;valita:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Převzorkov&amp;at obrázky na:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">V&amp;ložit všechna písma</translation>
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
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">Zobrazit &amp;náhledy stran</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">&amp;Doba zobrazení:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Tr&amp;vání efektu:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">&amp;Typ efektu:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">&amp;Přesouvání řádků:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">O&amp;d:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">&amp;Směr:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">Po&amp;užít efekt na všechny strany</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">Použít ši&amp;frování</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Uživatel:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">&amp;Vlastník:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">&amp;Povolit tisk dokumentu</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Povolit z&amp;měny dokumentu</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Povolit &amp;kopírování textu a grafiky</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Povolit přidávání &amp;anotací a polí formulářů</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">&amp;Bezpečnost</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">&amp;Plánovaný výstup na:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Použít vlastní nastavení reprodukce</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Nastavení reprodukce</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">&amp;Frekvence:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">Úhe&amp;l:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">Funkce &amp;bodu:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Prostá tečka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Čára</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Kruh</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Elipsa</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Použít ICC profil</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;Info text:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">&amp;Výstupní profil:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Uložit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF soubory (*.pdf);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Pokročilá volba, která není implicitně povolena. Může být použita, jestliže to vyžaduje vaše tiskárna.
Pozor. vámi exportované PDF soubory se nemusí korektně tisknout na jiné tiskárně
a nebudou plně přenositelné mezi systémy.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Komprimovat text a &amp;vektorovou grafiku</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Povolit prezentační &amp;efekty</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Prezentace</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotace:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Čá&amp;stečné zařazení písem (subset)</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Částečně zařazená písma (subset):</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Zrcadlit stranu(y) horizontálně</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Zrcadlit stranu(y) vertikálně</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation>Uložit jako PDF</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Správa barev musí být povolena k použití PDF/X-3. Správu barev povolíte v Nastavení.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;PDF/X-3 je podporován a povolen, ale nemůže být použitý ve vybrané verzi PDF. Zvolte úroveň kompatibility PDF/X-3 .&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;PDF/X-3 není podporován. Program není sestaven s podporou CMS.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation type="obsolete">Povolit přidávání PDF anotací.
Není-li povoleno, anotace jsou zakázány.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation type="obsolete">Nastevní bezpečnosti PDF</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation type="obsolete">Nastavení PDF/X-3</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Bezpečnost PDF nelze použít s PDF/X-3. Bezpečnost podporují jiné verze PDF. Zvolte jinou úroveň kompatibility./qt&gt;</translation>
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
        <translation>Ukázka před tiskem</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Zajistí hezčí vzhled textu v náhledu, ale zpomalí jeho vytváření.
Týká se Type 1 písem</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Zobrazí práhlednost a průhledné objekty dokumentu. Vyžaduje Ghostscript 7.07 nebo novější</translation>
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
        <source>Anti-alias &amp;Text</source>
        <translation>Vyhlazený &amp;text</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Vyhlazená &amp;grafika</translation>
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
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">UCR - under color removal - odstranění přebytečné barvy, která by 
způsobila rozpíjení (při přílišné saturaci papíru barvou) nebo trhání 
barvy při několika vrstvém (např. barevném) laserovém tisku.
Viz. např. http://www.typo.cz/_pismo/pis-tech-post.html
a jiné.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Zajistí hezčí vzhled TrueType a OpenType písem, EPS, PDF a vektorové grafiky
v náhledu, ale zpomalí se jeho vytváření</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varování</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Azurová (Cyan)</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Purpurová (Magenta)</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Žlutá (Yellow)</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Černá (Black)</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="unfinished">Tisk...</translation>
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
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopírovat sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Přesunout sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Obrázek</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Soubor:</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Textový rám</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Text na křivky</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Odstavce:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Slova:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Znaky:</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Editovat text...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Žádné</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Řeťezený text</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Tisk:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Povoleno</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Zakázáno</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Program</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">chybí!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varování</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopie</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Vložit</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Zobrazit o&amp;kraje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Zobrazit &amp;rámy</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Zobrazit &amp;obrázky</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Ukázat &amp;mřížku</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Zobrazit &amp;vodítka</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">&amp;Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">M&amp;agnetická mřížka</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Ma&amp;gnetická vodítka</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Původní PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Aktuální PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">In&amp;fo</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Vložit obrázek...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">Vi&amp;ditelný obrázek</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Aktualizovat obrázek</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Upravit obrázek</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Přizpůsobit rámec obrázku</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Vložit text...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">Připo&amp;jit text...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Editovat text...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Vložit ukázkový &amp;text (Lorem Ipsum)</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Je PDF &amp;záložkou</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Je PDF &amp;anotací</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">Vlast&amp;nosti anotace</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Vlastnosti &amp;pole</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;Možnosti PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Zamknout</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">O&amp;demknout</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Zamknout &amp;rozměry objektu</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Odemknout &amp;rozměry objektu</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">&amp;Poslat do zápisníku</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Seskupit</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Zrušit &amp;seskupení</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">&amp;Hladina</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">&amp;Dát zcela dospodu</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Dát zcela &amp;navrch</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">Dát o &amp;vrstvu níž</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Dát o v&amp;rstvu výš</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Rám &amp;obrázku</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">&amp;Mnohoúhelník</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">O&amp;brysy</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Textový rám</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Beziérova křivka</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">K&amp;onverze na</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Smazat</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">V&amp;ymazat obsah</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Ukázat v&amp;lastnosti...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Skrýt v&amp;lastnosti...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Opravdu chcete smazat veškerý text?</translation>
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
        <translation>Čára</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnohoúhelník</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomená čára</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Text na křivky</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Kopie</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Jméno</translation>
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
        <translation type="unfinished">&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+V</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopírovat</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+Z</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+S</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+N</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
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
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Strana</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete">z %1</translation>
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
        <translation type="unfinished">Střední</translation>
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
        <source>Pictures</source>
        <translation type="obsolete">Obrázky</translation>
    </message>
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
        <translation>Strana</translation>
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
        <translation>Jít na</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Správa obrázků</translation>
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
        <translation type="unfinished">Uložit jako &amp;obrázek...</translation>
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
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Neznámý</translation>
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
    <message>
        <source>PostScript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
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
        <translation type="unfinished">Soubor</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Ne</translation>
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
        <translation>Vlastnosti mnohoúhelníků</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Ro&amp;hy:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotace:</translation>
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
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Počet rohů mnohoúhelníků</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Stupně rotace mnohoúhelníků</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Ukázkový mnohoúhelník</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Použít &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Aplikovat konvexnost/konkávnost na mnohoúhelník</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Záporná hodnota nastaví mnohoúhelník jako 
konkávní (nebo s hranami do hvězdy), kladná konvexní</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished">Ro&amp;hy:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotace:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Použít &amp;faktor</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Počet rohů mnohoúhelníků</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Stupně rotace mnohoúhelníků</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished">Aplikovat konvexnost/konkávnost na mnohoúhelník</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Ukázkový mnohoúhelník</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished">Záporná hodnota nastaví mnohoúhelník jako 
konkávní (nebo s hranami do hvězdy), kladná konvexní</translation>
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
        <source>Scrapbook</source>
        <translation>Zápisník</translation>
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
        <source>Units</source>
        <translation type="obsolete">Jednotky</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Body (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetry (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Palce (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pika (p)</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Nabídky</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Cesty</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
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
        <translation>Vodítka okrajů</translation>
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
        <source>Grid Layout</source>
        <translation type="obsolete">Rozložení mřížky</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Barvy mřížky</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Umístění</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Dolní index</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Horní index</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kapitálky</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Jiné</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Žádné</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Ostatní nastavení</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Náhled</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Malá</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Střední</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">Obrazovku přizpůsobíte posuem jezdce na spodním pravítku.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vybrat adresář</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externí nástroje</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Různé</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Interpret postsriptu</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Nástroj na úpravu obrázků</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Tisk</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Vyberte implicitní vzhled a dekoraci oken.
Scribus používá jakékoli téma z Qt nebo KDE</translation>
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
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Poloměr oblasti, kterou Scribus považuje za oblast daného objektu</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Počet současně otevřených dokumentů, které jsou vidět v Soubor menu</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Implicitní adresář na dokumenty</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Implicitní adresář na ICC profily</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Implicitní adresář na skripty</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Implicitní velikost strany, standardní nebo vlastní rozměr</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Implicitní orientace stran dokumentu</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Šířka stran dokumentu - upravitelná, jestliže je vybrán volitelný rozměr</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Povolit rozložení po jedné nebo po více stranách</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">První strana je levou stranou dokumentu</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi horním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi dolním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi levým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi pravým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Je-li povoleno, Scribus uloží záložní kopii dokumentu do souboru s .bak příponou
po každém automatickém uložení</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Časový interval automatického ukládání</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Vzdálenost mezi čarami vedlejší mřížky</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Vzdálenost mezi čarami hlavní mřížky</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Vzdálenost, ve které se čára mřížky chová vůči objektu jako magnet</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Barva čar vedlejší mřížky</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Barva čar hlavní mřížky</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Barva vodítek, které vkládáte manuálně</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Umístit mřížku za objekty</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Umístit mřížku před objekty</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Posunutí nad účaří</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Relativní velikost horního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Posunutí pod účaří</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Relativní velikost dolního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Relativní velikost kapitálek vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Procentuální zvětšení řádkování podle velikosti písma</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Vlastnosti textových rámců</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Vlastnosti obrázkových rámců</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Vlastnosti kreslení obrysů</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Vlastnosti úrovně zvětšení</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Vlastnosti čar</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Vlastnosti mnohoúhelníků</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Písmo nového textového rámu</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Velikost písma nového textového rámu</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Barva písma</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Počet sloupců v textovém rámu</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Mezera mezi sloupci textového rámu</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Ukázka písma</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Obrázkové rámy mohou libovolně měnit rozměry obrázku</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Horizontální zvětšení obrázků</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Vertikální zvětšení obrázků</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Držet stejné horizontální a vertikální zvětšení</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Obrázky budou deformovány podle rozměrů rámu</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Automaticky nastavovaná velikost obrázků dodržuje originální proporce</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Barva výplně obrázkových rámů</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Sytost barvy výplně</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Barva čar obrysů</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Sytost barvy čar</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Barva výplně obrysů</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Styl čar obrysů</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Tloušťka čar obrysů</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Minimální povolené zvětšení (zmenšení)</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Maximální povolené zvětšení</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Změna zvětšení - krok operace lupou</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Barva čar</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Sytost barvy</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Styl čar</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Šířka čár</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Počet rohů mnohoúhelníků</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Stupně rotace mnohoúhelníků</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Ukázkový mnohoúhelník</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Zvolit velikost náhledu v zápisníku</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Uložit obsah zápisníku po každé jeho změně</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Ukazovat strany vedle sebe, jsou-li nastaveny protilehlé strany</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papíru</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Barva okrajových čar</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vyplnit plochu za hranicemi strany barvou okrajů</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Povolit transparentnost v exportu PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Nastavení implicitní úrovně zvětšení</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="unfinished">Umístění Ghostscript interpreteru na disku</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat text při vykreslování EPS a PDF na monitoru</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Vyhlazovat grafiku při vykreslování EPS a PDF na monitoru</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Umístění grafického editoru na disku</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Neukazovat objekty mimo hranice tisknutelné strany nebo exportovaného souboru</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Pomocná mřížka</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Zapnout pomocnou mřížku</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Vypnout pomocnou mřížku</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete">px</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Téma:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost písma:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Nastavení myši</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Skok kolečka myši:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Poloměr oblasti patřící objektu:</translation>
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
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">P&amp;rotilehlé stránky (Dvoustrany)</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Prv&amp;ní strana vlevo</translation>
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
        <source>&amp;Enabled</source>
        <translation>Povo&amp;leno</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Interval:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Mezery &amp;vedlejší mřížky:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Mezery &amp;hlavní mřížky:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">&amp;Dosah magnetických vodítek:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">B&amp;arva vedlejší mřížky:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">&amp;Barva hlavní mřížky:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Barva v&amp;lastních vodítek:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">Barva po&amp;mocné mřížky:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">V &amp;pozadí</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">V p&amp;opředí</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">Zapnou&amp;t</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">Vypno&amp;ut</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Posunutí:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">Z&amp;většení:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Po&amp;sunutí:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">Zvě&amp;tšení:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Zvětše&amp;ní:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Pomocná &amp;mřížka:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">&amp;Odstup pomocné mřížky:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">&amp;Automatické řádkování:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Implicitní &amp;písmo:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">Implicitní &amp;velikost:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">&amp;Bava textu:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">&amp;Sloupce:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">O&amp;dstup:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Ba&amp;rva čáry:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Stín:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">Stí&amp;n:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Typ čáry:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">Šíř&amp;ka čáry:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">&amp;Maximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Krokování:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Vodorovné zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">V&amp;ertikální zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">Změnit velikost &amp;obrázku na rozměr rámu</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Dod&amp;ržet poměr stran</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">Ro&amp;hy:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotace:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Při změně &amp;uložit obsah</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Velká</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Zobrazovat &amp;stranu za stranou</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Barvy strany</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Pozadí:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Okraje:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Použí&amp;t vlastnosti průsvitnosti PDF-1.4</translation>
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
        <source>Clip to Page &amp;Margins</source>
        <translation>Připnout k okrajů&amp;m strany</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Uvnitř:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Vnější:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Aplikovat tzv. &amp;Under Color Removal</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">Š&amp;ablony:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Změnit...</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Uživatelem definovaný adresář šablon</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Použít &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Aplikovat konvexnost/konkávnost na mnohoúhelník</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Záporná hodnota nastaví mnohoúhelník jako 
konkávní (nebo s hranami do hvězdy), kladná konvexní</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>UCR - under color removal - odstranění přebytečné barvy, která by 
způsobila rozpíjení (při přílišné saturaci papíru barvou) nebo trhání 
barvy při několika vrstvém (např. barevném) laserovém tisku.
Viz. např. http://www.typo.cz/_pismo/pis-tech-post.html
a jiné.</translation>
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
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation type="obsolete">Implicitní adresář ICC profilů. Nelze
měnit, jestliža máte otevřný dokument.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation type="obsolete">Zapne pomocnou mřížku</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation type="obsolete">Vypne pomocnou mřížku</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation type="obsolete">Umístení Ghostscript interpretu ve vašem systému</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Umístění grafického editoru ve vašem systému. Jestliže
používáte GIMP a vaše distribuce obsahuje &apos;gimp-remote&apos;,
doporučujeme jeho nastavení. Gimp-remote použije již
běžící instanci GIMPu.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Jazyk:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="unfinished"></translation>
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
        <source>Preflight Verifier</source>
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
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
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
        <translation type="unfinished">Barva:</translation>
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
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch Space</source>
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
        <translation type="obsolete">Soubor</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ano</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ne</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
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
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
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
        <translation type="unfinished">Levá strana</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Pravá strana</translation>
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
        <translation type="unfinished">Postsript</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Book</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, &apos;Book&apos; sets margins classically (Gutenberg). &apos;Book&apos; is proposed for two-sided documents. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Otevřít...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Uložit</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Soubor</translation>
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
        <translation type="unfinished">Konzole skriptů</translation>
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
        <source>Open File With Python Commands</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varování</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Current Output</source>
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
        <translation>Atributy</translation>
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
        <translation>Ukázat informace o souboru</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Ukázat obsah souboru</translation>
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
        <translation>Po&amp;dtržené</translation>
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
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Pozadí</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Ukázka před tiskem</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">On-line reference</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skripty (*.py);; All Files (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-obrázky (*.svg *.svgz);;All Files (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-obrázky (*.svg);;All Files (*)</translation>
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
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;kript</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Uložit jako obrázek</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Chyba při zápisu souboru.</translation>
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
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Náhled písem</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Vložit speciální</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">No&amp;vé ze šablony...</translation>
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
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Uložit jako &amp;obrázek...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Ukázka pře&amp;d tiskem</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Import &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Všechny podporované formáty (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Uložit jako ša&amp;blonu...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">Manuál &amp;Scripteru...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">&amp;Scribus skripty</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Vykonat skript...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">&amp;Použité skripty</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Ukázat &amp;konzoli</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Uložit stranu jako &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Import &amp;SVG...</translation>
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
        <translation>Soubory CSV (čárkou oddělěné hodnoty)</translation>
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
        <source>Template: </source>
        <translation type="obsolete">Šablona:</translation>
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
        <translation>Albánština</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskitština</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulharština</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brazilština</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalánština</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Čínština</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Čeština</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dánština</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandština</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Angličtina</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Angličtina (Britská)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Němčina</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finština</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francouzština</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galština</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Řečtina</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Maďarština</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonéský</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italština</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Korejština</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litevština</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norština (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norština (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norština</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polština</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruština</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Švédština</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Španělština</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Španělština (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovenština</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovinština</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Srbština</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Nelze nastavit větší progres než je maximální</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished">&amp;O skriptu...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished">O skriptu</translation>
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
        <translation>Nelze získat řádkování z netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat odstup sloupců z netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze získat text z netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze vložit text do netextového rámu.</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="obsolete">Pozice je mimo povolené hranice</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Hodnota zarovnání je mimo povolený rozsah. Použijte jednu z předdefinovaných konstant scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit typ zarovnání netextového rámce</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Velikost písma mimo povolený rozsah. Musí být mezi 1 a 512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nasatvit velikost písma netextového rámu</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit písmo netextového rámu</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Písmo nenalezeno</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Velikost řádkování je mimo povolený rozsah. Musí být větší než 0</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit řádkovéní netextového rámce</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Mezisloupcová mezera nesmí být negativní</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit mezisloupcovou mezeru netextového rámu</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Počet slupců je mimo povolený rozsah. Musí být větší než 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit počet sloupců netextového rámce</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Pozice výběru mimo povolené hranice</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze vybrat text v netextovém rámce</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze smazat text z netextového rámce</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit výplň textu netextového rámce</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit obrys netextového rámce</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit stín netextového rámce</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Pouze textové rámce se mohou zřetězit</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Cílový rámec musí být prázdný</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Text cilového rámce už poračuje v rámci jiném</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Cílový rámec už je zřetězený</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Oba rámce jsou stejný objekt</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze zrušit zřetězení netextového rámce</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished">Objekt není zřetězený textový rámec, proto nelze řetězeí zrušit.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished">Objektu na posledním místě nelze zrušit zřetězení. Použijte místo něj předchozí rámec.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze převést netextový rámec na křivky</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Chyba při otvírání dokumentu</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Chyba při ukládání dokumentu</translation>
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
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze měnit velikost pomocí 0%</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze vytvořit prázdný náhled</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze ukládat bez názvu souboru</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze vytvořit vrstvu s prázdným jménem</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Vrstva nenalezena</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze odstranit poslední vrstvu</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze vytvořit vrstvu beze jména</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt se zadaným názvem už existuje</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Seznam musí obsahovat alespoň dva body (čtyři hodnoty)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="obsolete">Seznam musí obsahovat sudý počet hodnot</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Seznam musí obsahovat alespoň tři body (šest hodnot)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="obsolete">SeSeznam musí obsahovat alespoň čtyři body (osm hodnot)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="obsolete">Seznam musí obsahovat násobek šesti položek</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt nenalezen</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Styl nenalezen</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze nastavit styl netextového rámce</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Chyba při ukládání EPS</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="obsolete">Číslo strany je mimo povolený rozsah</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">parametr není seznam: musí být seznam s float hodnotami</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">parametr obsahuje nečíselné hodnoty: musí být seznam s float hodnotami</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="obsolete">Šířka čáry mimo povolený rozsah: &lt;0; 12&gt;</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Stín čáry mimo povolený rozsah: &lt;0; 100&gt;</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Stín mimo povolený rozsah: &lt;0; 100&gt;</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Úhel rohu nesmí být negativní číslo.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Styl čáry nenalezen</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze získat barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Barva nenalezena</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Nelze změnit barvu s prázdným jménem.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Barva není v dokumentu nalezena</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Barva není nalezena v implicitních barvách</translation>
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
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="obsolete">Import OpenOffice.org &amp;Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.o Writer dokumenty</translation>
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
      the text colour on a graphics frame doesn&apos;t make sense, and will result
      in this exception being raised.
    - Errors resulting from calls to the underlying Python API will be
      passed through unaltered. As such, the list of exceptions thrown by
      any function as provided here and in its docstring is incomplete.
Details of what exceptions each function may throw are provided on the
function&apos;s documentation.
</source>
        <translation type="obsolete">Scripter - Python modul programu Scribus
Scripter je rozhraní jazyka Python programu Scribus. Umožňuje ovládat Scribus a manipulovat
s objekty v dokumentu. Všechny dostupné třídy, procedury, funkce a konstanty jsou popsány dále.
Několik věci platí v celém Scripteru. Většina funkčnosti pracuje s rámy. Rámce jsou identifikovány
svým jménem - textovým řetězcem (není Python objektem). Některé funkce akceptují volitelný
parametr, t.j. jméno rámce. Některé obecnější výjimky mohou být vyvolány v různých funkcích,
ale zatím nejsou dokumentovány v nápovědě jednotlivých funkcí.
    - Mnoho funkcí vyvolá NoDocOpenError, jestliže je zavoláte a nemáte otevřený žádný dokument.
    - Jestliže nepoužijete jméno rámce jako parametr, funkce zkusí nejít vybraný objekt. Pokud
      nenalezne, vyvolá NoValidObjectError.
    - Funkce vyvolá WrongFrameTypeError, jestliže bude zpracovávat rámec, se kterým operace
      nebude dávat smysl. Např. nastavení barvy textu obrázkovému rámci smysl nedává, takže je
      výjimka vyvolána.
    - Chyby a výjimky, které vzniknou v ostatních Python modulech, budou předány interpreteru
      beze změny, takže vyjmenované výjimky v dokumentaci Scripteru nejsou ani zdaleka kompletní.
Detaily a výjimky Scripteru jsou popsány v dokumentaci jednotlivých funkcí.</translation>
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
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Předložky a zkratky (nezlomitelná mezera)...</translation>
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
- Many functions will raise a NoDocOpenError if you try to use themwithout a document to operate on.
- If you do not pass a frame name to a function that requires one,the function will use the currently selected frame, if any, orraise a NoValidObjectError if it can&apos;t find anything to operateon.
- Many functions will raise WrongFrameTypeError if you try to use them
on a frame type that they do not make sense with. For example, setting
the text colour on a graphics frame doesn&apos;t make sense, and will result
in this exception being raised.
- Errors resulting from calls to the underlying Python API will be
passed through unaltered. As such, the list of exceptions thrown by
any function as provided here and in its docstring is incomplete.

Details of what exceptions each function may throw are provided on the
function&apos;s documentation.</source>
        <translation type="obsolete">Scripter - Python modul programu Scribus
Scripter je rozhraní jazyka Python programu Scribus. Umožňuje ovládat Scribus a manipulovat
s objekty v dokumentu. Všechny dostupné třídy, procedury, funkce a konstanty jsou popsány dále.
Několik věci platí v celém Scripteru. Většina funkčnosti pracuje s rámy. Rámce jsou identifikovány
svým jménem - textovým řetězcem (není Python objektem). Některé funkce akceptují volitelný
parametr, t.j. jméno rámce. Některé obecnější výjimky mohou být vyvolány v různých funkcích,
ale zatím nejsou dokumentovány v nápovědě jednotlivých funkcí.
    - Mnoho funkcí vyvolá NoDocOpenError, jestliže je zavoláte a nemáte otevřený žádný dokument.
    - Jestliže nepoužijete jméno rámce jako parametr, funkce zkusí nejít vybraný objekt. Pokud
      nenalezne, vyvolá NoValidObjectError.
    - Funkce vyvolá WrongFrameTypeError, jestliže bude zpracovávat rámec, se kterým operace
      nebude dávat smysl. Např. nastavení barvy textu obrázkovému rámci smysl nedává, takže je
      výjimka vyvolána.
    - Chyby a výjimky, které vzniknou v ostatních Python modulech, budou předány interpreteru
      beze změny, takže vyjmenované výjimky v dokumentaci Scripteru nejsou ani zdaleka kompletní.
Detaily a výjimky Scripteru jsou popsány v dokumentaci jednotlivých funkcí.</translation>
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
        <translation>Specifikovaný objekt není rám obrázku.</translation>
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
        <translation>Nelze nastavit zarovnání textu netextovému rámu.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Velikost písma je mimo povolený interval - &lt;1, 512&gt;.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit velikost písma netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit písmo netextového rámu.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Velikost řádkování mimo hranice, musí být větší než 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit hodnotu řádkování netextovému rámu.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Velikost mezisloupcové mezery mimo povolený rozsah. Musí být kladné číslo.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit mezisloupcovou mezeru netextovému rámu.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Počet sloupců mimo povolený rozsah. Musí být &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit počet sloupců netertovému rámu.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Nelze vybrat text v netextovém rámu</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze smazat text z netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nasatvit výplň textu netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit přeškrtnutí netextového rámu.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze nastavit stín písma netextového rámu.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Řetězit lze pouye textové rámy.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Cílový rámec musí být prázdný.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je zřetězený.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Cílový rámec už je zřetězený.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Zdroj a cíl jsou stejný objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nelze zrušit zřetězení netextového rámce.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt není zřetězený, nemůže být proto z řetězení vyjmut.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt je poslední v řadě zřetězení. Zruště zřetězení předchozího objektu.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Netextový rám nelze převést na křivky.</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian)</source>
        <translation type="obsolete">Portugalština (Braz.)</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation type="obsolete">Nelze získat barvu bez názvu.</translation>
    </message>
    <message>
        <source>Colour not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Barva nenalezena</translation>
    </message>
    <message>
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
        <translation type="obsolete">Nelze uložit obrázek.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rám nemůže být poznámkou</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Netextový rám neobsahuje požadované informace</translation>
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
        <translation type="unfinished">Portugalsky</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished">Portugalština (Braz.)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Zhroucení Scribusu</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Scribus spadl díky signálu č. %1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vlastní</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Strana</translation>
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
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
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
        <source>Scribus Development Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
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
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished">p</translation>
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
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">p</translation>
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
        <source>Points (pt)</source>
        <translation type="unfinished">Body (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Palce (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Pika (p)</translation>
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
        <source>File exists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Nahradit</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Všechny</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">&amp;Náhled písem...</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation type="unfinished"></translation>
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
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Script</source>
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
        <source>Short Words Manual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopie č. %1 z</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Černá (Black)</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Azurová (Cyan)</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Purpurová (Magenta)</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Žlutá (Yellow)</translation>
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
        <translation type="unfinished">Nový ze šablony</translation>
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
        <translation type="unfinished">Předložky a zkratky</translation>
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
        <translation>Schovat</translation>
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
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Vodítka okrajů</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Povolit rozložení po jedné nebo po více stranách</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">První strana je levou stranou dokumentu</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi horním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Vzdálenost mezi dolním vodítkem okraje a okrajem strany</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi levým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Vzdálenost mezi pravým vodítkem okraje a okrajem strany
Jestliže jsou použity protilehlé strany, může tento okraj očetřit vazbu</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">P&amp;rotilehlé stránky (Dvoustrany)</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Prv&amp;ní strana vlevo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Uvnitř:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">&amp;Vnější:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strany</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vlastní</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Orientace:</translation>
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
        <source>Width:</source>
        <translation type="obsolete">Šířka:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Výška:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Čí&amp;slo první strany:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientace:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Šíř&amp;ka:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Výška:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Možnosti</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Automatické uložení</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Interval:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Informace o dokumentu</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Vodítka</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Barva:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Zo&amp;brazit netisknutelnou oblast barvou okrajů</translation>
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
        <source>Show Text Control Characters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Frames</source>
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
        <source>Display</source>
        <translation type="unfinished">Zobrazit</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Nástroje</translation>
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
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF Export</source>
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
        <source>Color Management</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
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
        <translation type="unfinished">Barva papíru</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Vyplnit plochu za hranicemi strany barvou okrajů</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Přizpůsobení barev</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Python skripty (*.py);; All Files (*)</translation>
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
        <source>None</source>
        <translation>Žádné</translation>
    </message>
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
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva obrysu písma</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Sytost barvy obrysu písma</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Nastavení barvy obrysu</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Vyrovnání znaků (kerning):</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ruční vyrovnání znaků (Kerning)</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Nastavení znaků</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation type="obsolete">Proklad:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuální nastavení prokládání textu</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished">Uložit stranu jako &amp;SVG...</translation>
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
        <translation type="unfinished">Import &amp;SVG...</translation>
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
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished">Uložit jako ša&amp;blonu...</translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished">Akce</translation>
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
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Soubor</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Hledám písma</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Váš systém neobsahuje žádná Postscriptová písma</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Končím</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Kritická chyba</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Inteligentní spojovník</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Zarovnat vlevo</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Zarovnat vpravo</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Zarovnat na střed</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Vložit číslo strany</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Připojit text ke křivce</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Zobrazit vrstvy</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Java skripty...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Předchozí stav (undo)</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Zobrazit paletu strany</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Zamknout/Odemknout</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Nedělitelná mezera</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Načítám nastavení</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Inicializace dělení slov</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Nastavení zkratek</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Načítám zápisník</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializace zásuvných modulů</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nový</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Otevřít...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Zavřít</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Uložit</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Uložit jako...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Vložit text/obrázek...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Informace o dokumentu...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Nastavení dokumentu...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Tisk...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Konec</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Vyjmout</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopírovat</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Vložit</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Vymazat</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Vybrat vše</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Barvy...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Styly...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Šablony...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Písma...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Vyberte nové písmo</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplikovat</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Smazat</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Seskupit</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Zrušit seskupení</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Zamknout</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Dát zcela dospodu</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Dát zcela navrch</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Dát o vrstvu níž</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Dát o vrstvu výš</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Umístit/zarovnat...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Vložit...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Smazat...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Přesunout...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Použít šablonu...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Ovládání vodítek...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Přizpůsobit oknu</translation>
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
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Miniatury</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Skrýt okraje</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Skrýt rámy</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Skrýt obrázky</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Ukázat mřížku</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Magnetická mřížka</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Nástroje</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Vlastnosti</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Obrys</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Zápisník</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Správa obrázků</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Dělení slov v textu</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">O Scribusu</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Online nápověda...</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Styl</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Podtržené</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Přeškrtnuté výš</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Kapitálky</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Horní index</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Dolní index</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Obkreslené</translation>
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
        <source>Ready</source>
        <translation>Připravený</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Žádné</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Vložit obrázek...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Barva</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Invertovat</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Vložit text...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Písmo</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Stín</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Odemknout</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Otevřít</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.scd);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Načítám...</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textové soubory (*.txt);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Nelze zapsat soubor: 
%1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Uložit jako</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenty (*.sla *.sla.gz *.scd *.scd.gz);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Ukládám...</translation>
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
        <source>Scribus Manual</source>
        <translation>Scribus manuál</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Chybí tyto programy:</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Všechny</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS soubory (*.eps);;Všechny soubory (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Načítám:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Přizpůsobení barev</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Anglicky</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Německy</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Španělsky</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italsky</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francouzsky</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Rusky</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dánsky</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovensky</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Maďarsky</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Česky</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Dánsky</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugalsky</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrajinsky</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polsky</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Řecky</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalánsky</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Vybrat adresář</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Zhroucení Scribusu</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus spadl díky signálu č. %1</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Včechny soubory (*)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finsky</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irsky</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Všechny podporované formáty</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Vytvořit nový dokument</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Otevřít dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Uložit aktuální dokument</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Zavřít aktuální dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Tisknout aktuální dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Uložit aktuální dokument jako PDF</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Zobrazit pomocnou mřížku</translation>
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
        <source>&amp;Item</source>
        <translation>O&amp;bjekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Str&amp;ana</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>Ná&amp;hled</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Nástroje</translation>
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
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Skrýt pomocnou mřížku</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Některé objekty jsou zamčené.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Zrušit</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Zamknout vše</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Odemknout vše</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litevsky</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Švédsky</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovinsky</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">Nas&amp;tavení</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">Správa &amp;barev...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Otevřít...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Otevřít &amp;předchozí</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zavřít</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Uložit</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Uložit j&amp;ako...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Návrat k u&amp;loženému</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Ad&amp;resář pro výstup...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">Vložit &amp;text/obrázek...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">&amp;Připojit text...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Import</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Uložit &amp;text...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Uložit stranu jako &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Uložit jako P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Export</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Na&amp;stavení dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Tisk...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Konec</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Zpět</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Vyjmou&amp;t</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopírovat</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Vy&amp;mazat</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Vybr&amp;at vše</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Hledat/Nahradit...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">&amp;Barvy...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">&amp;Styly odstavce...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Styly ča&amp;r...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">Ša&amp;blony...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Java skripty...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplikovat</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Vícenásobné duplikování</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Smazat</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Seskupit</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Zrušit seskupení</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Zam&amp;knout</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">&amp;Dát zcela dospodu</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Dát zcela &amp;navrch</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">Dát o &amp;vrstvu níž</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">Dát o v&amp;rstvu výš</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Umístit/z&amp;arovnat...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Tvar</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">Připojit &amp;text ke křivce</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">&amp;Odpojit text od křivky</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Kombinovat mnohoúhelníky</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">&amp;Rozdělit mnohoúhelníky</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">&amp;Převést na křivky</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Vložit...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Smazat...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Přesunout...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">Použí&amp;t šablonu...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">&amp;Přizpůsobit oknu</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">M&amp;iniatury</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Ukázat &amp;mřížku</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">&amp;Magnetická vodítka</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">&amp;Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Vlastnosti</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Obrysy</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Zápisník</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">V&amp;rstvy</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">&amp;Paleta strany</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Záložky</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Správa &amp;obrázků</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">&amp;Dělení slov v textu</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">&amp;Tipy pro nástroje</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">P&amp;DF nástroje</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Tipy pro nástroje</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Písma...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">Děl&amp;ení slov v textu...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">Správa &amp;klávesových zkratek...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">O &amp;Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">Scribus &amp;manuál...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;yl</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">V&amp;levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">Na &amp;střed</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">V&amp;pravo</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">Do &amp;bloku</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Vynucené</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Jiný...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Kaskáda</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Dlaždice</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Invertovat</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Vložit text...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Písmo</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efekty</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Zarovnání</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Stín</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabelátory...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Odemknout</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Zobrazit &amp;obrázky</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Zobrazit o&amp;kraje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Zobrazit &amp;rámy</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Zobrazit &amp;vodítka</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript: Nemůžete použít EPS obrázky</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Im&amp;portovat stranu(y)...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">M&amp;agnetická mřížka</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">N&amp;astavení...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importuji strany...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importovat stranu(y)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Zkoušíte importovat více stran, než je v dokumentu.&lt;/p&gt;Vyberte správnou činnost:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Vytvořit&lt;/b&gt; chybějící strany&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importovat&lt;/b&gt; strany až do konce&lt;/li&gt;&lt;li&gt;&lt;b&gt;Zrušit&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Vytvořit</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importovat</translation>
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
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Získat ICC profily</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">O&amp;vládání vodítek...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Stín:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">In&amp;formace o dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Zpět smazání objektu</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Zpět přesunutí objektu</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Zpět změna objektu</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">Upravit &amp;obrys</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Soubor %1 není ve Scribus formátu</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikánština</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Inicializován systém písem</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugalština (Braz.)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation type="obsolete">Váš systém neobsahuje žádná použitelná písma</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Správa barev je podporována, ale nemůže být použita. Ujistěte se, že máte nainstalovány ICC profily v adresáři, který je uveden v Nastavení...&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Správa barev není podporována (program není sestaven s její podporou).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Chorvatsky</translation>
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
        <source>Edit</source>
        <translation type="unfinished">Upravit</translation>
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
        <translation type="unfinished">Hladina</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;Možnosti PDF</translation>
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
        <translation type="unfinished">Znak</translation>
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
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postsript</translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Uložit jako</translation>
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
        <source>The following programs are missing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Bulharština</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Informace</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">chybí!</translation>
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
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
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
        <translation type="unfinished">Název:</translation>
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
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Příklad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Barva</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Nová vrstva</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normální</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Pozadí</translation>
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
        <source>All</source>
        <translation type="obsolete">Všechny</translation>
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
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopie</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Kopírovat sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Přesunout sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Zrušit</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Vložit</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="unfinished">Obrázek</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Soubor:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">Původní PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">Aktuální PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Řeťezený text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Textový rám</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Text na křivky</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Tisk:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Povoleno</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Zakázáno</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;Možnosti PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Přesu&amp;nout do hladiny</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">&amp;Hladina</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">K&amp;onverze na</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Varování</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Opravdu chcete smazat veškerý text?</translation>
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
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>od posledního uložení byl dokument změněný.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Za&amp;hodit</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Za&amp;vřít</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Uložit ihned</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation type="obsolete">&amp;Scribus skripty</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Vykonat skript...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Spustit Python skript ze souboru.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="obsolete">&amp;Nahrát rozšíření...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Nahrát makro nebo skript, který rozšiřuje samotný program.</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Použité skripty</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Ukázat &amp;konzoli</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Zobrazit Python konzoli, t.j. nástroj, ve kterém můžete spouštět příkazy jazyka Python.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;O skriptu...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;kript</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="obsolete">Nas&amp;tavení</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Otevřít</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python skripty (*.py);; All Files (*)</translation>
    </message>
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
        <source>Scribus - Script Plugin</source>
        <translation type="obsolete">Skriptovací modul</translation>
    </message>
    <message>
        <source>The &apos;Load Script&apos; function of the script plugin is currently disabled.
If you just want to run a normal script, you probably want to use
&apos;Execute Script...&apos; instead.

If you do actually want to load a Python extension script or macro, you
need to go into the Scripter Settings in the Script menu and enable
scripter extensions there.

Please read the documentation on extension scripts first.
</source>
        <translation type="obsolete">The &apos;Load Script&apos; function of the script plugin is currently disabled.
If you just want to run a normal script, you probably want to use
&apos;Execute Script...&apos; instead.
If you do actually want to load a Python extension script or macro, you
need to go into the Scripter Settings in the Script menu and enable
scripter extensions there.
Please read the documentation on extension scripts first.
</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Skrýt &amp;konzoli</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">O skriptu</translation>
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
        <source>Scribus - Scripter Preferences</source>
        <translation type="obsolete">Nastavení skriptování</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="obsolete">Povolit rozšířené</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="obsolete">Povolí makra a rozšiřující skripty</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Povolí použití rozšířené funkčnosti včetně Python maker a skriptu při spuštění. Lze použít pouze speciálně napsané skripty.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="obsolete">Použít skript při startu</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Je-li skript souštěny při startu aplikace neopatrně napsán, může způsobit závažné problémy.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="obsolete">Procházet...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="obsolete">Procházet soubory a hledat skript</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Vybrat nový skript&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="obsolete">Jaký skript má být spuštěn při startu aplikace</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Jaký skript má být spuštěn při startu aplikace. Pozn. v okamžiku, kdy je skript spouštěn, nemá ještě Scribus vytvořenou pracovní plochu.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="obsolete">Soubor:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+Z</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="obsolete">Zavřít bez uložení změn</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="obsolete">Uložit změny a zavřít</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Pokročilé možnosti</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="obsolete">Importovat celý namespace</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="obsolete">Spustit &apos;from scribus import *&apos;, když otevřete konzoli</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Spustit &apos;from scribus import *&apos;, když otevřete konzoli. Pokud nevíte, co to znamená, nechte volbu beze změny.&lt;/p&gt;
&lt;p&gt;Změna se projeví až po novém spuštěni aplikace.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="obsolete">Povolit stará jména funkcí, procedur a konstant</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Povolit používání konvence StarychJmen&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Pokud nevíte, jaké byly staré a již nahrazené Python funkce Scribusu, nechte beze změny.&lt;/p&gt;
&lt;p&gt;Změna se projeví až po novém spuštěni aplikace.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="obsolete">Použít simulaci stdin</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Nahradit sys.stdin falešným vstupním souborem, což zajistí to, že Scribus nezamrzne během pokusu o čtení dat ze stdin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Pokud nevíte, co sys.stdin udělá v GUI aplikaci, nechte beze změny.&lt;/p&gt;
&lt;p&gt;Běžné skripty mají sys.stdin ošetřený tak, že jej nepoužijí - čtení ze stdin vrátí prázdný řetězec. Jestliže je ale nastavený opravdový stdin, skript zablokuje celý Scribus až do doby, kdy je mu předána vstupní informace právě ze stdin. Scribus je většinou spoštěn mimo terminál, takže se uživateli výzva stdin nezobrazí, a ten si pak myslí, že program &quot;zamrnul&quot;.&lt;/p&gt;
&lt;p&gt;Když je volba vypnuta, je možné použít např. os.popen() a otevřít rouru operačního systému...&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Zobrazit náhledy pro strany</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Zobrazit názvy šablon</translation>
    </message>
    <message>
        <source>Show Master Page Names</source>
        <translation type="unfinished"></translation>
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
        <translation>Barva obrysu</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Odstín obrysu</translation>
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
        <source>None</source>
        <translation>Žádné</translation>
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
        <source>OK</source>
        <translation type="obsolete">OK</translation>
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
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Uspořádat strany</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Dostupné šablony:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Strany dokumentu:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Protilehlé stránky (Dvoustrany)</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">První strana vlevo</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Strany nebo šablony smažete přetáhnutím do odpadkového koše.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Zde jsou všechny vaše šablony; na vytvoření nové strany
přetáhněte šablonu do náhledu strany.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normální</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Náhled všech stran dokumentu.</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="unfinished"></translation>
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
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Stín</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Stín:</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Předložky a zkratky (nezlomitelná mezera)...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Předložky a zkratky</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words.</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <source>Warning</source>
        <translation>Varování</translation>
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
        <source>&amp;Append</source>
        <translation type="obsolete">Připoji&amp;t</translation>
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
        <translation type="obsolete">&amp;Uložit</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Opravdu chcete smazat tento styl?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Import</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Editor textu</translation>
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
        <source>Warning</source>
        <translation>Varování</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Opravdu chcete zahodit všechny změny?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Opravdu chcete smazat veškerý text?</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Aktualizovat textový rám</translation>
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
        <source>Update Text Frame and Exit</source>
        <translation>Aktualizovat textový rám a skončit</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Skončit bez aktualizace textového rámu</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nový</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Znovu nahrát původní text z rámu</translation>
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
        <translation>&amp;Aktualizovat textový rám a skončit</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Skončit &amp;bez aktualizace textového rámu</translation>
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
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">Vložit s&amp;peciální...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>Ak&amp;tualizovat textový rám</translation>
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
        <source>Load Text from File</source>
        <translation>Nahrát text ze souboru</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Uložit text do souboru</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Znovu nahrát původní text z rámu</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Hledat/Nahradit...</translation>
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
        <source>Search/Replace</source>
        <translation>Hledat/Nahradit</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Smazat veškerý text</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Chytrý výběr textu</translation>
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
        <translation type="unfinished"></translation>
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
        <translation>Podtržené</translation>
    </message>
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
        <source>Outline Text</source>
        <translation type="obsolete">Obrys textu</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Přeškrtnuté</translation>
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
        <source>Outline</source>
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
        <translation type="obsolete">Aktualizovat styly odstavce</translation>
    </message>
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
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Má importer přidávat jméno dokumentu před
jméno stylu odstavce ve Scribusu</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Má být existující Scribus styl změněn, jestliže v importovaném dokumentu
existuje styl se stejným jménem nebo se nemá změnit</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Nastavení importu OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Má si importer vždy pamatovat nastavenou hodnotu
a nikdy znovu nevyžadovat potvrzení</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="obsolete">Zabalit styly odstavce</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="obsolete">Setřídit styly odstavce podle atributů.
Výsledkem je méně stylů, ale mohou vzniknout problémy
s jejich nastavením. Používejte, jestliže víte, že text nemusíte
příliš upravovat.</translation>
    </message>
    <message>
        <source>Update Paragraph Styles</source>
        <translation type="obsolete">Aktualizovat styly odstavce</translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Připojit</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+V</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Smazat</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+S</translation>
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
        <source>Check for placed PDF Files</source>
        <translation type="unfinished"></translation>
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
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Barva:</translation>
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
        <source>Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished">Zobrazit pomocnou mřížku</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Pomocná &amp;mřížka:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">&amp;Odstup pomocné mřížky:</translation>
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
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished">Vzdálenost mezi čarami vedlejší mřížky</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Vzdálenost mezi čarami hlavní mřížky</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Vzdálenost, ve které se čára mřížky chová vůči objektu jako magnet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Poloměr oblasti, kterou Scribus považuje za oblast daného objektu</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Barva čar vedlejší mřížky</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Barva čar hlavní mřížky</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Barva vodítek, které vkládáte manuálně</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Barva okrajových čar</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
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
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Správa tabelátorů</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Exportovat rozsah</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">Všechny str&amp;any</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">&amp;Vybrat strany</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotace:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Nastavení souboru</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">&amp;Kompatibilita:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">Vaz&amp;ba:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Levý okraj</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Pravý okraj</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Vytvořit &amp;náhledy</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Uložit &amp;propojené textové rámy jako PDF články</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">Vče&amp;tně záložek</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="unfinished">Nastavení obrázků</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automaticky</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="unfinished">Z&amp;působ:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Maximální</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Vysoká</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Střední</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Nízká</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Minimální</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">&amp;Všeobecné</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">V&amp;ložit všechna písma</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">Čá&amp;stečné zařazení písem (subset)</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Vkládání</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Dostupná písma:</translation>
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
        <translation type="unfinished">Písma k vložení:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Částečně zařazená písma (subset):</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Písma</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Strana</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">Zobrazit &amp;náhledy stran</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Efekty</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">&amp;Doba zobrazení:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Tr&amp;vání efektu:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">&amp;Typ efektu:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">&amp;Přesouvání řádků:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">O&amp;d:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">&amp;Směr:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished">s</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Bez efektu</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">Pruhy</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Rám</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Rozpuštění</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">Lesk</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">Rozdělit</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">Setřít</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Vodorovně</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Svisle</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Zevnitř</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Zvenku</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Zleva doprava</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Shora dolů</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Zdola nahoru</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Zprava doleva</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Zleva nahoře doprava dolů</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">Po&amp;užít efekt na všechny strany</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">Použít ši&amp;frování</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Hesla</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;Uživatel:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">&amp;Vlastník:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Nastavení</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">&amp;Povolit tisk dokumentu</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Povolit z&amp;měny dokumentu</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Povolit &amp;kopírování textu a grafiky</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Povolit přidávání &amp;anotací a polí formulářů</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">&amp;Bezpečnost</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Všeobecné</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">&amp;Plánovaný výstup na:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Monitor / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Tiskárna</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">&amp;Použít vlastní nastavení reprodukce</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Nastavení reprodukce</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">&amp;Frekvence:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">Úhe&amp;l:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">Funkce &amp;bodu:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Prostá tečka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Čára</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">Kruh</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Elipsa</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Plné barvy (Solid colors):</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Použít ICC profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished">Účel generování:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished">Perceptuální (fotografická) transformace</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Relativní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Sytost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Absolutní kolorimetrická transformace</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Obrázky:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">Nepoužívat vložené ICC profily</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">&amp;Barva</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">Výstup do PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">&amp;Info text:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">&amp;Výstupní profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Vlastní formát stránky</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished">Vložit písma do PDF. Vložením písem zajistíte
stejný vzhled dokumentu v jiných prohlížečích.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Ukázat náhled každé strany uvedené v seznamu nahoře.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Jak dlouho je strana zobrazena, než je spuštěn přechod na dalsší.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Jak dlouho běží prezentační efekt.
Menší hodnota efekt zrychlí, větší zpomalí.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Typ efektu.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Směr efektu Přesouvání řádků nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Startovní pozice efektu Rám nebo Rozdělit.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Směr efektu Lesk nebo Setřít.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Použít vybraný efekt na všechny strany.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Export všech stran do PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Export rozsahu stránek do PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Vložte čárkou oddělený seznam položek, kde
položka může být *, t.j. všechny strany, 1-5,
t.j. interval, nebo jediné číslo strany.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Určuje vazbu stran v PDF. Jestliže nepotřebujete speciální
nastavení, nechte implicitní volbu.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished">Vytvoří náhledy všech stran v PDF.
Některé prohlížeče náhledy použijí k navigaci.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Vytvořit PDF články, což umožňuje navigaci odkazy v PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Vložit záložky obsažené v dokumentu.
Ty jsou užitečné k navigaci v obsáhlých PDF dokumentech.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished">Rozlišení textu a vektorové grafiky při exportu. Neovlivňuje 
rozlišení bitmapových obrázků, jako jsou např. fotografie.</translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="unfinished">Úrovňe komprese: Minimální (25%), Nízká (50%), Střední (75%), Vysoká (85%), Maximální (95%)</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">DPI (Body na palec) exportu obrázků.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="unfinished">Zvolte heslo, které povolí nebo zakáže všechny
bezpečnostní vlastnosti exportovaného PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Zvolit heslo, které musí použít čtenář PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Povolit tisk PDF. Jestliže není zatrženo, tisk není povolen.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Povolit modifikaci PDF. Jestliže není zatrženo, modifikace jsou zakázány.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="unfinished">Povolit kopírování textu a grafiky z PDF.
Jestliže není zatrženo, text a grafika nelze kopírovat.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="unfinished">Povolit přidávání poznámek a polí formulářů do PDF.
Jestliže není zatrženo, přidávání je zakázáno.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished">Barevný model výstupu PDF.
Zvolte Monitor/Web na PDF, které je publikováno na monitoru nebo na typické kancelářské tiskárně.
Zvolte Tiskárna, bude-li PDF tištěno na CMYK tiskárně.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">Pokročilá volba, která není implicitně povolena. Může být použita, jestliže to vyžaduje vaše tiskárna.
Pozor. vámi exportované PDF soubory se nemusí korektně tisknout na jiné tiskárně
a nebudou plně přenositelné mezi systémy.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished">Vložit barevný profil plných barev</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Barevný profil plných barev</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished">Účel reprodukce plných barev</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Vložit barevný profil obrázků</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">Nepoužívat barevný profil vložený ve zdrojových obrázcích</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Barevný profil obrázků</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Účel reprodukce obrázků</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Výstupní profil tisku.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Volitelný text, jehož vyplněním bude PDF nebo PDF/X-3 splňovat
specifikaci. Doporučujeme použít titul dokumentu.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Vzdálenost ořezu od horního okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Vzdálenost ořezu od dolního okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Vzdálenost ořezu od levého okraje fyzické strany</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Vzdálenost ořezu od pravého okraje fyzické strany</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Zrcadlit stranu(y) horizontálně</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Zrcadlit stranu(y) vertikálně</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Písmo:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Velikost:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <source>Tab Fill Character:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tab Width:</source>
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
        <translation type="unfinished">Příliš žluťoučký kůň úpěl ďábelské Ódy</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">Ba&amp;rva čáry:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Stín:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">&amp;Barva výplně:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">Stí&amp;n:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Šíř&amp;ka čáry:</translation>
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
        <translation type="unfinished">Vo&amp;lná změna velikosti</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">&amp;Vodorovné zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">V&amp;ertikální zvětšení:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">Změnit velikost &amp;obrázku na rozměr rámu</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">Dod&amp;ržet poměr stran</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">&amp;Barva výplně:</translation>
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
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">&amp;Maximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Krokování:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Vlastnosti textových rámců</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Vlastnosti obrázkových rámců</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Vlastnosti kreslení obrysů</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Vlastnosti úrovně zvětšení</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Vlastnosti čar</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Vlastnosti mnohoúhelníků</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Písmo nového textového rámu</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished">Velikost písma nového textového rámu</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Barva písma</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Počet sloupců v textovém rámu</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Mezera mezi sloupci textového rámu</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Ukázka písma</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Obrázkové rámy mohou libovolně měnit rozměry obrázku</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Horizontální zvětšení obrázků</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Vertikální zvětšení obrázků</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished">Držet stejné horizontální a vertikální zvětšení</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Obrázky budou deformovány podle rozměrů rámu</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Automaticky nastavovaná velikost obrázků dodržuje originální proporce</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Barva výplně obrázkových rámů</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Sytost barvy výplně</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Barva čar obrysů</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Sytost barvy čar</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Barva výplně obrysů</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Styl čar obrysů</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Tloušťka čar obrysů</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Minimální povolené zvětšení (zmenšení)</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Maximální povolené zvětšení</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">Změna zvětšení - krok operace lupou</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Barva čar</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Sytost barvy</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Styl čar</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Šířka čár</translation>
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
        <translation type="unfinished">Vlastní</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Dolní index</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">&amp;Posunutí:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">Z&amp;většení:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Horní index</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Po&amp;sunutí:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">Zvě&amp;tšení:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Podtržené</translation>
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
        <translation type="unfinished">Šířka čáry:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">Přeškrtnuté výš</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Kapitálky</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">Zvětše&amp;ní:</translation>
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
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished">Posunutí nad účaří</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Relativní velikost horního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Posunutí pod účaří</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Relativní velikost dolního indexu vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Relativní velikost kapitálek vůči normální velikosti písma</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Procentuální zvětšení řádkování podle velikosti písma</translation>
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
        <translation>Vlevo</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Vpravo</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Tečka</translation>
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
        <translation>Odsazení první řádky odstavce</translation>
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
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Pozice:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">P&amp;rvní řádek:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">Levé &amp;odsazení:</translation>
    </message>
    <message>
        <source>None</source>
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
        <translation type="unfinished">Vlastní</translation>
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
        <translation>Přehled</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Prvek</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Informace</translation>
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
        <translation type="obsolete">Písmo:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Obrázek</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Text</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Čára</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Mnohoúhelník</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Lomená čára</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Text na křivky</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Strana</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Varování</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Název &quot;%1&quot; není jedinečný.
Vyberte, prosím, jiný.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Seskupit</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Strana</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Seskupit</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Vytvořit</translation>
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
        <translation type="unfinished">Zrušit</translation>
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
        <translation type="unfinished">Zamknout</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Odemknout</translation>
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
        <translation type="unfinished">Smazat</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Přejmenovat</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="unfinished">Vložit</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Vyjmout</translation>
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
        <source>Set font height</source>
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
        <translation type="unfinished">Mnohoúhelník</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Lomená čára</translation>
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
        <source>Import OpenOffice.org Draw image</source>
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
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation type="unfinished"></translation>
    </message>
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
        <translation type="unfinished">&amp;Zpět</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Vpřed</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
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
    <name>VlnaDialog</name>
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
        <translation>&amp;Celý dokument</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Zrušit</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Info a jazyky</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Nahradit původní nastavení uživatelským</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Jestliže existuje soubor
(%1)
můžete zvolit, zda chcete přidat jeho konfiguraci
k již existující - nezaškrtnuté. Zaškrtnutím předdefinované
hodnoty nahradíte.</translation>
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
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Předložky a zkratky pro Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>Dostupné v těchto jazycích</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>O předložkách a zkratkách</translation>
    </message>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">Ú&amp;pravy</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">Nápo&amp;věda</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Předložky a zkratky</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Nástroje</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Vybrat položky</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Otočit položku</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Vložit textový rámec</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Upravit obsah rámu</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Vložit obrázek</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Vlastnosti...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Vložit mnohoúhelníky</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Vložit čáry</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Vložit Beziérovy křivky</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Vložit čáru kreslenou volnou rukou</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Vytvořit řetězení textu</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Přerušit řetězení textu</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Vložit tabulku</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Zvětšit nebo zmenšit</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Editace textu</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Kreslit různé tvary</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Měřit</translation>
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
        <translation>Vložit PDF poznámky</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Vybrat znak:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Vložit znak na pozici kurzoru v textu</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Smazat aktuální výběr(y).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Zavřete dialog a vraťte se k editaci textu.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Vložit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Vy&amp;mazat</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zavřít</translation>
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
        <source>Get text only</source>
        <translation type="obsolete">Pouze text</translation>
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
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Pamatovat si příponu souboru tohoto typu jako asociaci
a neptat se znovu na typ importu.</translation>
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
        <translation>Velikost strany</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
        <translation>Jestliže odstraníte šablonu z aplikace, odstraníte pouze záznam z template.xml. Soubory zůstanou na disku. Menu s mazací položkou se zobrazí pouze tehdy, jestliže máte právo měnit soubor template.xml.</translation>
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
        <translation>Velikost strany</translation>
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
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Zrušit</translation>
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
