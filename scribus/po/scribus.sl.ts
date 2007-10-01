<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Vrne spisek vseh barv, ki so določene v dokumentu.
Če ni odprt noben dokument, vrne spisek privzetih barv.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Prikaže pogovorno okno &quot;Nov dokument&quot;. Ustvari nov dokument, če uporabnik
sprejme nastavitve. Ne ustvari novega dokumenta, če uporabnik
prekliče ukaz. Vrne vrednost true, če je bil novi dokument ustvarjen.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;ime&quot;]) -&gt; string

Vrne ime polnilne barve predmeta &quot;ime&quot;.
Če &quot;ime&quot; ni podano, se uporabi trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>getFillColor([&quot;ime&quot;]) -&gt; string

Premakne predmet &quot;ime&quot; za dx in dy relativno na trenuten položaj.
Razdalje so podane v enotah trenutnega dokumenta (glejte konstante
UNIT). Če &quot;ime&quot; ni podano, se uporabi trenutno izbrani predmet.
Če je predmet &quot;ime&quot; del skupine, se premakne celotna skupina.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Izključi ponovno risanje strani, ko je bool = False, sicer je ponovno risanje
vključeno. Ta sprememba obvelja, čeprav je skripta konec, zato preverite,
da pokličete setRedraw(True) v stavku finally: na vrhnji ravni svojega skripta.</translation>
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
        <translation>createRect(x, y, širina, višina, [&quot;ime&quot;]) -&gt; string

Ustvari nov pravokotnik na trenutni strani in vrne njegovo ime.
Koordinate so podane v trenutnih merskih enotah dokumenta
(glejte konstante UNIT). &quot;ime&quot; mora biti enkraten imenovalec predmeta,
saj bo služil za sklicevanje na ta predmet v prihodnje. Če &quot;ime&quot;
ni podano, bo Scribus ustvaril naključno ime.

Lahko vrne NameExistsError, če izrecno navedete ime, ki je že v uporabi.</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(vrsta, &quot;barva1&quot;, senca1, &quot;barva2&quot;, senca2, [&quot;ime&quot;])

Nastavi polnilo preliva predmeta &quot;ime&quot;. Opisi barv so enaki kot pri
setFillColor() in setFillShade(). Oglejte si konstante za vrste,
ki so na voljo (FILL_&lt;vrsta&gt;).</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;ime&quot;]) -&gt; float

Vrne velikost pisave v točkah za besedilni okvir &quot;ime&quot;. Če ima ta besedilni
okvir izbrano besedilo, bo vrnjena vrednost, dodeljena prvemu znaku izbire.
Če &quot;ime&quot; ni podano, je uporabljen trenutno izbran element.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;niz&quot;)

Zapiše &quot;niz&quot; v vrstico sporočil Scribusa (vrstica stanja). Besedilo
mora biti kodirano v nizu UTF8 ali &apos;unicode&apos; (priporočeno).</translation>
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
        <translation>newPage(kje [,&quot;glavnastran&quot;])

Ustvari novo stran. Če je &quot;kje&quot; -1, se nova stran pripne dokumentu, sicer je
nova stran vstavljena pred &quot;kje&quot;. Številke strani se štejejo od 1 navzgor,
ne glede na to, ali je prikazana številka prve strani dokumenta ali ne.
Dodatni parameter &quot;glavnastran&quot; določa ime glavne strani za novo stran.

Če je številka strani zunaj obsega, lahko vrne IndexError</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;niz&quot;)

&quot;niz&quot; mora biti veljavno ime datoteke slike SVG. Besedilo
mora biti kodirano v UTF8 ali &apos;unicode&apos; (priporočeno).</translation>
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
        <translation>newDocument(velikost, robovi, postavitev, številkaPrveStrani,
                        merskaEnota, vrstaStrani, zaporedjePrveStrani) -&gt; bool

Ustvari nov dokument in vrne true, če je uspelo. Parametri imajo naslednji pomen:

velikost = Skupek mer (širina, višina), ki opisuje velikost dokumenta. Uporabite
lahko vnaprej določene konstante, poimenovane PAPER_&lt;vrsta_papirja&gt;,
npr. PAPER_A4 itn.

robovi = Skupek mer (levo, desno, zgoraj, spodaj), ki opisuje robove dokumenta.

postavitev = postavitev strani - konstanti PORTRAIT in LANDSCAPE

številkaPrveStrani = je številka prve strani v dokumentu, uporabljena za
oštevilčevanje strani. Čeprav najpogosteje hočete 1, je uporabno za višje
številke, če ustvarjate dokument v več delih.

merskaEnota: ta vrednost nastavi merske enote, ki jih uporablja dokument. Uporabite
vnaprej določene konstante, eno izmed: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICAS, UNIT_POINTS.

vrstaStrani = Ena od vnaprej določenih konstant PAGE_n. PAGE_1 je posamična stran,
PAGE_2 je za dvostranske dokumente, PAGE_3 je za tristransko zgibanko in
PAGE_4 je za štiristransko zgibanko.

zaporedjePrveStrani = Kateri položaj ima prva stran dokumenta.
Indeksirano od 0 (0 = prva).

številoStrani = Število strani, ki naj bodo ustvarjene.

Vrednosti za širino, višino in robove so izražene v danih merskih enotah
dokumenta. Konstante PAPER_* so izražene v točkah. Če vaš dokument
ni v točkah, poskrbite, da bo tako.

primer: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

Lahko vrne napako ScribusError, če je firstPageOrder večja od dovoljenega z pagesType.</translation>
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
        <translation>valueDialog(napis, vsebina [,privzetavrednost]) -&gt; string

Pokaže pogosto pogovorno okno &apos;Vprašaj za niz&apos; in vrne njegovo vrednost kot niz.
Parametri: naslov okna, besedilo v oknu in neobvezna &apos;privzeta&apos; vrednost.

Primer: valueDialog(&apos;naslov&apos;, &apos;besedilo v oknu&apos;, &apos;neobvezno&apos;)</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Zapre trenutni dokument brez poprejšnje ponudbe za shranjevanje.

Lahko vrne napako NoDocOpenError, če dokumenta za zapiranje ni</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Vrne vrednost true, če je dokument odprt.</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;ime&quot;)

Odpre dokument &quot;ime&quot;.

Lahko vrne napako ScribusError, če dokumenta ni mogoče odpreti.
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

Shrani trenutni dokument z njegovim trenutnim imenom, vrne true, če je uspešno.
Če dokument še ni bil shranjen, lahko to prikliče interaktivno pogovorno okno
za shranjevanje.

Če shranjevanje ne uspe, tega ni mogoče ugotoviti.</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;ime&quot;)

Shrani trenutni dokument pod novim imenom &quot;ime&quot; (ki je lahko polna ali
relativna pot).

Lahko vrne ScribusError, če shranjevanje ne uspe.</translation>
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
        <translation>setUnit(vrsta)

Spremeni merske enote dokumenta. Možne vrednosti za &quot;enoto&quot; so
določene kot konstante UNIT_&lt;vrsta&gt;.

Lahko vrne napako ValueError, če je posredovana neveljavna enota.</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (konstanta enot Scribus)

Vrne merske enote dokumenta. Vrnjena vrednost bo ena izmed
konstant UNIT_*:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;imedatoteke&quot;)

Naloži sloge odstavkov iz dokumenta Scribus &quot;imedatoteke&quot; v
trenutni dokument.</translation>
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
        <translation>getLineColor([&quot;ime&quot;]) -&gt; string

Vrne ime barve črte predmeta &quot;ime&quot;.
Če &quot;ime&quot; ni podano, uporabi trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;ime&quot;]) -&gt; integer
Vrne širino črte predmeta &quot;ime&quot;. Če &quot;ime&quot;
ni podano, bo uporabljen trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;ime&quot;]) -&gt; integer

Vrne vrednost senčenja za barvo črte predmeta &quot;ime&quot;.
Če &quot;ime&quot; ni podano, je uporabljen trenutno izbrani predmet.</translation>
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
        <translation>getLineStyle([&quot;ime&quot;]) -&gt; integer (glej konstante)

Vrne slog črte predmeta &quot;ime&quot;. Če &quot;ime&quot; ni podano, je uporabljen
trenutno izbrani predmet. Konstante sloga črte so:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;ime&quot;]) -&gt; integer

Vrne vrednost senčenja za barvo polnila predmeta &quot;ime&quot;.
Če &quot;ime&quot; ni podano, bo uporabljen trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;ime&quot;]) -&gt; (x,y)

Vrne par (x, y), ki vsebuje vrednosti spremembe merila za okvir slike
&quot;ime&quot;.  Če &quot;ime&quot; ni podano, je uporabljen trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;ime&quot;]) -&gt; string

Vrne ime datoteke slike v okviru slike. Če &quot;ime&quot; ni
podano, je uporabljen trenutno izbrani predmet.</translation>
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
        <translation>getRotation([&quot;ime&quot;]) -&gt; integer

Vrne zasukanost predmeta &quot;ime&quot;. Vrednost je izražena v stopinjah,
in sicer je v smeri urinega kazalca vrednost pozitivna. Če &quot;ime&quot; ni podano,
bo uporabljen trenutno izbran element.</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Vrne seznam z imeni vseh predmetov na trenutni strani.</translation>
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
        <translation>sizeObject(širina, višina [, &quot;ime&quot;])

Spremeni velikost predmeta &quot;ime&quot; na podano širino in višino. Če &quot;ime&quot;
ni podano, je uporabljen trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([št]) -&gt; string

Vrne ime izbranega predmeta. Če je podana, predstavlja &quot;št&quot; številko
izbranega predmeta; 0 npr. pomeni prvi izbrani predmet, 1 drugi
izbrani predmet itn.</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Vrne število izbranih predmetov.</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;ime&quot;)

Izbere predmet z danim &quot;imenom&quot;.</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Prekliče izbiro vseh predmetov v celem dokumentu.</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(seznam)

Združi predmete s &quot;seznama&quot; v skupino. &quot;Seznam&quot; mora vsebovati imena
predmetov, ki naj bodo združeni. Če &quot;seznam&quot; ni podan, so uporabljeni
trenutno izbrani predmeti.</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;ime&quot;)

Razdruži skupino predmetov, kateri pripada predmet &quot;ime&quot;. Če &quot;ime&quot; ni podano,
je uporabljen trenutno izbrani predmet.</translation>
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
        <translation>lockObject([&quot;ime&quot;]) -&gt; bool

Zaklene predmet &quot;ime&quot;, če ni zaklenjen, ali ga odklene, če je zaklenjen.
Če &quot;ime&quot; ni podano, je uporabljen trenutno izbrani predmet. Vrne true,
če je zaklenjen.</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;ime&quot;]) -&gt; bool

Vrne true, če je predmet &quot;ime&quot; zaklenjen.  Če &quot;ime&quot; ni podano, je
uporabljen trenutno izbrani predmet.</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Vrne seznam z imeni vseh pisav, ki so na voljo.</translation>
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
        <translation>getLayers() -&gt; list

Vrne seznam z imeni vseh obstoječih plasti.</translation>
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
        <translation>getActiveLayer() -&gt; string

Vrne ime trenutno aktivne plasti.</translation>
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
        <translation>createLayer(plast)

Ustvari novo plast z imenom &quot;ime&quot;.

Lahko vrne napako ValueError. če ime plasti ni sprejemljivo.</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Vrne niz z vrednostjo -lang.</translation>
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
        <translation>deleteObject([&quot;ime&quot;])

Izbriše element z imenom &quot;ime&quot;. Če &quot;ime&quot; ni podano, je zbrisan
trenutno izbrani element.</translation>
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
        <translation>setStyle(&quot;slog&quot; [, &quot;ime&quot;])

Uporabi podani &quot;slog&quot; na predmetu z imenom &quot;ime&quot;. Če ime predmeta
ni podano, velja za izbrani predmet.</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Vrne seznam imen vseh slogov odstavkov v trenutnem dokumentu.</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Vrne številko trenutne strani. Številke strani se štejejo od 1
naprej, ne glede na to, kaj je prikazano na prvi strani vašega dokumenta.</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Ponovno izriše vse strani.</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;ime&quot;)

Shrani trenutno stran kot EPS v datoteko &quot;ime&quot;.

Lahko vrne napako ScribusError, če shranjevanje ni uspelo.</translation>
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
        <translation>pageCount() -&gt; integer

Vrne število strani dokumenta.</translation>
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
        <translation>getVGuides()

Glejte getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Glejte setHGuides.
</translation>
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
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation>closeMasterPage()

Zapre trenutno aktivno glavno stran, če je kakšna odprta, in povrne
v navadno urejanje. Urejanje glavne strani začnete z editMasterPage().</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation>masterPageNames()

Vrne seznam imen vseh glavnih strani v dokumentu.</translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation>editMasterPage(imeStrani)

Omogoči urejanje glavne strani in odpre imenovano glavno stran
za urejanje. Urejanje zaključite z closeMasterPage().</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(imeStrani)

Ustvari novo glavno stran z imenom pageName in jo odpre
za urejanje.</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(imeStrani)

Izbriše imenovano glavno stran.</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(double)

Poveča oz. pomanjša dokument v glavnem oknu vmesnika. Dejanja zahtevajo
celoštevilčne vrednosti, npr. 20,0, 100,0, itn. Prilagodi oknu uporablja -100.</translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>setInfo(&quot;avtor&quot;, &quot;info&quot;, &quot;opis&quot;) -&gt; bool

Dokumentu nastavi njegove podatke. &quot;Avtor&quot;, &quot;Info&quot; in &quot;Opis&quot; so nizi.</translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation>duplicateObject([&quot;ime&quot;]) -&gt; string

Ustvari dvojnika izbranega predmeta (ali izbrane skupine).</translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Id izgradnje:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finščina</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brazilščina</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Različica Scribusa %1
 %2 %3</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galicijščina:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Češčina:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Valižanščina:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Avtorji prispevkov:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Poljščina:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrajinščina:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baskovščina:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovaščina:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italijanščina:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Danščina:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Litvanščina:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Različica za Windows:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turščina:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Ruščina:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovenščina:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Madžarščina:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Francoščina:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bolgarščina:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norveščina:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Nemščina:</translation>
    </message>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">O Scribusu%1%2</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Tu lahko vidite različico, datum priprave in\n
v Scribus vključene knjižnice\n
C-C-T pomeni podporo za: C=CUPS C=littlecms T=TIFF.\n
Manjkajoča knjižnica je označena z zvezdico ( * )</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;O programu</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;vtorji</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Prevajalci</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Na spletu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Razvijalci:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Uradna dokumentacija:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Druga dokumentacija:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Angleščina (Britanska):</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Švedščina:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Domača stran</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Spletna pomoč</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Poročila o napakah in predlogi</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Dopisni seznam</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Uradni prevodi in prevajalci:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Korejščina:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Srbščina:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Španščina:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Nekdanji prevajalci:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katalonščina:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>O Scribusu %1</translation>
    </message>
    <message>
        <source>Portugese (Brazilian):</source>
        <translation type="obsolete">Portugalščina (Brazilska):</translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation type="obsolete">Afrikaansko:</translation>
    </message>
    <message>
        <source>Dutch:</source>
        <translation type="obsolete">Nizozemščina:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Tu vidite različico, datum izgradnje in
podporo knjižnicam v Sckribusu
C-C-R-F pomenijo C=littlecms C=CUPS T=TIFF F=Fontconfig.
Manjkajoče knjižnice so prikazane s *</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1 %2 %3</translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="obsolete">Uporabljena GhostScript različica %1</translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="obsolete">Različica GS ni dostopna</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="obsolete">Scribus različica %1
%2 %3 (%4)</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="obsolete">Mac OSX Aqua različica:</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="obsolete">Okenska različica:</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Tu vidite natančno različico Scribusa, datum izgradnje in podporo knjižnic. C-C-T-F pomeni C=littlecms, C=CUPS, T=TIFF, F=Fontconfig. Zadnja črka označuje upodobljevalnik C=Cairo ali A=libart. Manjkajoče knjižnice so predstavljene z *. Vidite lahko tudi zaznano različico Ghostscripta.</translation>
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
        <translation>Uporabljena različica Ghostscript-a %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Na voljo ni nobene različice Ghostscript-a</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus različice %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>ID gradnje:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Tu vidite natančno različico Scribusa, datum izgradnje in podporo knjižnicam. C-C-T-F pomeni C=littlecms, C=CUPS, T=TIFF, F=Fontconfig. Zadnja črka označuje upodobljevalnik C=Cairo ali A=libart. Manjkajoče knjižnice so predstavljene z *. Vidite lahko tudi zaznano različico Ghostscripta.</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Različica Mac OS&amp;#174; X Aqua:</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Različica Windows&amp;#174; :</translation>
    </message>
    <message>
        <source>March</source>
        <translation type="obsolete">Marec</translation>
    </message>
    <message>
        <source>May</source>
        <translation type="obsolete">Maj</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation>Ikone projekta Tango:</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>Različica OS/2&amp;#174;/eComStation&amp;#8482;:</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Ime datoteke:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Različica:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Vključeno:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Datum izzida:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Opis:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Avtorj(i):</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Licenca:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: O vtičnikih</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation type="obsolete">Ime datoteke:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="obsolete">Različica:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation type="obsolete">Vključeno:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation type="obsolete">Datum izzida:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="obsolete">Copyright:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="obsolete">Avtorj(i):</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="obsolete">Opis:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="obsolete">Licenca:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
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
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Odpri ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot ...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Povrni na shranjeno</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Zberi za &amp;izvoz ...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Pridobi besedilo ...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Dodaj &amp;besedilo ...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Pridobi sliko ...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Shrani &amp;besedilo ...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Shrani stran kot &amp;EPS ...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Shrani kot &amp;PDF ...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Nastavitve dokumenta ...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Na&amp;tisni ...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Končaj</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ponovi</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>Način obdelave &amp;predmetov</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">&amp;Počisti vsebino</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Izberi &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Izprazni izbiro</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Najdi in zamenjaj ...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Uredi sliko ...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Barve ...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Slogi odstavka ...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Slogi &amp;črt ...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Glavne strani ...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Java skripti...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Nastavitve ...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 točk</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Drugo ...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Sredinsko</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Desno</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Vsiljeno</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Običajno</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Podčrtano</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Podčrtaj &amp;besede</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Prečrtano</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Velike črke</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>Pomanjšane &amp;velike črke</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Nadpisano</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Po&amp;dpisano</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Obroba</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Senca</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>Slikovni učink&amp;i</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorji ...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Večkratno podvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Združi</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Razdruži</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Je &amp;zaklenjeno</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Velikost je &amp;zaklenjena</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Spusti na &amp;dno</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Dvigni na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Dvigni</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Pošlji v &amp;skicirko</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>L&amp;astnosti ...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Slika je vidna</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Posodobi sliko</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Okvir prilagodi sliki</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Razširjene lastnosti slike</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Nizka ločljivost</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Običajna ločljivost</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Polna ločljivost</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Je &amp;zaznamek PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>je &amp;opomba PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Lastnosti opombe</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Lastnosti &amp;polja</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Uredi obliko ...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Besedilo &amp;pripni na pot</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Besedilo &amp;snemi s poti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Sestavi poligone</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Razdeli &amp;poligone</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Krivulja &amp;Bezier</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>Okv&amp;ir slike</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Obrobe</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Mnogokotnk</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Okvir besedila</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Znak ...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Vzorčno besedilo</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Vstavi ...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;Uvozi ...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>I&amp;zbriši ...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopiraj ...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Premakni ...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Uporabi glavno stran ...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Upravljaj z &amp;vodili ...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Upravljaj z lastnostmi strani ...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>Prilagodi &amp;oknu</translation>
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
        <translation>&amp;Sličice predogleda</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Pokaži &amp;robove</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Pokaži &amp;okvire</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Pokaži &amp;slike</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Pokaži &amp;mrežo</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Pokaži &amp;vodila</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Pokaži &amp;osnovno mrežo</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Pokaži &amp;verige besedil</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Pokaži ukazne znake</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Vodila glede na stran</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Pripni na mrežo</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Pripni na vodila</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Lastnosti</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Skicirka</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Plasti</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Uredi strani</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Zaznamki</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Meritve</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Zgodovina dejanj</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>Končno pre&amp;verjanje</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>R&amp;azporedi in poravnaj</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Orodja</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Orodja &amp;PDF</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Izberi predmet</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abela</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Lik</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Črta</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Prostoročna črta</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Zasukaj predmet</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Povečaj ali pomanjšaj</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Povečaj</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Uredi vsebino okvira</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Uredi besedilo ...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Poveži besedilne okvire</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Razveži besedilne okvire</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Izbira barve</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopiraj lastnosti predmeta</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Uredi besedilo z urejevalnikom zgodbe</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Vstavi besedilni okvir</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Vstavi slikovni okvir</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vstavi tabelo</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Vstavi lik</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Vstavi mnogokotnik</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Vstavi črto</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Vstavi krivuljo bezier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Vstavi prostoročno črto</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Upravljaj s slikami</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>
&amp;Deli besede</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Odstrani deljenje besed</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Ustvari kazalo vsebine</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>O &amp;QT-ju</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Namigi orodja</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Pr&amp;iročnik za Scribus ...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Pametno &amp;deljenje</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Neprelomen pomišljaj</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Neprelomen pre&amp;sledek</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;Številka strani</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Nova vrstica</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Prelom okvira</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Prelom stolpca</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Zaščitena blagovna znamka</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Blagovna znamka</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Oznaka</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Pomišljaj M</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Pomišljaj N</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Vezaj števil</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Narekovalni pomišljaj</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Opuščaj</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Ravni dvojni</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Levi enojni</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Desni enojni</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Dvojni levi</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Dvojni desni</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Obratni enojni</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Obratni dvojni</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Enojen levi guillemet </translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Enojen desni guillemet </translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Dvojni levi guillemet</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Dvojni desni guillemet</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Nizka enojna vejica</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Nizka dvojna vejica</translation>
    </message>
    <message>
        <source>Double Turned Comma</source>
        <translation type="obsolete">Dvojna obrnjena vejica</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>Levi enojni CJK</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>Desni enojni CJK</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>Dvojni levi CJK</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>Dvojni desni CJK</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Preklopi palete</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Preklopi vodila</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Pred&amp;ogled tiskanja</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaSkripti ...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Pretvori v glavno stran ...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Kaskadno</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Tlakovano</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;O vtičnikih</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Več informacij ...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Tiskanje omogočeno</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>&amp;Prezrcali vodoravno</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>&amp;Prezrcali navpično</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Pokaži ravnila</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Oris</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Poševnica</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Vmesna pika</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Presledek N</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Presledek M</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Ozek presledek</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Širok presledek</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Vmesni presledek</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Las tanek presledek</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Vstavi pameten vezaj</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Vstavi nedeljiv pomišljaj</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Vstavi nedeljiv presledek</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Vstavi številko strani</translation>
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
        <translation>&amp;Slogi ...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Oris</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Orisi</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Prilepi (&amp;absolutno)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Pokaži stolpce okvira z besedilom</translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Napredne možnosti</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Ustvari Postscript 3. ravni</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Ustvari Postscript 2. ravni; pazite,\n
to so lahko ogromne datoteke</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Ustvari Postscript 1. ravni; pazite,\n
to so lahko ogromne datoteke</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">&amp;Vodoravno prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">&amp;Navpično prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Uveljavi profile &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">Postscript &amp;1. ravni</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">Postscript &amp;2. ravni</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">Postscript &amp;3. ravni</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Uveljavi &amp;odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Poravnaj</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Vodoravno</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Z vrha</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Po sredi</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Z desne</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Z leve</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Od spodaj</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Navpično</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Razporedi/Poravnaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Uveljavi</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Med:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">Po&amp;ravnaj</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">Raz&amp;postavitev</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">&amp;Enakomerno razporedi</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">V&amp;mes:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">&amp;Ne spreminjaj</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">Po&amp;ravnaj</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">Raz&amp;postavitev</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> točk</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">&amp;Enakomerno razporedi</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;Ne spreminjaj</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Razporejanje in poravnava</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Poravnaj</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativno na:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Prvo izbiro</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Zadnjo izbiro</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Meje</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Vodila</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Izbira</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Desne strani predmetov poravnaj na levo stran vodila</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Levo strani predmetov poravnaj na desno stran vodila</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Poravnaj spodnje strani</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Poravnaj desne strani</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Poravnaj vrhnje strani predmetov na dno sidra</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Sredinsko po navpični osi</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Poravnaj leve strani</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Sredinsko po vodoravni osi</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Poravnaj spodnje strani predmetov na vrh sidra</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Poravnaj vrhnje strani</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Izbrano vodilo:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Razporedi</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Poenoti vodoravne razmike med predmeti</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Vodoravni razmiki med predmeti naj bodo enaki navedeni vrednosti</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Enakomerno razporedi desne strani</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Enakomerno razporedi spodnje strani</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Enakomerno vodoravno razporedi središča</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Poenoti navpične razmike med predmeti</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Navpični razmiki med predmeti naj bodo enaki navedeni vrednosti</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Enakomerno razporedi leve strani</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Enakomerno navpično razporedi središča</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Enakomerno razporedi vrhnje strani</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Razdalja:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Predmete razporedi z dano razdaljo</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Ni izbrano</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Nekateri predmeti so zaklenjeni.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odkleni vse</translation>
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
        <translation>Poravnaj besedilo na levo</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Poravnaj besedilo obojestransko na silo</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Poravnaj besedilo sredinsko</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Poravnaj besedilo obojestransko</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Poravnaj besedilo na desno</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>vsota</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cilj</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Potisni</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Zvezda</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tanko</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Čas</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Široko</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Predpni simbol valute</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Pošlji obrazec</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Omejitev</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Preveri</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Križ</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Ob pozornosti</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Pojdi na</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikone</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Umik</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Stran:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Drugo</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Enobarvno</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Vrsta:</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznamsko polje</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Uporabi simbol valute</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Oblika števil</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Vrsta preverjanja:</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>V datoteko:</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ne tiskaj</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Pošlji na URL:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Izračunaj</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Prehod kazalca</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Vrednost ni preverjena</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>največ</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Oblikovanje</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinirano polje</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Posebna skripta za preverbe:</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>najmanj</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Dejanje</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Obroba</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Oblika datuma</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Gumb</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Krog</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation>Lastnosti polja</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Črtkano</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Oblika časa</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Dogodek:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Vrednost je</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Oblika</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skrito</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Posebna skripta za izračunavanje</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Tu vnesite seznam polj, ločenih z vejicami</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Posebne skripte</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Preobrni</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Brez predogleda</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Število</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstrani</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Kvadrat</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Slog:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Polje je oblikovano kot:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Širina:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Namig orodja:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Ob zabrisanosti</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Ne izvozi vrednosti</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Slike (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Pošlji podatke kot HTML</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Besedilno polje</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Postavitev ikon ...</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Kazalec spusti</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Izberi ...</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Kazalec vstopi</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Uporabi ikone</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Samo za branje</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pritisnjeno</translation>
    </message>
    <message>
        <source>product</source>
        <translation>zmnožek</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Besedilo ob prehodu kazalca</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Potrdi</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Spremeni ...</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Oblika odstotka</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Znaki</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Geslo</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Vidnost:</translation>
    </message>
    <message>
        <source>average</source>
        <translation>povprečje</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Privzdignjeno</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Uvozi podatke</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Kazalec izstopi</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Kazalec pritisne</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skripta:</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Odstotek</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimalke:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>iz naslednjih polj:</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Uvozi podatke iz:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>in manjša ali enaka kot:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Natipk:</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Več-vrstično</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčrtano</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Karo</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Urejanje dovoljeno</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Videz</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Potrditveno polje</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Vrednost ni izračunana</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Uredi ...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Ponastavi obrazec</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Zahtevano</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ne preverjaj črkovanja</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Obrobljeno</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Viden</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Vrednost mora biti večja ali enaka kot :</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Oblika:</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Poudari</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Privzeto izbrano</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Primer:</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ne drsaj</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Besedilo ob pritisku gumba</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Če želite uporabljati ikone na gumbih, potrebujete vsaj ikono za Navadno</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Sprememba izbire</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Pisava za k PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Možnost ne bo upoštevana pri PDF 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF datoteke (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Slike (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Brez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source> pt</source>
        <translation>pik</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Cilj</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Povezava</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Povezava na splet</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation>Lastnosti opomb</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Dokumenti PDF (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Povezava navzven</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Vrsta:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Stran:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Uporabi glavno stran</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Glavna stran:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Uporabi na</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>&amp;trenutni strani</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;lihih straneh</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>so&amp;dih straneh</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>vseh str&amp;aneh</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;v obsegu</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Uveljavi izbrano predlogo na lihih, sodih ali vseh straneh v danem območju&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>to</source>
        <translation>v</translation>
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
        <translation>Uporabi izbrane glavne strani na lihih, sodih ali vseh straneh v naslednjem obsegu</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Običajo</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Uveljavi predlogo</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Predloga:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Uveljavi na &amp;trenutni strani</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Uveljavi s &amp;strani:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Za:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Uveljavi na &amp;sodih straneh</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Uveljavi na &amp;lihih straneh</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>Generator &amp;črtne kode ...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Uporabniški vmesnik Scribus za pisanje črtne kode (Pure Postscript Barcode Writer)</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Napaka pri odpiranju datoteke: %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 ali 13 številk</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 številk</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 ali 12 številk</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 ali 8 številk</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 številk</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 številki</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 ali 10 številk ločenih s ustreznimi pomišljaji</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Spreminjajoče število znakov, številk in simbolov -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Spreminjajoče število ASCII znakov in posebnih ukazov simbolov, prične se z ustreznim začetnim znakom za začetni nabor znakov. FNC 1 simbol mora slediti takoj za začetkom znaka pri UCC/EAN-128s.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Spreminjajoče število številk in simbolov -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Spreminjajoče število številk. ITF-14 ima 14 znakov in nima številk za preverjanje</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Spreminjajoče število številk</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Spreminjajoče število številk in velikih črk</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Spreminjajoče število šestnajstiških znakov</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Črtna koda nepopolna</translation>
    </message>
    <message>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation>12 ali 13 številk z vezaji. Zastarela oblika ISBN-10 sprejema 9 ali 10 številk z vezaji, vendar ni več v javni uporabi po 1. januarju 2007. (Opomba: Za pretvorbo stare številke ISBN-10 v novo ISBN-13, na začetek dodajte predpono 978- prvim 9 številkam, npr. 1-56592-479-7 -&gt; 978-1-56592-479. Zadnja preskusna številka bo izračunana samodejno.)</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Generator črtne kode</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Črtna koda</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Vrsta:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Izberite eno od razpoložljivih vrst črtne kode</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Številčni prikaz same kode. Glejte sporočilo pomoči spodaj</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Ponastavi vzorce črtne kode</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Vključi besedilo v črtno kodo</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt +I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Če je izbrano, vključi številke v črtni kodi</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Straži prazen prostor</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Riši puščice, da je prostor poleg kode zagotovljen</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Ozadje</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Barva ozadja - pod črtami kode</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Črte</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Barva črt črtne kode</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Besedilo</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Barva besedila in številk</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Tu so prikazani nasveti in pomoč</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Predogled rezultata. 72dpi vzorec.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Ko&amp;da:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>V&amp;ključi kontrolno vsoto</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Ustvari in vključi kontrolno vsoto v črtno kodo</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>&amp;Vključi številko kotnrolne vsote</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Vključi številko kontrolne vsote v besedilo črtne kode</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Delete</source>
        <translation>Izbriši</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Predmet</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Odložišča (*.scs);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Skicirka</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nov vnos</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Preimenuj</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Ime &quot;%1&quot; ni Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Naloži ...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Majhen</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">&amp;Srednji</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="obsolete">&amp;Velik</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Predogled</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Izberite mapo za skicirko</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Skicirka (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Izberite datoteko skicirke za uvoz</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation>&amp;Uvozi datoteko skicirke ...</translation>
    </message>
    <message>
        <source>Main</source>
        <translation>Glavno</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Premakni zaznamek</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Zaznamki</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Vstavi zaznamek</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Zaznamki</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikona</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nikoli</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Ponastavi</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Napis prekrije ikono</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Način umerjanja:</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Postavitev:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Samo napis</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Napis pod ikono</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Vedno</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Merilo:</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Samo ikona</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Če je ikona premajhna</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Napis levo od ikone</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Naslov desno od ikone</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Če je ikona prevelika</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Nesorazmerno</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Sorazmerno</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation>Postavitev ikon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Napis nad ikono</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Barvni profil, ki ste ga ustvarili sami ali dobili od proizvajalca.
Ta naj bo prirejen modelu vašega zaslona in ne kak splošen profil (npr. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Barvni profil vašega tiskalnika. To naj bi bil poseben profil za vaš model,
kakršnega ponuja proizvajalec, in ne kak splošen profil (npr. sRGB).</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Sistemski profili</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Privzet profil pri uvažanju slik.</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Namen upodobitve</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Nastavitve upravljanje barv</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Nadomeščanje po črnini je način za izboljšanje kontrasta slike. 
Če v dokumentu uporabljate slike, je to priporočljivo.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Privzet namen upodobitve strani. Če niste gotovi,
izberite Relativne barvne vrednosti ali Zaznavno.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Omogoči &quot;mehko nastavljanje&quot; barv dokumenta pri tisku
glede na izbran tiskalniški profil.</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Zaznavno</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Privzet namen upodobitve strani. Če niste gotovi,
izberite Relative-Colorimetric ali &quot;Samo pogled&quot;.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Način prikazovanja barv na zaslonu, ki se morda ne bodo natisnile pravilno. 
To zahteva zelo natančno izbiro vseh profilov.</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Privzet profil za barvne površine na strani.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Vključi upravljanje barv</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Slike:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Polne barve:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Zaslon:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Ti&amp;skalnik:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Z&amp;aslon:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Tiskalnik:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Pri izrisu &amp;posnemaj tiskalnik</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Označi barve izven &amp;spektra</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Uporabljaj kopenzacijo &amp;črnine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>Slike &amp;RGB:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>Slike &amp;CMYK:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Privzet barvni profil za uvožene slike CMYK</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Privzet barvni profil za uvožene slike RGB</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source> %</source>
        <translation>V redu</translation>
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
        <source>G:</source>
        <translation>G:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation>K:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation>M:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>R:</source>
        <translation>R:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Novo</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Staro</translation>
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
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Ime barve ni edinstveno</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>Spletno varne RGB</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation>Uredi barvo</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Nespremenljivi izbirniki barv</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Spremenljivi izbirniki barv</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Katalog barv HSV</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Barvni &amp;model</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Ne morete ustvariti barve z imenom &quot;%1&quot;,
ker je to ime rezervirano za prosojno barvo</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>je čista barva</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>je registracijska barva</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Ne morete ustvariti barve z imenom &quot;%1&quot;,
ker je to ime rezervirano za prosojno barvo</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>Ime barve ni edinstveno</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation>S to izbiro boste omogočili tiskanje na vseh ploščah. Registracijske barve se uporabljajo za tiskarske oznake, kot so oznake za obrezovanje, registrske oznake in podobno. Te niso pogosto uporabljene v sami postavitvi.</translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation>S to izbiro boste to barvo naredili za točkovno barvo, s čimer boste ustvarili novo točko pri izdelavi plošč ali separacij. To je najpogosteje uporabljeno, ko logotip ali druga barva potrebuje natanko predstavitev ali pa je ni mogoče potanko prikazati s črnili CMYK. Metalna in fluoroscentna črnila so dober primer, ki ga ni mogoče enostavno prikazati s črnili CMYK.</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation type="obsolete">Če pritisnete in držite desni miškin gumb,
lahko vidite pomanjšano sličico.

Tipka &apos;Insert&apos; doda znak v spodnjo izbiro,
tipka &apos;Delete&apos; pa odstrani zadnjega</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Če pritisnete in držite desni miškin gumb, lahko vidite pomanjšano sličico. Tipka &apos;Insert&apos; doda znak v spodnjo izbiro, tipka &apos;Delete&apos; pa odstrani zadnjega</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Izberi znak:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Pisava:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Razred znaka:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vstavi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Vstavi črke v besedilo pod kazalko</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Izbriši trenutno izbiro.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Zapri to pogovorno okno in se vrni k urejanju besedila.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Celoten nabor znakov</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Osnovni latin</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 dodatek</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin razširjeni A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latin razširjeni B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Splošno naglaševanje</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Nad- in podpisano</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Valutni simboli</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Črkam podobni znaki</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Oblike števil</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Puščice</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematični znaki</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Risanje s kvadratki</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blokovski elementi</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometrijski liki</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Razni simboli</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Viseče oznake</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Različice male oblike</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligature</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Posebno</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grška pisava</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Razširjena grška pisava</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Cirilica</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Cirilica - dodatek</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabska pisava</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Razširjena arabska A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Razširjena arabska B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebrejska pisava</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>&amp;Vstavite kodo:</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Zapri to pogovorno okno in se vrni k urejanju besedila</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Vpišite štiriznakovno vrednost unicode sem</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation>Trenutni profil:</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Predmeti</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Težave</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Manjkajoči znaki</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Prelivanje besedila</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Predmet ni na strani</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Manjka slika</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Slika ima ločljivost manjšo od %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Predmet je prosojen</translation>
    </message>
    <message>
        <source>Object is a PDF-Annotation or Field</source>
        <translation type="obsolete">Predmet je PDF opomba ali polje</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Predmet je postavljeni PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Ni najdenih težav</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Stran </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Prosti predmeti</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Najdene težave</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Preveri pred letom</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Predmet je opomba ali polje PDF</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Prezri napake</translation>
    </message>
</context>
<context>
    <name>CheckerPrefsList</name>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Izberi sloge</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Dostopni slogi</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Zbiranje ...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Neuspešno zbiranje datotek za izhod datoteke:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Neuspešeno zbiranje datoteke:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Odstrani neuporabljene</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Nabori barv</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Trenuten nabor barv:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Shrani nabor barv</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Izberite nabor barv za nalaganje</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Shrani trenuten nabor barve</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Odstrani neuporabljene barve iz nabora barv trenutnega dokumenta</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>V trenutni nabor uvozi barve iz drugega dokumenta</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Ustvari novo barvo znotraj trenutnega nabora</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Uredi izbrano barvo</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Naredi kopijo trenutno izbrane barve</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Izbriši izbrano barvo</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Izberi trenuten nabor barv kot privzetega</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Izberite ime</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nova barva</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Če je upravljanje barv omogočeno, potem opozorilni trikotnik predstavlja opozorilo, da je barva morda zunaj barvnega gamuta profila trenutno izbranega tiskalnika. To pomeni, da barve morda ne bo mogoče natisniti natanko tako, kot je videti na zaslonu. Spot barve nakazuje rdeči krožec. Registracijske barve imajo poleg barve registracijsko oznako. Več namigov o opozorilih gamuta najdete v sprotni pomoči pod Upravljanje barv.</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Enobarvno</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analogno</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementarno</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Deljeno komplementarno</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadično</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetradično (dvojno komplementarno)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Osnovna barva</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Enobarvna svetla</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Enobarvna temna</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>Prva analogna</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Druga analogna</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>Prva deljena</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>Druga deljena</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>Tretja deljena</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>Četrta deljena</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>Prva triadična</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>Druga triadična</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>Prva tetradična (nasprotna osnovna)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>Druga tetradična (kot)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Tretja tetradična (nasprotni kot)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Normal Vision</source>
        <translation>Običajen pogled</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Polna barvna slepota</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Okvara pogleda:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Barvni krog</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
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
        <translation>R (Y)</translation>
    </message>
    <message>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation>Izberite način:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Kot (0 - 90 stopinj):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Spoji barve</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>Zamen&amp;jaj barve</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Združi pripravljene barve v barve dokumenta</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Zamenjaj pripravljene barve v barve dokumenta</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Barve pusti nedotaknjene</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Spajanje barv</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Napaka:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Odpiranje upravljalnika barv.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Spajanje barv</translation>
    </message>
    <message>
        <source>Protanopia (red)</source>
        <translation type="obsolete">Protanopia (rdeča)</translation>
    </message>
    <message>
        <source>Deuteranopia (green)</source>
        <translation type="obsolete">Deuteranopia (zelena)</translation>
    </message>
    <message>
        <source>Tritanopia (blue)</source>
        <translation type="obsolete">Tritanopia (modra)</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>Pr&amp;ipravi barvo ...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>Uvoz&amp;i obstoječo barvo ...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Spoji barve</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>Zamen&amp;jaj barve</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>I&amp;zhod</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>Saturation:</source>
        <translation type="obsolete">Nasičenost:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="obsolete">Vrednost:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Razlika med izbrano vrednostjo in preštetimi vrednostmi. Za več informacij si preberite navodila.</translation>
    </message>
    <message>
        <source>Saturation component in HSV mode</source>
        <translation type="obsolete">Nasičenost HSV načina</translation>
    </message>
    <message>
        <source>Value component in HSV mode</source>
        <translation type="obsolete">Vrednost HSV načina</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>S klikom na krog izberite čisto barvo. Ta ustreza vrednosti odtenka v HSV načinu.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Vzorčna barvna shema</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Izberite metodo pripravljanja barvne sheme. Za več informacij si preberite navodila.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Barve izbrane barvne sheme</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Simuliranje pogostih okvar vida. Izberite vrsto okvare.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nova barva</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Ne najdem zahtevane barve. Najbrz ste izbrali črno, sivo ali belo. Teh barv se ne da pripraviti.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>B&amp;arvne komponente ...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Protanopija (rdeča)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Devteranopija (zelena)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Tritanopija (modra)</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Barva %1 že obstaja!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Barva %1 je dodana.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>Barvni &amp;krog ...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Pomočnik pri nastavljanju barv</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Izbirnik barv, ki upošteva teorijo barv.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Uveljavi</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Posamična stran</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Dvostransko</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>Trojno zloženo</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>Četvorno zloženo</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Leva stran</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Sredina</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Srednja leva</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Srednja desna</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Desna stran</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation>Navadno levo</translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation>Navadno sredinsko</translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation>Navadno desno</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Ponedeljek</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Torek</translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation>Sreda</translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation>Četrtek</translation>
    </message>
    <message>
        <source>Friday</source>
        <translation>Petek</translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation>Sobota</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Nedelja</translation>
    </message>
    <message>
        <source>January</source>
        <translation>Januar</translation>
    </message>
    <message>
        <source>February</source>
        <translation>Februar</translation>
    </message>
    <message>
        <source>March</source>
        <translation>Marec</translation>
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
        <translation>Junij</translation>
    </message>
    <message>
        <source>July</source>
        <translation>Julij</translation>
    </message>
    <message>
        <source>August</source>
        <translation>Avgust</translation>
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
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Krožen preliv</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Metoda navadnega ali prelivnega polnila</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Senca:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Prekrivnost:</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Navpičen preliv</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Nastavi prosojnost izbrane barve</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Barva izbranega predmeta</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Križno diagonalni preliv</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Uredi lastnosti barve polnila</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Nasičenost barve</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonalni preliv</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Uredi lastnosti barve črte</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Vodoraven preliv</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Prost enosmeren preliv</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Prost krožen preliv</translation>
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
        <translation>točk</translation>
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
        <translation type="obsolete">pik</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Premakni vektor</translation>
    </message>
    <message>
        <source>Move the start of the Gradient Vector with the left Mouse Button pressed and
nove the end of the Gradient Vector with the right Mouse Button pressed</source>
        <translation type="obsolete">S pritisnjenim levim miškinim gumbom premikate začetek vektorja preliva,
z desnim pa konec vektorja preliva</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Začetek vektorja preliva premikate s pritisnjenim levim miškinim gumbom, konec pa z desnim</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Možnosti uvoza CVS</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Ločilo polj:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Ločilo vrednosti:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Prva vrstica je glava tabele</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Samo sode strani</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vrednost</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Stran(i) na list</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Zrcalno</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Možnost</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Usmerjenost</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation>Možnosti tiskanja</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Samo lihe strani</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Nabor strani</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Strani na list</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Tiskanje n-gor</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Vse strani</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Tu vidite razne CUPSove možnosti pri tiskanju.\n
Natančne nastavitve so odvisne od vaših gonilnikov tiskalnika.\n
Podporo CUPSa lahko preverite pod Pomoč -&gt; O Scribusu.\n
Poiščite naslednje: C-C-T pomeni podporo za: C=CUPS C=littlecms T=TIFF.\n
Manjkajoča knjižnica je označena z zvezdico ( * )</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Tu vidite razne možnosti, ki jih pri tiskanju ponuja CUPS.\n Natančne nastavitve so odvisne od vaših gonilnikov tiskalnika.\n Podporo CUPSa lahko preverite pod Pomoč -&gt; O Scribusu.\n Poiščite naslednje: C-C-T pomeni podporo za: C=CUPS C=littlecms T=TIFF.\n Manjkajoča knjižnica je označena z zvezdico ( * )</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Premakne v mapo dokumenta.
To lahko nastavite v Nastavitvah.</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodiranje znakov:</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Stisni datoteko</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;vključi pisave</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Določi barvne komponente</translation>
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
        <translation>O (H):</translation>
    </message>
    <message>
        <source>S:</source>
        <translation>N (S):</translation>
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
        <translation>Z (G):</translation>
    </message>
    <message>
        <source>B:</source>
        <translation>Mo (B):</translation>
    </message>
    <message>
        <source>C:</source>
        <translation>C:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation>Ma (M):</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>R (Y):</translation>
    </message>
    <message>
        <source>K:</source>
        <translation>K:</translation>
    </message>
    <message>
        <source>Set &amp;RGB</source>
        <translation>Določi &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Določi C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Določi &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Preklical uporabnik</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Zamenjaj z:</translation>
    </message>
    <message>
        <source>Delete Color</source>
        <translation>Izbriši barvo</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Izbriši barvo:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Izbriši barvo:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Zamenjaj z:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>do:</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Odstrani iz:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Izbriši strani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Izbriši od:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Izbriši slog</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Izbriši slog:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Zamenjaj z:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">namesto</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Manjka pisava</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Pisava %1 ni nameščena.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Uporabi</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Uvoz ni uspel</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Uvoz dokumenta Word ni uspel
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Podatki o dokumentu</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Naslov:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Avtor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Ključne besede:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Opis:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Založnik:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Avtorji &amp;prispevkov:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datum:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Vrsta:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>Zap&amp;is:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Označevalec:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Vir:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Odnos:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Pokritje:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Pravice:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Nadaljne &amp;informacije</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Oseba ali organizacije, ki je odgovorna za vsebino dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Ime dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Opis vsebine dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Tema vsebine dokumenta.
To polje lahko vključite v Scribusov dokument za opombo, ali v PDF kot metapodatek</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Oseba ali organizacija, ki je odgovorna za objavo dokumenta</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Oseba ali organizacija, odgovorna za prispevke k vsebini dokumenta</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datum, povezan z dogodkom v življenskem ciklu dokumenta. Skladno s standardom ISO 8601 v obliki: LETO-MM-DD</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Zvrst ali žanr vsebine dokumenta (npr. kategorije, funkcije, žanri ...)</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Fizično ali elektronsko udejanjenje dokumenta. Navedite vrsto nosilca in velikosti.
Lahko tudi RFC2045,RFC2046 kot MIME vrste</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Nedvoumen sklic na dokument v danem kontekstu, npr. ISBN ali URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Sklic na dokument, iz katerega je dokument izveden, npr. ISBN ali URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Jezik v katerem je napisana vsebina dokumenta, običajno jezikovna koda ISO-639 
z dodano okrajšavo in ISO-3166 državno kodo, na primer sl-SI, en-GB</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Sklic na povezan dokument, po možnosti s formalnim označevalcem, kakršna sta ISBN in URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Relevanca oz. umestitev vsebine dokumenta, po možnosti vključujoč mesto, čas in pravni doseg</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Podatki o pravicah, uporabljenih in vsebovanih v dokumentu, npr. avtorske pravice,
patenti, blagovne znamke</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Oseba ali organizacija, ki je v glavni meri odgovorna za vsebino dokumenta. To polje lahko vključite v Scribusov dokument ali kot metapodatek v PDF</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Ime dokumenta. To polje lahko vključite v Scribusov dokument ali kot metapodatek v PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Kratek povzetek vsebine dokumenta. To je pri izvozu vključeno v PDF</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Tema vsebine dokumenta. To polje je namenjeno ključnim besedam, ki jih želite vključiti v PDF, za lažje iskanje po PDF datotekah</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fizična ali digitalna manifestacija dokumenta. Umestno je omeniti mere dokumenta. Smiselne so tudi oznake RFC2045,RFC2046 za vrste MIME</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Jezik, v katerem je dokument napisan. Običajno je to jezikovna koda po standardu ISO-639, z neobveznim dodatkom državne kode po standardu ISO-3166. Primer: sl-SI, en-GB</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Dokumentu dodaj odsek s številkami strani. Nov odsek bo dodan za trenutno izbranim.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Izbriši trenutno izbran odsek.</translation>
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
        <translation>&lt;b&gt;Ime:&lt;/b&gt; Izbirno ime za odsek, npr. Kazalo&lt;br/&gt;&lt;b&gt;Prikaži:&lt;/b&gt; Izberi za prikaz številčenja strani v tem odstavku, če je en ali več besedilnih okvirjev nastavljen, da to stori.&lt;br/&gt;&lt;b&gt;Od:&lt;/b&gt; Začetna številka, pri kateri naj se odstavek začne.&lt;br/&gt;&lt;b&gt;Do:&lt;/b&gt; Končna številka, pri kateri naj se odstavek konča.&lt;br/&gt;&lt;b&gt;Slog:&lt;/b&gt; Izberite slog številčenja strani, ki bo uporabljen.&lt;br/&gt;&lt;b&gt;Začetek:&lt;/b&gt; Številka, pri kateri naj se slog začne. Primer: Če je Začetek=2 in Slog=a,b,c, ..., potem se bo številčenje začelo pri b. Za prvi odstavek dokumenta bo to zamenjalo staro prvo številko strani, izbrano v oknu novega dokumenta.</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Številke strani so izven meja</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Vrednost, ki ste jo vpisali, je zunaj dosega številčenja strani v trenutnem dokumentu (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Odseki dokumenta</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Od</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Do</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Slog</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Začetek</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Prikazano</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Se nanaša na</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je starš</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je otrok </translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Besedilni okvirji</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Slikovni okvirji</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Dvojiško</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Cela števila</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Niz znakov</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="obsolete">Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Ime</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Vrednost</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Odnos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Odnos do</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="obsolete">Samodejno dodaj k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiraj</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Počisti</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Brez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Brez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Realno število</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vrednost</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Odnos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Odnos do</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Samodejno dodaj k</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cijanska</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscript datoteke (*.ps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Rumena</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation>Nastavi tiskalnik</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Črna</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Cilj tiskanja</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Možnosti ...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Datoteka:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Nadomesten ukaz za tiskanje</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Ukaz:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Obseg</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Natisni &amp;vse</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Natisni &amp;trenutno stran</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Natisni &amp;obseg</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Število kopij:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">Tiskaj &amp;običajno</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Tiskaj &amp;separacije</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Če je mogoče tiskaj v &amp;barvah</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Tiskaj &amp;črnobelo</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">&amp;Napredne možnosti...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Natisni</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Uporabite drug upravljalnik tiskanja, na primer kprinter ali 
gtklp, za dodatne možnosti tiskanja</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Tiskaj navadno</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Tiskaj ločeno (separacije)</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Če je mogoče, natisni v barvah</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Natisni sivinsko</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript ravni 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript ravni 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript ravni 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Vodoravno prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Navpično prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Nastavi velikost papirja</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Uveljavi odstranjevanje pod barvo</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Pretvori čiste barve v procesne barve</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Uporabi profile ICC</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Napredne možnosti</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Predogled ...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Nastavi raven PostScripta.
Ravni 1 in 2 lahko ustvarita velike datoteke</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Datoteke PostScript (*.ps);; Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Za dodatne možnosti tiskanja uporabite drug upravljalnik tiskanja, na primer kprinter ali gtklp</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>To je način za zamenjavo sivin sestavljenih iz cijanske, rumene in magentne barve s črnino. POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej narediti nekaj poskusov. POD tudi zmanjša možnost prekomerne nasičenosti črnil CMY.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Vključi čiste barve, da se pretvorijo v kompozitne. Če ne nameravate tiskati čistih barv v tiskarni, potem je verjetno najbolje, da to pustite vključeno.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Omogoča vključevanje profilov ICC v pogovorno okno tiskanja, če je upravljalnik barv vključen</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>To omogoča natančno določitev velikosti datoteke PostScript. Ni priporočeno, razen če to zahteva vaš tiskalnik.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Poreži do robov strani</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Ne kaži predmet, ki so zunaj robov</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Nastavitev tiskalnika ni mogoče pridobiti</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Ne pokaži predmetov, ki so zunaj robov tiskanja strani</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Uvoz datoteke:\n
%1\n
ni uspel!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Usodna napaka</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Napaka</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation>Uvažanje PostScripta</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Preučevanje PostScripta:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Tvorba predmetov</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Pretvorba %1 slik ni uspela!</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
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
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Ime sloga ni edinstveno</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Razmik nad odstavkom</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorji in zamikanje</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Ime vašega sloga odstavka</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Nastavi celotno višino ozaljšanih začetnic v številu vrstic</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Pisava izbranega besedila ali predmeta</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Poseben učinek, ki poveča prvo črko odstavka</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Uredi slog</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva potez besedila</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Navpični presledki</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Poravnaj besedilo na osnovno mrežo</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva polnila besedila</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Razmik pod odstavkom</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Razmik vrstic</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Učinek:</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Pisava:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">P&amp;oravnava:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">&amp;Ozaljšana začetnica</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Črte:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Barva po&amp;teze:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Prilagodi osnovni &amp;mreži</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">&amp;Razmik vrstic:</translation>
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
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Sample text of this paragraph style</source>
        <translation type="obsolete">Vzorčno besedilo za ta slog odstavka</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged
for blue quartz</source>
        <translation type="obsolete">V kožuščku hudobnega fanta stopiclja mizar</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Razdalje</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Nespremenljiv razmik vrstic</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Samodejno razmikanje vrstic</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Poravnaj na mrežo osnovnice </translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Ozaljšane začetnice</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Razdalja do besedila:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Predogled sloga odstavka</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Določi razmik med ozaljšano začetnico in besedilom</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Spremeni vzorčno besedilo sloga odstavka</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Ime sloga ni edinstveno</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Izberite za lažje branje svetlih besedilnih slogov</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ročno sledenje</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Odmik od osnovnice črk</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation type="obsolete">Klikni za izbiro kako so vrstice razmaknjene</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Kliknite in držite pritisnjeno za izbor načina razmikanja črt.</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Samodejno</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Urejevalnik</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Java skripti (*.js);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Odpri ...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot ...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Shrani in končaj</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Končaj brez shranjevanja</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>P&amp;onovi</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Po&amp;čisti</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>Pri&amp;dobi imena polj</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaSkripti (*.js);; Vse datoteke (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Učinki slike</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Možnosti:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Senca:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Svetlost:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Polmer:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Vrednost:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posteriziraj:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Učinki na voljo</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Zabrisanost</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Svetlost</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Obarvanost</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Sivinsko</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Preobrni</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posteriziraj</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Izostri</translation>
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
        <translation>Uporabljeni učinki</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>Vse str&amp;ani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">V red&amp;u</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Spremeni končno mapo</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Mapa v katero shranjujete slike.\n
Ime izvozne datoteke bo &apos;imedokumenta-številkastrani.vrstadatoteke&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Izvozi samo trenutno stran</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Vrste datotek za izvoz</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Izberite mapo za izvažanje</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Izvozi v mapo:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Vrsta slike:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kakovost:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Izvozi kot sliko(e)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Ločljivost:</translation>
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
        <translation>Obseg</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Trenutna stran</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Obseg</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Izvozi obseg strani</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Vpišite z vejicami ločen seznam območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka posamezne strani.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Izvozi vse strani</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Ločljivost slik. Za uporabo na zaslonu
uporabite ločljivost 72 dpi</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kakovost slik - 100% je najvišjša, 1% je najnižja</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Velikost slik. 100% za brez sprememb, 200% za dvojno velikost, itn.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Razširjene lastnosti slike</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajno</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Potemni</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Posvetli</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Obarvanost</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Barva</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Svetilnost</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Pomnoži</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Zaslon</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Prelij</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Prekrij</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Ostra luč</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Mehka luč</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Razlika</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Izključitev</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Barvno posvetli</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Barvno potemni</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Izključitev</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Način spajanja:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Prekrivnost:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Plasti</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Ne uporabljaj poti</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Poti</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Velikost:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Brez naslova</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Naslov:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Avtor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznano</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Skribusov dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Ločljivost:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
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
        <source>Colorspace:</source>
        <translation>Barvni prostor:</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Sivinsko</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Open</source>
        <translation type="obsolete">Odpri</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">V trenutni nabor dodaj barve iz obstoječega dokumenta</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Ustvari novo barvo znotraj trenutnega nabora</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Izberi nabor barve</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Odstrani neuporabljene barve iz nabora barv trenutnega dokumenta</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Barve</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Izbriši izbrano barvo</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Nabori barv</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Trenuten nabor barve</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Shrani trenuten nabor barve</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Nova barva</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopija %1</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Uredi izbrano barvo</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Izberi ime</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Napravi kopijo trenutno izbrane barve</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Izberi trenuten nabor barv kot privzet</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">&amp;Odstrani neuporabljene</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Shrani nabor barv</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Ime:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Uvozi</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="obsolete">V trenutni nabor uvozi barve iz drugega dokumenta</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Nekatere pisave, uporabljene v tem dokumentu, so bile nadomeščene:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>je bila zamenjana z:</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Da</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Ime pisave</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Nadomestne pisave</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Podnabor</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Zamenjava</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Dodatne &amp;poti</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Vključi v:</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Uporabi pisavo</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Nadome&amp;stne pisave</translation>
    </message>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Globalne nastavitve pisav</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Pot do datoteke s pisavo</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Dodatne poti</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Pis&amp;ave na voljo</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Pisave na voljo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Dodaj ...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Ime pisave</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Uporabi pisavo</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Vdelaj v:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Podnabor</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Pot do datoteke s pisavo</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Poti za iskanje pisav lahko nastavite samo v Nastavitvah, in to le če nimate odprtega dokumenta. Za spreminjanje poti iskanja pisav zaprite vse dokumente in uporabite Uredi-&gt;Nastavitve.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="obsolete">Poti za iskanje pisav lahko nastavite samo v Nastavitvah, in to le če nimate odprtega dokumenta. Za spreminjanje poti iskanja pisav zaprite vse dokumente in uporabite Uredi-&gt;Nastavitve.</translation>
    </message>
    <message>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation>Poti za iskanje pisav lahko nastavite samo v nastavitvah, in to le če nimate odprtega dokumenta. Za spreminjanje poti iskanja pisav zaprite vse odprte dokumente, nato uporabite Datoteka-&gt;Nastavitve-&gt;Pisave.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Piškur molče grabi fižol iz dna cezijeve hoste</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Predogled pisav</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Dodaj izbrano pisavo v slog, meni Pisava</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Zapusti predogled</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="obsolete">Ime pisave</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="obsolete">Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Podnabor</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="obsolete">Dostop</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Uporabnik</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Sistem</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Predogled pisav</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>font preview</comment>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Dodaj izbrano pisavo v slog, meni Pisava</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Zapusti predogled</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>V kožuščku hudobnega fanta stopiclja mizar</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="obsolete">Velikost pisave:</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="obsolete">Hitro iskanje:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Išči</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Zapri</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation type="obsolete">To polje omogoča hitro iskanje po imenih pisav. Na primer: &apos;bold&apos; pokaže vse pisave, ki imajo v imenu besedo bold. Iskanje ni občutljivo za velike/male črke.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Začni z iskanjem</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Velikost izbrane pisave</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Vzorec bo prikazan po sprostitvi tipke</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>To vnosno polje omogoča hitro iskanje po imenih pisav. Iskanje ni občutljivo za velike/male črke. Lahko uporabite standardne označbe (*,?, [...]) v vaši frazi. Primer: t* prikaže vse pisave, ki se začnejo s t ali T. *bold* pokaže vse pisave, ki imajo v imenu besedo bold, bolder itd.</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Predogled pisav</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Hitro iskanje:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Najdi</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Ime pisave</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Podnabor</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Dostop</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Velikost pisave:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Vzorčno besedilo za predogled</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>&amp;Nastavi</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Ponastavi besedilo</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
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
        <translation>Predogled &amp;pisav ...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Pogovorno okno za predogled pisav</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Urejanje, iskanje, pregledovanje pisav, ki so na voljo.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Nadomeščanje pisave</translation>
    </message>
    <message>
        <source>This Document contains some Fonts that are not installed on your System,
please choose a suitable replacement for them.</source>
        <translation type="obsolete">V dokumentu so uporabljene pisave, ki jih nimate na sistemu. 
Izberite primerne nadomestke zanje.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Izvorna pisava</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Nadomestna pisava</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Te zamenjave naj bodo stalne</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>V dokumentu so uporabljene pisave, ki niso nameščene v sistem. Izberite njihove nadomestke. Če izberete &apos;Prekliči&apos;, prekinete nalaganje dokumenta.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Prekliče te nadomestke in ustavi nalaganje dokumenta.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>S tem Scribusu naročite, naj trajno uporablja te nadomestke za manjkajoče pisave v vseh prihodnjih postavitvah. To lahko spremenite v Uredi &gt; Nastavitve &gt; Pisave.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Če izberete V redu in shranite, se bodo te zamenjave trajno zapisale v dokument.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Položaj:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation type="obsolete">Tu lahko dodajate, spreminjate ali odstranite barvne postanke.</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Dodajte, spremenite ali odstranite barvne postanke</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> točk</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Vodoravna vodila</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation>Upravljaj z vodili</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Navpična vodila</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Izbriši</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Izbriši</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Zakleni vodila</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Vrste in stolpci - samodejna vodila</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>V&amp;rstice:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>St&amp;olpci:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>Razmi&amp;k vrstic</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Razmik sto&amp;lpcev</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>Nanašajoč se na:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Stran</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Robovi</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Izbira</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>&amp;Posodobi</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Nastavi vodila v dokumentu. Upravljalnik vodil ostane odprt, spremembe pa so trajno uveljavljene</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;Uporabi na vseh straneh</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Vodila</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Enota</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Predogled</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Prazno vodilo (0,0) že obstaja</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Priročnik žal ni dosegljiv. Najnovejši priročnik najdete na http://docs.scribus.net,
najnovejše datototeke za prenos pa na www.scribus.net.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Vsebina</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Povezava</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribusova spletna pomoč</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Vsebina</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Najdi</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Poi&amp;šči</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;Izbriši vse</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Zaznamki</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Na&amp;tisni ...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>I&amp;zhod</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Iskanje ne razlikuje med velikimi in malimi črkami</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation type="obsolete">Neznano</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Najdi</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Iskalni izraz:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nov zaznamek</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Naslov novega zaznamka:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Najdi ...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Najdi &amp;naslednjega</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Najdi &amp;prejšnjega</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>Dod&amp;aj zaznamek</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>Izbriš&amp;i vse</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Zaznamki</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Pomembnost</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Accept</source>
        <translation>Sprejmi</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Možno deljenje</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Preskoči</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Nastavitve deljenja besed</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Največ zaporednih deljenj besed.
Vrednost 0 pomeni neomejeno deljenje besed.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Dolžina najkrajše besede, ki bo še deljena.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Najmanjša beseda:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Predlogi za &amp;deljenje</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>&amp;Samodejno deli besede med pisanjem</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Če vključite možnost Dodatno-&gt;Deli besede, se bo za vsako besedo pojavilo pogovorno okno z vsemi možnimi deljenji te besede.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Omogoči samodejno deljenje besed med pisanjem.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Dovoljena &amp;zaporedna deljenja: </translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Podatki o sliki</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Splošni podatki</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Datum / čas:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Ima vdelan profil:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Ime profila:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Ima vdelane poti:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Ima plasti:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>Podatki EXIF</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Umetnik:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Komentar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Komentar uporabnika:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Model kamere:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Proizvajalec kamere:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Opis:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Model skenerja:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Proizvajalec skenerja:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Uvozi &amp;EPS/PS ...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Uvozi datoteke EPS</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Uvoz datotek EPS v dokument Scribus s spreminjanjem vektorskih podatkov v Scribusove predmete.</translation>
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
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>na konec</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>pred stranjo</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation>Vstavi stran</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Vstavljanje</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>za stranjo</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Predloga (desna stran)</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Vstavljanje</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Stran(i)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Predloga (leva stran):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Predloga:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vstavi</translation>
    </message>
    <message>
        <source>Template (&amp;Left Page):</source>
        <translation type="obsolete">Predloga (&amp;Leva stran):</translation>
    </message>
    <message>
        <source>Template (&amp;Right Page):</source>
        <translation type="obsolete">Predloga (&amp;Desna stran):</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Glavne strani</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Glavna stran:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Premakni predmete skupaj s stranjo</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Vstavi tabelo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Število stolpcev:</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Število vrstic:</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Število vrstic:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Število stolpcev:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Nova skripta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Uredi Javaskripte</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Uredi ...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Dodaj ...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nova skripta:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Ali zares želite zbrisati to skripto?</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Resnično želite izbrisati to skripto?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Doda novo skripto, z vnaprej definirano funkcijo z enakim imenom. Če hočete uporabiti skripto kot skripto &quot;Open Action&quot;, potem ne pozabite spremeniti imena funkcije.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Alt</source>
        <translation>Alt</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation>Alt+</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation>Ctrl</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation>Ctrl+</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation>Shift</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Dejanje</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Uredi bližnjice na tipkovnici</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Izberi tipko za to dejanje</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Trenutna tipka</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Tipka &quot;ne&quot;</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Uporabniško določena tipka</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Nastavi &amp;tipko</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Ta kombinacija tipk je že uporabljena</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Nabori tipk za bližnjice</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Naloži</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Uvozi ...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Izvozi ...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Ponastavi</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Nabori tipk za bližnjice, ki so na voljo</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Naloži izbran nabor tipk za bližnjice</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>V trenutno nastavitev uvozi nabor tipk za bližnjice</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Izvozi trenutni nabor tipk za bližnjice v datoteko</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Ponovno naloži privzete tipke za bližnjice</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Datoteke XML naborov tipk za bližnjice (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>To zaporedje tipk je že v uporabi</translation>
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
        <source>Delete Layer</source>
        <translation>Izbriši plast</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Spusti plast</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Plasti</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Dvigni plast</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nova plast</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Ali res želite izbrisati tudi vse predmete tega sloja?</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Dodaj sloj</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Želite izbrisati tudi vse predmete na tej plasti?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Dodaj novo plast</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Izbriši plast</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Dvigni plast</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Spusti plast</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Edit Line Styles</source>
        <translation>Uredi sloge črt</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nov slog</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Ali zares želite zbrisati ta slog?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Ali zares želite izbrisati ta slog?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation type="obsolete">LineStyleWBase</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Širina črte:</translation>
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
        <translation>Raven konec</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Kvadratast konec</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Zaobljen konec</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spoj pod kotom</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Nagnjen spoj</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Zaobljen spoj</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Polna črta</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Črtkana črta</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Pikčasta črta</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Črta pika črta</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Črta pika pika črta</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Izberi vzorčno besedilo</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Avtor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Več:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>Datoteka XML:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Vzorčno besedilo</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Odstavki: </translation>
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
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">točk</translation>
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
        <translation type="obsolete">pik</translation>
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
        <translation type="obsolete">pik</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nova</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Opis</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Uredi...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Uvozi</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Upravljaj lastnosti strani</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Premikaj predmete skupaj s stranjo</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Vrsta:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Znotraj:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Prednastavljene postavitve:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Uporabi nastavitve robov na vseh straneh</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Uporabi spremembe robov na vseh obstoječih straneh dokumenta</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med levim robom strani in levim mejnim vodilom. Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Razdalja med desnim robom strani in desnim mejnim vodilom. Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Robovi tiskanja ...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Uvozi robove za posamezne velikosti strani iz rapoložljivih tiskalnikov.</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Uredi glavne strani</translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation type="obsolete">Podvoji izbrano glavno stran</translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="obsolete">Izbriše izbrano glavno stran</translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="obsolete">Doda novo glavno stran</translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="obsolete">Uvozi glavne strani iz drugega dokumenta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Resnično želite izbrisati to glavno stran?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ime:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Nova glavna stran</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Nova glavna stran</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopija #%1 od </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Podvoji izbrano glavno stran</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Izbriši izbrano glavno stran</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Dodaj novo glavno stran</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Uvozi glavne strani iz drugega dokumenta</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nova glavna stran %1</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>Glavne strani ni mogoče preimenovati</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>Navadne strani ni mogoče preimenovati.</translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>Preimenuj glavno stran</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>Novo ime:</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Večkratno podvojevanje</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">pal</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Število kopij:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vodoravni zamik:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Navpični zamik:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Razdalje</translation>
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
        <translation>Kot:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Dolžina:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pik</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">pal</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Napaka skripte</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Če ste pognali uradno skipto, prosimo sporočite napako na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Pokaži k&amp;onzolo</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Skrij k&amp;onzolo</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Tudi to sporočilo je v odložišču. Uporabite Ctrl+V da ga prilepite v slednika hroščev.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Uvozi</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Spremeni...</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Uvozi predlogo</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Uvozi stran(i)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Iz dokumenta:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Uvozi stran(i):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> od 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Ustvari stran(i)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">pred stranjo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">za stranjo</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">na konec</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> od %1</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Iz dokumenta:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Uvozi stran(i):</translation>
    </message>
    <message>
        <source>&amp;Import Template</source>
        <translation type="obsolete">&amp;Uvozi predlogo</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens importWhereData
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Pred stranjo</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Za stranjo</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Na konec</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Uvozi glavne strani</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>Uvoz&amp;i glavne strani</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vpišite spisek uvoznih žetonov ločenih z vejicami. Žeton je lahko * za vse strani, 1-5 za območje ali številka posamezne strani.</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Pisava manjka</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Pisava %1 ni nameščena.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Uporabi</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>namesto</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>to:</source>
        <translation type="obsolete">Za:</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Premakni stran(i):</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">na konec</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation>Premakni strani</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiraj stran</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">Pred stranjo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">za stranjo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Premakni stran(i)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Pred stran</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Za stran</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Na konec</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Za:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Število kopij:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Premakni v ozadje</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Raven</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Širina</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Obdrži razmerje spremembe merila X in Y enako</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Izberi spodnji levi kot za izhodišče</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Lastnosti besedila po poti</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Spremeni nastavitve levih ali končnih točk</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Prezrcali vodoravno </translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Prezrcali navpično</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Izberi zgornji desni kot za izhodišče</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Namen upodobitve slike</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Razdalja med besedilom in dnom okvira</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Raven konec</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Uredi nastavitve tabulatorja v besedilnem okviru ...</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Razdalja do krivulje:</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Točka, od katere se merijo razdalje ali koti sukanja</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Navpični zamik slike znotraj okvira</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Razdalja med stolpci</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Izvorni profil slike</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Spremeni velikost slike navpično</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Zaobljen spoj</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Črta na dnu</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Širina stolpcev</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Višina:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Ime izbranega predmeta</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Premakni v ospredje</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Nasičenost barve črte besedila</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Končne točke</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Senca:</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Oblika:</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Navpični položaj trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Pisava za izbrano besedilo ali predmet</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Spremeni velikost slike vodoravno</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Nasičenost barve polnila besedila</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Leva točka</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vodoravni zamik slike znotraj okvira</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Razdalja med besedilom in vrhom okvirja</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Posebna razdalja</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Črta na vrhu</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Vrste spojev črt</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Debelina črte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Širina spremembe merila znakov</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Slog trenutnega odstavka</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Izberi spodnji desni kot za izhodišče</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Kvadraten konec</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Sukanje predmeta okrog trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Črta na desni</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Črta na levi</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Vrste zaključkov črt</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Pokaži krivuljo</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ročno zgoščevanje</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Zakleni ali odkleni predmet </translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Zaznavno</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Preklopi med presledkom in širino stolpca</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Barva črte besedila</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Nakaže raven, na kateri je predmet, 0 pomeni, da je predmet na dnu</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Naj slika ustreza velikosti okvira</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Zakleni ali odkleni velikost predmeta</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="obsolete">Naj besedilo iz nižjih okvirjev obliva obliko predmeta</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Izberi zgornji levi kot za izhodišče</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Uporabi raje razmerje stranic slike in ne okvira</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Premakni eno raven navzgor </translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Nagnjen spoj</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Slog črte trenutnega predmeta</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Premakni eno raven navzdol </translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spoj pod kotom</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Zaobljen konec</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometrija</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Razdalja med besedilom in levim robom okvira</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Izhodišče:</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Vključi ali izključi možnost tiskanja predmeta</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Barva polnila besedila</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Uredi obliko okvira ...</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Ohrani razmerja</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Razdalja med besedilom in desnim robom okvira</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Vhodni profil:</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Vzorec črte</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Izberi središče za izhodišče</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Črte celic</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Vodoravni položaj trenutnega izhodišča</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Dovoli, da je slika lahko druge velikosti kot okvir</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Nastavi radij zaobljenosti roba</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Izberi obliko okvira ...</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Razdalja besedila</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Število stolpcev v okviru besedila</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Razmik vrstic</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Začetni zamik:</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Oblika</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Besedilo</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Slika</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Črta</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Barve</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Sukanje:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Uredi obliko ...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>&amp;Zaobljeni
robovi:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Razdalja:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatorji ...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Besedilo &amp;poteka okrog okvira</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Uporabi &amp;okvir</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Uporabi obrobno črto</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Velikost pisave:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Zgoščevanje:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">&amp;Razmik vrstic:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>S&amp;log:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Prosto spreminjanje merila</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;Sprememba merila X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Sprememba &amp;merila Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Raztegni na &amp;velikost okvira</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Sorazmerno</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Izhodišče:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Vrsta črte:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Širina črte:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Robovi:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Konci:</translation>
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
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Pri oblivanju besedila uporabi pravokotno obrobo namesto oblike okvirja</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Pri oblivanju besedila uporabi dodatno črto na osnovi oblike okvirja</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Jezik za deljenje besed v tem okviru</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Pisava z leve na desno</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Začetna puščica:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Končna puščica:</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Nespremenljiv razmik vrstic</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Samodejno razmikanje vrstic</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Poravnaj na mrežo osnovnice </translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Dejanska X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Dejanska Y-DPI:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Zamik na osnovnico znakov</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Višina spremembe merila znakov</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ročno sledenje</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno.&lt;br/&gt;Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="obsolete">Barva črte besedila. Na voljo le z nastavitvijo pisave na &quot;obrobo&quot;.</translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="obsolete">Barva polnila besedila. Na voljo le z nastavitvijo pisave na &quot;obrobo&quot;.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Pravilo polnjenja</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Liho-sodo</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Neničelno</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation type="obsolete">Klikni za izbiro kako so vrstice razmaknjene</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Barva poteze črke in/ali sence, odvisno od izbire. Če sta izbrani obe, si delita isto barvo.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Barva izbranega besedila. Če je izbran učinek pisave Orisano, bo to tudi barva polnila. Če je izbrano senčenje besedila, bo to najbolj vrhnja barva.</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation>Besedilo v spodnjih okvirih naj obliva predmet. Možnosti spodaj določajo podrobnosti.</translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Uporabite okvirno škatlo, ki je vedno pravokotna, namesto oblike okvira za potek besedila ali okvire besedila pod predmetom. </translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Uporabite drugo vrstico, izvorno temelječo na obliki okvira, za potek besedila ali okvire besedila pod predmetom. </translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Klinite in držite pritisnjeno za izbor načina razmikanja črt.</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Raven konec</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Črta pika pika črta</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Zaobljen spoj</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Širina črte:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Ime &quot;%1&quot; ni edinstveno.
Prosim izberite drugo.</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Uredi slog</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Pravokotni konec</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Črtkana črta</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Pikčasta črta</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Nagnjen spoj</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spoj pod kotom</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Zaobljen konec</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Črta črta-pika</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Polna črta</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Ime &quot;%1&quot; ni edinstveno. Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Napredek</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Splošni napredek:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>MusterPages</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Uredi predloge</translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation type="obsolete">Podvoji izbrano glavno stran</translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="obsolete">Izbriše izbrano glavno stran</translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="obsolete">Doda novo glavno stran</translation>
    </message>
    <message>
        <source>Loads master page(s) from another document</source>
        <translation type="obsolete">Naloži glavno stran(i) iz drugega dokumenta</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Ali zares želite zbrisati to predlogo?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Da</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Ime:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nova predloga</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopija %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Ime:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopija #%1 izvirnika </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Običajno</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Ime:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopija #%1 </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Običajno</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopija %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nova predloga</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Uredi predloge</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nova</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ne</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Da</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Ime:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Ali zares želite zbrisati to predlogo?</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Moj &amp;vtičnik</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Moj vtičnik</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Vtičnik deluje!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">palcev</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Točk (pts)</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Velikost strani dokumenta; lahko je običajna ali po meri</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nov dokument</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dolžina strani v dokumentu; nastavljivo, če ste izbrali posebno velikost</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Usmerjenost strani dokumenta</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Možnosti</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Razdalja med samodejno ustvarjenimi stolpci</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Naj bo prva stran leva</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pik</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Širina strani v dokumentu; nastavljivo, če ste izbrali velikost po meri</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Ob dodajanju strani samodejno ustvari besedilne okvire</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Palcev (in)</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Omogoči enojno ali mnogotero postavitev</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Število stolpcev v samodejno ustvarjenih besedilnih okvirih</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="obsolete">Številka prve strani:</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Privzeta merska enota za urejanje dokumenta</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Vodila stolpcev</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetrov (mm)</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloidna</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Dno:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Številka &amp;prve strani:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Privzeta merska enota:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Samodejni besedilni okviri</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Razdalja:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Znotraj:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Poslovna</translation>
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
        <translation type="obsolete">Odpri dokument</translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="obsolete">Nedavni dokumenti</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Tega okna ne kaži več</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Začetno število strani dokumenta</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Š&amp;tevilo strani:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenti (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript datoteke (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG slike (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG slike (*.svg);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nov dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Odpri &amp;obstoječi dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Odpri nedavni &amp;dokument</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nova &amp;iz predloge ...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Naloži dokument z vnaprej nastavljeno postavitvijo</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Začni nov dokument iz predloge, ki jo je ustvaril drug uporabnik ali pa sam (npr. dokument, ki uporablja stalne sloge).</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Leva stran</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Desna stran</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Vozlišča</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Če označeno, uporabljaj koordinate glede na stran, 
sicer glede na predmet.</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Ponastavi to nadzorno točko</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Izbriši vozlišča</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Dodaj vozlišča</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Premakni vozlišča</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Premakni nadzorne točke</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Ponastavi nadzorne točke</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutne koordinate</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;Uredi obrobo</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Ponastavi obrobo</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Končaj urejanje</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Nadzorne točke premikaj neodvisno</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Nadzorne točke premikaj simetrično</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Odpri mnogokotnik ali odreži krivuljo bezier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Zapri to krivuljo bezier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Vodoravno prezrcali pot</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Navpično prezrcali pot</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Pot razreži vodoravno proti levi</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Pot razreži navpično navzgor</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Pot razreži navpično navzdol</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Pot zasukaj v nasprotni smeri urinih kazalcev</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Pot zasukaj v smeri urinih kazalcev</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="obsolete">Zmanjšaj velikost poti za prikazanih %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Povečaj velikost poti za prikazanih %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Kot sukanja</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">Povečaj ali zmanjšaj za %</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Vključi način za urejanje obrob</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Ponastavi obrobo na osnovno obliko okvira</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Pot razreži vodoravno proti desni</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Če je vključeno, so koordinate relativne glede na stran, sicer glede na predmet.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Zmanjšaj velikost poti za prikazanih %</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Zmanjšaj velikost poti za prikazano vrednost</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Povečaj velikost poti za prikazano vrednost</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>Odstotek povečave ali pomanjšave</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Vrednost, za katero naj se poveča ali pomanjša</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Ta dokument ni videti kot datoteka OpenOffice Draw.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Uvozi iz &amp;OpenOffice Draw ...</translation>
    </message>
    <message>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="obsolete">Uvozi OpenOffice Draw datoteke</translation>
    </message>
    <message>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="obsolete">Uvozi OpenOffice Draw datoteko v trenuten dokument in spremeni vektorske podatke v Scribusove predmete.</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Uvozi datoteke OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Uvozi večino datotek OpenOffice.org Draw v trenutni dokument, njihovi vektorski podatki se pretvorijo v predmete Scribusa.</translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenDocument 1.0 Draw</translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenDocument 1.x Draw</translation>
    </message>
    <message>
        <source>This file contains some unsupported features</source>
        <translation>Ta datoteka vsebuje nekatere nepodprte značilnosti</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Možnosti uvoznika OpenDocument</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Prepiši sloge odstavka</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>S tem bodo obstoječi slogi v trenutno odprtem Scribusovem dokumentu prepisani</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Združi sloge odstavka</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Združevanje slogov glede na lastnosti. Tako dobite manj podobnih slogov in jih obdržite, tudi če so v izvirniku poimenovani drugače.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Ime dokumenta uporabi kot predpono za sloge odstavka</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Ime dokumenta pripne slogom odstavka.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ne vprašaj več</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Naj bodo te nastavitve privzete. Pri uvažanju datotek OpenDocument OASIS ne sprašuj več.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Dokument Scribusa</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Dokument Scribusa 1.2.x</translation>
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
        <translation>Širina črte</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation type="obsolete">Konzola skripte</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Shrani kot PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Izvozi v datoteko:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Izvozi datoteko za vsako stra&amp;n posebej</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">S tem vključite shranjevanje ločenih PDF datotek za vsako stran dokumenta. Številke strani se samodejno dodajo. To je koristno za pripravo na tisk.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Datoteke PDF (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>S tem vključite shranjevanje ločenih PDF datotek za vsako stran dokumenta. Številke strani so dodane samodejno. To je koristno pri pripravi PDF za tisk.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Gumb Shrani bo onemogočen, če poskušate izvoziti kot PDF/X-3 in manjkajo podatki v poljih zavihka PDF/X-3.</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> točk</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Okvir</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Nizka</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete"> dpi</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete"> sec</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Visoka</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Stran</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Učinek zavzema</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Slike:</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Razdeli</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Razpusti</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Začetek učinkov škatlja in razcep.</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Izvozni profil za tiskanje. Če je mogoče izberite profil v povezavi z vašim tiskalnikom.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Ustvari PDF prispevke, kar je koristno pri pregledovanju povezanih člankov v PDFju.</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Polno barvanje</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Vodoravno</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Vključi barvne profile za čiste barve</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Izvozna resolucija za besedilo in vektorsko grafiko.
To ne vpliva na resolucijo slik.</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Velikost odmika od fizičnega levega robu strani</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Izberite glavno geslo, ki bo o(ne)mogočilo vse varnostne možnosti v izvoženem PDFju</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Pokaži predoglede strani</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Omogoči varnostne zmožnosti v izvoznem PDFju.
Če izberete Acrobat 4, bo PDF zaščiten s 40 bitno enkripcijo.
Če izberete Acrobat 5, bo PDF zaščiten s 128 bitno enkripcijo.
Pozor: PDF enkripcija ni tako zanesljiva kot GPG ali PGP in ima nekatere omejitve.</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Obseg izvoza</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Možnosti datoteke</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">Pisave</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Vključi povezave iz dokumenta.
To je uporabno za pregledovanje dolgih PDFjev.</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Desni rob</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Vrsta stiskanja slik.
Samodejno dovoli Scribusu, da sam izbere najboljši način.
ZIP je uporabna za slike z barvnimi ploskvami.
JPEG je boljši pri ustvarjanju majhnih PDFjev z mnogo slikami.
Če ne potrebujete posebnih nastavitev stiskanja, raje pustite samodejno.</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Vključi pisave:</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Velikost odmika od fizičnega desnega robu strani</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Pretvori slike na izbrano DPI resolucijo.
Če tega ne izberete, bodo slike risane pri izvirni resoluciji.</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Največja</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Ključna beseda za PDF/X-3, sicer izvoz ne uspe pravilno.
Predlagamo, da uporabite naslov dokumenta.</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Z leve proti desni</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Z desne proti levi</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Najmanjša</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Raven stiskanja: najmanj (25%), malo (50%), srednje (75%), močno (85%), najmočneje (95%)</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Polne barve</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Namen upodobitve:</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Zavese</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Brez učinkov</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Ne uporabi priloženih ICC profilov</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Dostopne pisave:</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Srednja</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Smer učinkov bleščice in metla.</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Levi rob</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Zaslon / Splet</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Zunaj</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Izvozi obseg strani v PDF</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Od zgoraj levo do spodaj desno</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Vključi barne profile za slike</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Barvni model za izvoz vašega PDFja.
Izberite Zaslon/splet za PDFje ki bodo brani na ekranu ali tiskani na običajnih tiskalnikih.
Izberite Tiskalnik, za tiskanje s CMYK tiskalniki.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Omogoči učinke za predstavitve, kadar uporabljate Acrobat Reader v celozaslonskem načinu.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Velikost odmika od fizičnega vrha strani</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Vse strani</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Tiskalnik</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Vključujem</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Nastavljač</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Stiskanje besedila in slik.
Če nimate dobrega razloga, raje pustite to vključeno, ker zmanjša velikost PDFja.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Shrani kot</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Smer gibajočih črt pri učinkih razcep in zavesa.</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Samo za predogled</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Namen upodobitve:</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Izvozi vse strani v PDF</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Čas kazanja strani, preden se prezentacija prične na izbrani strani</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Dodaj učinek na vse strani</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Pripravi pomanjšane sličice vsake strani v PDFju.
Nekateri pregledovalniki jih lahko uporabijo za pregledovanje.</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Gesla</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Samodejno:</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Namen upodobitve PDF/X-3</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Učinki</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Velikost odmika od fizičnega dna strani</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (Dots Per Inch) pri izvozu slik.</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Nasičenost</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Nastavitve</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Ne uporabi barvnih profilov iz izvirnih slik</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Vrsta učinka ob izrisu</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Določi združljivost PDF. Privzeta je naširše združljiva oblika Acrobat 4.
Izberite Acrobat 5, če vaš dokument uproablja zmožnosti PDF-1.4, kot na
primer prosojnosti, ali če potrebujete enkripcijo.
PDF/X-3 se uporablja za pošiljanje v tiskarno, in je omogočeno le, če 
uporabljate tudi upravljanje barv.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Dolžina učinka. Krajši čas učinek pospeši, daljši upočasni</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Splošno</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">Splošno</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Osvetli</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Namen upodobitve:</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Določi povezovanje strani PDF. Če niste prepričani
pustite na privzeti vrednosti - levo.</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Od spodaj navzgor</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Vključi pisave v PDF. To bo omogočilo, da se pri
prenosu izgled in postavitev dokumenta ne spremenita.</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Nastavitve slik</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Od zgoraj navzdol</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Navpično</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Znotraj</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Izberite geslo, s katerim bodo uporabniki lahko brali vaš PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Dovoli tiskanje PDFja. ČE ne izberete, bo tiskanje onemogočeno.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Dovoli spreminjanje PDFja. Če ne izberete, bo spreminjanje onemogočeno.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Dovoli kopiranje besedila in slik iz PDFja.\n
Če ne izberete, se besedila in slik ne bo dalo presneti.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Dovolite dodajanje opomb in drugih polj v PDF.\n
Če ne izberete, bo dodajanje polj onemogočeno.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Ustvari datoteko PDF </translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">&amp;Izvozi v datoteko:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">&amp;Spremeni...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">Vse str&amp;ani</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Izberi strani</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">&amp;Združljivost:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">&amp;Povezanost:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Pripravi &amp;sličice</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">&amp;Povezane besedilne okvirje shrani kot članke PDF </translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Vključi opombe</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Resolucija:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metoda:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kakovost:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;Slike reduciraj na:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Vključi vse pisave</translation>
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
        <translation type="obsolete">Pokaži pred&amp;oglede strani</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">Dolžina &amp;prikazovanja:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Trajanje &amp;učinka:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">&amp;Vrsta učinka:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">&amp;Premikanje črt:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">&amp;Iz:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">&amp;Smer:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">&amp;Dodaj učinek na vse strani</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">&amp;Uporabi enkripcijo</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Uporabnik:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">&amp;Lastnik:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Dovoli &amp;tiskanje dokumenta</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Dovoli &amp;spreminjanje dokumenta</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Dovoli &amp;kopiranje besedila in slik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Dovoli &amp;dodajanje opomb in polj</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">&amp;Varnost</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">&amp;Namen izhoda:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Uporabi posebne nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">&amp;Frekvenca:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Kot:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">Funkcija &amp;spot:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Enostavna točka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Črta</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Okroglo</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Elipsa</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Uporabi ICC profil</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;Podatki:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Profil &amp;izhoda:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF datoteke (*.pdf);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">To je napredna nastavitev, ki ni privzeta. Vključite jo le, če jo potrebujete zaradi tiskalnika ali
navodil iz tiskarne. Sicer se vaš PDF morda ne bo pravilno natisnil in ne prav prikazal na drugih
sistemih.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Stiskanje besedila in &amp;vektorske grafike</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">&amp;Vključi predstavitvene učinke</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Predstavitev</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">&amp;Omejeno vključi vse pisave</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Pisave za omejiti:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Vodoravno prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Navpično prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="obsolete">Shrani kot PDF</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="obsolete">Izvozi datoteko za vsako stra&amp;n</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">S tem vključite shranjevanje ločenih PDF datotek za vsako stran dokumenta. Številke strani se samodejno dodajo. To je koristno za pripravo na tisk.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Shranjevanje PDF</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Izvažanje glavnih strani:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Izvažanje strani:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Izvažanje predmetov na trenutni strani:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Predogled tiskanja</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Vključi/izključi barvno ploščo M (magentna)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Vključi/izključi barvno ploščo K (črna)</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Vključi/izključi barvno ploščo C (cijanska)</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Prikaže prosojnost in prosojne predmete v dokumentu. To zahteva vsaj Ghostscript 7.07 ali novejšega</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Pisave vrste True-Type in Open-Type, ter EPS, PDF in vektorske grafike izgledajo prijetneje
na račun malce počasnejšega izrisovanja.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Prikaže predogled tiskanja s simulacijo splošnih barvil CMYK namesto barv RGB</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Vključi/izključi barvno ploščo Y (rumena)</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Omogoča lepši izgled besedil, na račun malce počasnejšega \n
izrisovanja. Vpliva samo na pisave vrste Type 1</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">Mehčaj &amp;besedilo</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">Mehčaj &amp;slike</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Prikaži &amp;prosojnost</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Prikaži CMYK</translation>
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
        <translation>&amp;Y (R)</translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation>&amp;K</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Prijetneje izrisuje TrueType in OpenType pisave, ter EPS, PDF in vektorsko grafiko.
Predogled se na račun tega malce upočasni</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="obsolete">Zaznane napake.
Poskusite uporabiti &quot;Pregled pred poletom&quot;, da jih odpravite</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Opusti</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Zanemari</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Ime separacije</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cijanska</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Rumena</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Črna</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Sprememba merila:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Natisni ...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Vključi lepši izgled besedilnih predmetov, na račun malce počasnejšega izrisovanja. Vpliva le na pisave Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Vključi lepši izgled TruType pisav, OpenType pisav, EPS, PDF in vektorskih grafik, na račun malce počasnejšega izrisovanja</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Metoda nadomeščanja odtenkov sivine, ki nastanejo kot mešanica cijanske, magente in rumene, s črno. To najbolj vpliva na nevtralne dele slike, ki so blizu sivi. S tem se lahko izboljša rezultat tiskanja, priporočamo pa preverjanje vsakega primera posebej. To tudi zmanjpa možnost prevelike nasičenosti črnil CMY.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Spremeni merilo strani.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation>Omogoči &amp;glajenje robov</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Ponuja prijaznejši ogled pisav Type 1, TrueType, OpenType, dokumentov EPS, PDF ter vektorske grafike v predogledu, pri čemer je predogled malce počasnejši</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Obdelovanje glavnih strani:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Izvažanje strani:</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Izključeno</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">manjka!</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Datoteka: </translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Okvir besedila</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Slika</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Natisni: </translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Program</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Znaki: </translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopija</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Besedilo po poti</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Povezano besedilo</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Vključeno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Odstavki: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Besede: </translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Uredi besedilo...</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopiraj sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Premakni sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Prilepi</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Pokaži &amp;meje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Pokaži &amp;okvirje</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Pokaži &amp;slike</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Kaži &amp;mrežo</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Prikaži &amp;vodila</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Kaži &amp;osnovno mrežo</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Preskakuj do mreže</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">&amp;Preskakuj do vodil</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Izvorna PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Dejanska PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">&amp;Podatki</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Dodaj sliko...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">&amp;Slika je vidna</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Osveži sliko</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Uredi sliko</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Okvir &amp;prilagodi sliki</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Najdi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">&amp;Dodaj besedilo...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Uredi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Vstavi naključno besedilo</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Je PDF &amp;beležka</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">je PDF &amp;opomba</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Lastnosti opombe</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Lastnosti &amp;polja</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Možnosti &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Zakleni</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Od&amp;kleni</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Zakleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Odkleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Pošlji na &amp;odložišče</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Pošlji na &amp;sloj</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Združi</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">&amp;Razdruži</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">&amp;Raven</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Pošlji na &amp;dno</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Prinesi na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Dvigni</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">&amp;Okvir slike</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">&amp;Mnogokotnik</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Obrobe</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Okvir besedila</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezier krivulja</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">&amp;Pretvori v</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">&amp;Počisti vsebino</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Prikaži &amp;lastnosti...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Skrij &amp;lastnosti...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ali zares želite počistiti vso besedilo?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Line</source>
        <translation>Črta</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Slika</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomljena črta</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnogokotnik</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Besedilo po poti</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopija</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>V odnosu z</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Je starš</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Je otrok</translation>
    </message>
    <message>
        <source>Page Item Attributes</source>
        <translation type="obsolete">Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Ime</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Vrednost</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Odnos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Odnos do</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiraj</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Počisti</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vrednost</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Odnos</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Odnos do</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation type="obsolete">Prikaz strani</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Prva stran:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Postavitev dokumenta</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Dvostransko</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Srednja desna</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Za brisanje povlecite strani ali glavne strani v koš</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Tu so vse vaše glavne strani. Za pripravo nove strani povlecite glavno stran v spodnji pogled</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Razporedi strani</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Glavne strani na voljo:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Strani dokumenta:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Stran </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> od %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 od %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 od %1</translation>
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
        <translation>Foolscap</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Pismo</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation>Vladno pismo</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Tabloid</translation>
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
        <translation>Veliki Post</translation>
    </message>
    <message>
        <source>Demy</source>
        <translation>Demy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Srednja</translation>
    </message>
    <message>
        <source>Royal</source>
        <translation>Kraljevska</translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation>Slonovska</translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation>Dvojna Demy</translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation>Četvorna Demy</translation>
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
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Result</source>
        <translation>Rezultat</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Izberi</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Najdi rezultate za: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Predogled</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Pojdi na</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Pot</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Natisni</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Manjkajoče</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Iskanje</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Stanje</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Slike</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Preliči iskanje</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Upravljaj s slikami</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Iskanje slik</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Neuspešno iskanje: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Ni slike z imenom &quot;%1&quot;.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Izberi osnovno mapo za iskanje</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Shrani kot &amp;sliko ...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Izvozi kot sliko</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Izbrane strani izvozi kot rastrske slike.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Vtičnika ni mogoče najti</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>Neznana napaka</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Simbola ni mogoče najti (%1)</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Uvozi</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Neznano</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Vtičnik: nalaganje %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>zaganjanje neuspešno</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>neznana vrsta vtičnika</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Vtičnik: %1 naložen</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Vtičnik: %1 ni naložen: %1</translation>
    </message>
    <message>
        <source>PostScript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">PostScript datoteke (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG slike (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG slike (*.svg);;</translation>
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
        <translation>Upravljalec vtičnikov</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Vtičnik</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Kako pognati</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Ga želite naložiti?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>ID vtičnika</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Za uveljavitev sprememb morate ponovno zagnati program.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>Polygon Properties</source>
        <translation>Lastnosti mnogokotnika</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Koti:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Število kotov mnogokotnika</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Stopinje zasuka mnogokotnika</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Primer poligona</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Uveljavi &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Spremeni obliko mnogokotnika z uporabo faktorja vbočenosti ali izbočenosti </translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Negativna vrednost pomeni vbočenost (zvezdastost) mnogokotnika,
pozitivna pa izbočenost</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>&amp;Koti:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Sukanje:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Uveljavi &amp;faktor</translation>
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
        <translation>Število kotov mnogokotnika</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Zasukanost mnogokotnika v stopinjah</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Spremeni obliko mnogokotnika z uporabo faktorja vbočenosti ali izbočenosti </translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vzorčni poligon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Negativna vrednost pomeni vbočenost (zvezdastost) mnogokotnika,
pozitivna pa izbočenost</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Negativna vrednost pomeni vbočenost (zvezdastost) mnogokotnika, pozitivna pa izbočenost</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete"> px</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Grafični vmesnik</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Položaj urejevalnika slik na sistemu</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Uporabi glajenje pisav pri zaslonski upodobitvi EPS in PDF</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Meniji</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Razno</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Poti</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Drugo</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Majhen</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Enote</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Naj bo prva stran dokumenta leva</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="obsolete">Druge možnosti</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Lastnosti risanja poligonov</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="obsolete">Izberi velikost predogleda v paleti odložišča</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Razpored pod vodilom pisave na črti</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Razdalja med malimi črtami mreže</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Razdalja med glavnimi črtami mreže</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Zunanja orodja</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Nasičenost barve polnila</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Nasičenost barve črte</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Male črke</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Naj bo mreža pod predmeti na strani</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Kadar uporabljaš nasproti postavljene strani, jih kaži dve po dve</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Velikost pisave v novih okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Relativna velikost podpisave v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Barva polnila okvirjev s slikami</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Lastnosti risanja oblik</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Razpored nad vodilom pisave na črti</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Uporabi glajenje robov slik pri zaslonskem upodabljanju EPS in PDF</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Slog črt</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Postscript upodobljevalnik</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Barva črte pri oblikah</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Barva polnila pri oblikah</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Naj bosta vodoraven in navpičen razteg ista</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Vodila</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Barve mreže</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Število kotov poligona</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Srednje</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Interval samodejnega shranjevanja</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Postavitev mreže</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Če izberete to, bo Scribus shranil varnostno kopijo
vašega dokumenta (s končnico .bak), vsako izbrano enoto časa</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografija</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Število vrstic, ki jih preskoči premik kolešča na miški</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Pisava v novih okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Najmanjša dovoljena povečava</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Največja dovoljena povečava</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Vključi osnovno mrežo</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Širina strani dokumenta; to lahko nastavite, če ste označili velikost strani po meri</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Lastnosti risanja črt</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Orodje za obdelavo slik</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Privzeta stopnja povečave</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Samodejno razpete slike ohranijo svoja razmerja</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papirja</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Postavitev</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Stopinje zasuka poligona</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Barva vodil robu</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Predogled</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Vodoraven razteg slik</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Širina črt</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Slog črte pri oblikah</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Privzeta velikost pisave menijev in oken</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Piškur molče grabi fižol iz dna cezijeve hoste.</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Večjost razmaka črt v primerjavi z velikostjo pisave v odstotkih</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Navpičen razteg slik</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Odložišče</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">Za prilagoditev prikaza povlecite spodnje vodilo.</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Lastnosti okvija za sliko</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Tiskanje</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Privzete vrednosti povečav</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Privzeta mapa s Scripterjevimi skriptami</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pike</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Ne kaži predmet, ki so zunaj robov</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Podpisano</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Nadpisano</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Število stolpcev v okvirjih z besedilom</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Naj bo mreža pred predmeti na strani</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Relativna velikost napisaneve v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Relativna velikost pomanjšanih vleikih črk v primerjav iz običajno velikostjo pisave</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Okvir s sliko lahko razpne sliko na kakršnokoli velikost</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Izključi osnovno mrežo</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Barva malih črt mreže:</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Barva večjih črt mreže:</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Privzeta mapa z ICC profili</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Lastnosti okvirja z besedilom</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Privzeta mapa z dokumenti</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Privzeta usmeritev strani dokumenta</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Barva vodilnih črt</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Pike (pt)</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Prikaz</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Število nedavno uporabljenih dokumentov, ki naj bodo prikazani v meniju Datoteka</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Nasičenost barve</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Palcev (in)</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Vključi enotno ali razpršeno postavitev</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Sprememba povečave v vsaki zoom operaciji</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Razmak med stolpci okvirja z besedilom</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Privzeta velikost strani; lahko je običajna ali po meri</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Debelina črte pri oblikah</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Primer pisave</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Samodejno shranjevanje</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Nastavitve</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Osnovna črta mreže</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="obsolete">Položaj upodobljevalnika Ghostscript na sistemu</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Izberite privzet izgled in okraske oken.
 Scribus uporabi vse dostopne KDE ali QT teme.</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Slike v okvirjih s slikami naj bodo razpete do elikosti okvirja</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Označi področje zunaj robov z barvo robu</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Splošno</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Uporabi sposobnosti prosojnosti pri izvozu v PDF-1.4</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Doseg oprijemanja predmetov</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="obsolete">Shrani vsebino odložišča ob vsaki spremembi</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Privzeta merska enota ob urejanju dokumenta</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Barva črt</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Višina strani dokumenta; to lahko nastavite, če ste izbrali velikost strani po meri</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Primer poligona</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetri (mm)</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Razdalja do vodila, na kateri bo predmet preskočil na vodilo</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Barva pisave</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Velikost pisave:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Nastavitve miške</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Preskok koleščka:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Radij dosega:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nedavni dokumenti:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenti:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>Profili &amp;ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skripte:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Vključeno</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Presledek:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Razmik &amp;malih mrežnih črt:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Razmik &amp;glavnih mrežnih črt:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Doseg &amp;preskakovanja do vodil:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Barva &amp;malih mrežnih črt:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Barva &amp;glavnih mrežnih črt:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Barva &amp;uporabnikovih vodil:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">&amp;Barva osnovne mreže:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">V &amp;ozadju</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">V o&amp;spredju</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">&amp;Vključeno</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">&amp;Izključeno</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Raztegovanje:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">R&amp;aztegovanje:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Ra&amp;ztegovanje:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">&amp;Osnova mreže:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">&amp;Zamik osnove mreže:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">&amp;Samodejni razmiki &amp;vrstic:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Privzeta &amp;pisava:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">Privzeta &amp;velikost:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">Barva &amp;besedila:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Razpoka:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Barva &amp;črte:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Senčenje:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">S&amp;enčenje:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Vrsta črte:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">&amp;Širina črte:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">&amp;Najmanj:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Na&amp;jveč:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Po korakih:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">&amp;Prosto raztegovanje</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Vodoravno raztegovanje:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">&amp;Navpično raztegovanje:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Raztegni sliko na velikost okvirja</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Ohrani &amp;razmerja</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Koti:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Vrtenje:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="obsolete">&amp;Shrani vsebino ob spremembah</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="obsolete">Velik</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Prikazuj strani &amp;drugo ob drugi</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Barve strani</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Ozadje:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Meje:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Prikaži &amp;nenatisljivo območje v barvi robov</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Uporabi PDF 1.4 &amp;prosojnosti</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Prilagodi velikost prikaza</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Ime zagonske datoteke:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Gladi robove &amp;besedila</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Gladi robove &amp;slik</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Ime &amp;zagonske datoteke:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">&amp;Reži po mejah strani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Znotraj:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="obsolete">Uveljavi &amp;odstranjevanje barve pod</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">P&amp;redloge:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Spremeni ...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Uveljavi &amp;faktor</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Dodatni imeniki za predloge dokumentov</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Spremeni obliko mnogokotnika z uporabo faktorja vbočenosti ali izbočenosti </translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Negativna vrednost pomeni vbočenost (zvezdastost) mnogokotnika,
pozitivna pa izbočenost</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">To je način za zamenjavo sivin sestavljenih iz cijan, rumene in 
magenta barve s črnino. 
POD se najbolj izrazi na nevtralnih ali temnih delih slik, blizu sivi. Tako 
lahko izboljšate kakovost tiska nekaterih slik, vendar se splača prej 
narediti nekaj preiskusov. 
POD tudi zmanjša možnost prekomerne nasičenosti CMY črnil.</translation>
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
        <translation type="obsolete">Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloidna</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Jezik:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Enote:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Razveljavi/ponovi</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Obseg zgodovine dejanj</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Deljenje</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Pisave</translation>
    </message>
    <message>
        <source>Doc-Checker</source>
        <translation type="obsolete">Pregled dokumenta</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Upravljanje barv</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Izvoz PDF</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tipke za bližnjice</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Prikaz strani</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Pokaži slike</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Pokaži verige besedil</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Pokaži okvire</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Prazen prostor</translation>
    </message>
    <message>
        <source>External Tools.</source>
        <translation type="obsolete">Dodatna orodja.</translation>
    </message>
    <message>
        <source>Always ask before Fonts are replaced when loading a Document</source>
        <translation type="obsolete">Ob nalaganju dokumenta vedno vprašaj preden zamenjaš pisave</translation>
    </message>
    <message>
        <source>Plugin Manager</source>
        <translation type="obsolete">Upravljalec dodatkov</translation>
    </message>
    <message>
        <source>&amp;Refresh</source>
        <translation type="obsolete">&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="obsolete">&amp;Posodobi</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Vtičniki</translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
        <translation type="obsolete">Vključi ali izključi povezane okvirje</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Vključi ali izključi prikazovanje okvirov</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Vključi ali izključi prikazovanje slik</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Izberite jezik, ki naj se privzeto uporablja v Scribusu.
Če pustite prazno, se bo nastavitev zaznala iz nastavitev sistema.
Vedno lahko uporabite tudi možnost v ukazni vrstici ob zaganjanju</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Nastavite število korakov v zgodovini popravkov.
Če nastavite na 0, se bo shranilo neomejeno število.</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Pr&amp;edloge dokumeta:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Končno preverjanje</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Kazalo vsebine in seznami</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Pokaži ukazne znake v besedilu</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Vodila glede na stran</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Presledki med stranmi</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodoravno:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Navpično:</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Za prilagoditev pogleda povlecite spodnje ravnilo z drsnikom.</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Ločljivost:</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Vedno vprašaj, preden se pri nalaganju dokumenta zamenjajo pisave</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Ob urejanju slogov je viden predogled trenutne sloga odstavka</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Pokaži začetno pogovorno okno</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Vzorčno besedilo</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Vedno uporabljaj običajno vzorčno besedilo</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Števec odstavkov:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Razno</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Datoteka</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Da</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ne</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Prikaži nenatisljive znake, kot je konec odstavka v besedilnih okvirih</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="obsolete">PRivzet imenik z ICC profili. To se ne spremeni ob odprtju dokumenta.</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Dodatna mapa s predlogami dokumentov</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Položite ravnilo ob svoj zaslon in povlecite drsnik tako, da nastavite pravilno povečavo, pri kateri bo Scribus strani in predmete prikazal v pravi velikosti</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="obsolete">Lokacija urejevalnika slik. Če uporabljate gimp predlagamo ukaz gimp-remote, ki omogoča urejanje slike v že odprtem gimpu.</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Določi količino prostora levo od platna, ki bo deloval kot odložišče za ustvarjanje in popravljanje elementov ter za njihovo vleko na aktivno stran</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Določi količino prostora desno od platna, ki bo deloval kot odložišče za ustvarjanje in popravljanje elementov ter za njihovo vleko na aktivno stran</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Določi količino prostora nad platnom, ki bo deloval kot odložišče za ustvarjanje in popravljanje elementov ter za njihovo vleko na aktivno stran</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Določi količino prostora pod platnom, ki bo deloval kot odložišče za ustvarjanje in popravljanje elementov ter za njihovo vleko na aktivno stran</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Določite Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Določite urejevalnik slik</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Tolmač PostScripta</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Vključi ali izključi  prikaz povezanih okvirov.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Izberite privzeti jezik, v katerem naj se požene Scribus. Če pustite polje prazno, bo jezik zaznan iz sistemskega okolja. To lahko vedno nadomestite z zastavico v ukazni vrstici</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>&amp;Velikost pisave (meniji):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Velikost pisave (&amp;palete):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Izberite privzeti videz okna. Scribus podeduje vse razpoložljive teme KDE ali Qt, če je Qt nastavljen, da išče po vtičnikih KDE.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Priveta velikost pisave za orodna okna</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Privzet imenik s profili ICC. To ni mogoče spremeniti z odprtim dokumentom. Privzeta nastavitev je, da Scribus preišče sistemske mape v sistemih Mac OSX in Windows. V okoljih Linux ali Unix Scribus preišče mape $home/.color/icc,/usr/share/color/icc in /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Če je vključeno, Scribus shrani varnostno kopijo vašega dokumenta s končnico .bak vsakič, ko preteče določeni čas</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Nastavi, za koliko nazaj se ukazi shranjujejo po korakih. Nastavitev 0 pomeni, da se bo shranilo neskončno ukazov.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Pot do mape, kjer hranite svoj urejevalnik slik. Če uporabljate gimp in ga vaša distribucija vključuje, predlagamo, da nastavite &apos;gimp-remote&apos;, s tem lahko urejate slike v gimpu, ki je že zagnan.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Metoda nadomeščanja odtenkov sivine, ki nastanejo kot mešanica cijan, magenta in rumene, s črno. To najbolj vpliva na nevtralne dele slike, ki so blizu sivi. S tem se lahko izboljša rezultat tiskanja, priporočamo pa preverjanje vsakega primera posebej. To tudi zmanjpa možnost prevelike nasičenosti CMY črnil.</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter.</source>
        <translation type="obsolete">Pot do lokacije kjer je Ghostscript prevajalec.</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation>Dodajte pot za prevajalnik Ghostscript. V okolju Windows potrebujete povezavo do programa z imenom gswin32c.exe - IN NE gswin32.exe. Sicer se bo ob zagonu Scribusa slednji morda obesil.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Privzeto</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Shrani ...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Shrani nastavitve</translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Single Page</source>
        <translation type="obsolete">Sama stran</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="obsolete">Dvojne strani</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Leva stran</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Desna stran</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="obsolete">Trojno zloženo</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">Sredina</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="obsolete">Četrvorno zloženo</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">Srednja leva</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Srednja desna</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Želite prenesti stare nastavitve Scribusa?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus je zaznal datoteke nastavitev različice 1.2.
Ali jih želite uporabiti z novo različico?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Datoteke z nastavitvami &quot;%1&quot; ni mogoče odpreti za pisanje: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Neuspešno pisanje v datoteko nastavitev &quot;%1&quot;: QIODevice koda stanja %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Neuspešno odpiranje datoteke nastavitev &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Neuspešno branje nastavitev XML iz &quot;%1&quot;: %2 v vrstici %3, znak %4</translation>
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
        <translation>Napaka pri zapisovanju nastavitev</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus ni uspel shraniti svojih nastavitev: &lt;br&gt;%1&lt;br&gt;Preverite dovoljenja datoteke in mape ter razpoložljiv prostor na disku.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Napaka pri nalaganju nastavitev</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus ni uspel naložiti svojih nastavitev: &lt;br&gt;%1&lt;br&gt;Naložene bodo privzete vrednosti.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Book</source>
        <translation type="obsolete">Knjiga</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Revija</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, &apos;Book&apos; sets margins classically (Gutenberg). &apos;Book&apos; is proposed for two-sided documents. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Tu lahko izberete preddoločeno postavitev strani. &apos;Brez&apos; pusti robove pri miru, &apos;Knjiga&apos; jih določi klasično (Gutenberg). &apos;Knjiga&apos; je predlog za dvostranske dokumente. &apos;Revija&apos; določi vse robove na isto vrednost, prednost ima leva ali notranja vrednost.</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Zlato razmerje</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Devet delov</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Tu lahko izberete vnaprej določeno postavitev strani. &apos;Brez&apos; pusti robove pri miru, Gutenberg jih določi klasično. &apos;Revija&apos; določi vse robove na isto vrednost, prednost ima leva ali notranja vrednost.</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Brez</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Odpri ...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Shrani &amp;kot ...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>I&amp;zhod</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Poženi</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Poženi kot &amp;konzolo</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Shrani izhod ...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skripta</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Konzola Python Scribusa</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>To je izvedeno iz standardne Python konzole, tako da vsebuje nekaj omejitev, še posebej glede praznega prostora. Za več informacij preberite navodila.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Konzola skripte</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Sem vpisujete ukaze. Izbira je izvajana kot skripta</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Izhod vaše skripte</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Skripte Python (*.py)</translation>
    </message>
    <message>
        <source>Open File With Python Commands</source>
        <translation type="obsolete">Odpri datoteko z Python ukazi</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Shrani datoteko z ukazi Python</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Besedilne datoteke (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Shrani trenuten izhod</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Odpri datoteko s skripto Python</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>Na&amp;s:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Vred:</translation>
    </message>
    <message>
        <source>Hu&amp;e:</source>
        <translation>O&amp;barvanost:</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Določi barve po meri &gt;&gt;</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Izberi barvo</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Dodaj barvam po meri</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Modra:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Zelena:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Osnovne &amp;barve</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Barve po &amp;meri</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Kanal &amp;alfa:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rdeča:</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ne</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Mapa</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Nazaj</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Shrani</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Velikost</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Razvrsti</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Odpri</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Napaka</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Pisanje: %1</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Razvrsti po veliko&amp;sti</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Razvrsti po &amp;datumu</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nova mapa 1</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>Mapa</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Vrs&amp;ta datoteke:</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Ime datoteke:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Znova naloži</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nova mapa</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Nerazvrščeno</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Poglej &amp;v:</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Predoglej vsebino datoteke</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nova mapa %1</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Za pisanje in branje</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Samo za branje</translation>
    </message>
    <message>
        <source>Copy or Move a File</source>
        <translation>Prepiši ali premakni datoteko</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Poišči mapo</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Pokaži &amp;skrite datoteke</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Nedostopno</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Datoteke ni mogoče najti.
Preverite ime in pot do nje.</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Seznamski pogled</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Posebno</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Samo za pisanje</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>povezava</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Mape</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Povezava do posebnega predmeta</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Izberite mapo</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Vse datoteke (*.*)</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Branje: %1</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>P&amp;reimenuj</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Mapa:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Mapo navzgor</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Pokaži podatke datoteke</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>datoteka</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Ustvari novo mapo</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Povezava do datoteke</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Povezava do mape</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Podroben pogled</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Razvrsti po &amp;mapah</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Izbriši %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Resnično želite izbrisati %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Da</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Pisava</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>S&amp;log pisave</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Učinki</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Pre&amp;črtano</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Podčrtano</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Vzorec</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>S&amp;kripta</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Uporabi</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Izberite pisavo</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>&amp;Redo</source>
        <translation>P&amp;onovi</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Počisti</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Izberi vse</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>V vrsto</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Prilagodi ...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;O Qt&lt;/h3&gt;&lt;p&gt;Ta program uporablja QT različice %1.&lt;/p&gt;&lt;p&gt;Qt je C++ orodje za razvoj grafičnih vmesnikov in programov za več okolij.&lt;/p&gt;&lt;p&gt;Qt omogoča prenos iste kode v okolja MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux in vse pomembnejše različice Unixov.&lt;br&gt;Qt obstaja tudi za vdelane naprave.&lt;/p&gt;&lt;p&gt;Qt je izdelek podjetja Trolltech. Za več informacij obiščite &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>No</source>
        <translation>Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Da</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Predogled tiskanja</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Poskušate ubesediti predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Slike SVG (*.svg *.svgz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Kličete predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python Skripte (*.py);; Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">Skripta</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Joj! Poskušate zbrisati predmet ki ne obstaja!</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ste prepričani, da želite prepisati datoteko:
%1 ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Spletna pomoč</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Opozorilo</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Slike SVG (*.svg);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Priprave v teku ...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Shrani kot sliko</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Napaka pri zapisovanju izhodnih datotek.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Napaka pri zapisovanju izhodnih datotek.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Izvoz je bil uspešen.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Datoteka že obstaja. Jo želite prepisati?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>že obstaja. Jo želite prepisati?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Da, vedno</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Predogled &amp;pisav</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Vstavi posebej</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Nova &amp;iz predloge...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Bilteni</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brošure</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogi</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Letaki</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Znaki</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kartice</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Glave pisem</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Ovojnice</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Vizitke</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Koledarji</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Oglasi</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Nalepke</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Meniji</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programi</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Obrazci PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Predstavitve PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revije</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakati</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Razglasi</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Besedilne datoteke</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Zgibanke</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Lastne predloge</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Shrani kot &amp;sliko...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Pred&amp;ogled tiskanja</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Uvozi &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Vse podprte oblike (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Shrani kot &amp;predlogo...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">Priročnik za s&amp;kripte...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Skribusove skripte</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Poženi skripto ...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Nedavne skripte</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Pokaži &amp;konzolo</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Shrani stran kot &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Uvozi &amp;SVG...</translation>
    </message>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Aaaa! Napačni argumenti! Kličite:</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Aaaa! Sliko poskušate vklučiti v predmet ki ne obstaja ali ni izbran!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Aaaa! Poskušate (od)kleniti predmet ki ne obstaja! In tudi noben drug ni izbran.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Aaaa! Povprašujete po predmetu, ki ne obstaja! In tudi noben ni izbran.</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Uvažanje besedila</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Vse podprte vrste</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Datoteke HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Datoteke z besedilom</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Datoteke z vrednostmi, ločenimi z vejicami</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CVS_podatki</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CVS_glava</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Pisava %1 je okvarjena, zato bo izpuščena</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Predloga:</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Zunanje povezave
</translation>
    </message>
    <message>
        <source>OO.o Writer Documents</source>
        <translation type="obsolete">Dokumenti OO.o Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtri za besedila</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Ovitki medijev</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>albanski</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>baskovski</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>bolgarski</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="obsolete">Brazilščina</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>katalonski</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>kitajski</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>češki</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>danski</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>nizozemski</translation>
    </message>
    <message>
        <source>English</source>
        <translation>angleški</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>angleški (Velika britanija)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>nemški</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>finski</translation>
    </message>
    <message>
        <source>French</source>
        <translation>francoski</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>galicijski</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>grški</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>madžarski</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>indonezijski</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>italijanski</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>korejski</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>litovski</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>norveški (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>norveški (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>norveški</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>poljski</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>ruski</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>švedski</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>španski</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>španski (latinski)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>slovaški</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>slovenski</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>srbski</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Poskus da bi bil napredek večji kot največji</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;O skriptah ...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>O skriptah</translation>
    </message>
    <message>
        <source>Import &amp;Open Office Draw...</source>
        <translation type="obsolete">Uvozi &amp;OpenOffice Draw...</translation>
    </message>
    <message>
        <source>Open Office Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice Draw (*.sxd;;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Barve s praznim imenom ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Barve s praznim imenom ni mogoče spremeniti.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Barve s praznim imenom ni mogoče ustvariti.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Barve s praznim imenom ni mogoče izbrisati.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Barve s praznim imenom ni mogoče zamenjati.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Enota je izven veljavnega obsega. Uporabite eno od Scribusovih konstant UNIT_*.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Cilj ni slikovni okvir.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Kotni polmer mora biti pozitivno število.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Velikosti pisave nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Pisave nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Velikosti besedila nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Stolpcev nebesedilnega okvirja ni mogoče prešteti.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Razmika vrstic nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Razmika stolpcev nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Besedila nebesedilnega okvirja ni mogoče pridobiti.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Besedila nebesedilnega okvirja ni mogoče nastaviti.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Besedila nebesedilnega okvirja ni mogoče vstaviti.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Poravnava je izven dovoljenega obsega. Uporabite eno od Scribusovih konstant ALIGN_*.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Števnik izbire je izven meja</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="obsolete">Uvozi iz &amp;OpenOffice Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Dokumenti OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Barve ni mogoče najti - napaka python </translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Nastavitve po meri (neobvezno): </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Navadne nastavitve:</translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Kratke &amp;besede...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Obdelovanje kratkih besed. Prosim, počakajte ...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Obdelava kratkih besed. Opravljeno.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>afrikaanski</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>turški</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>ukrajinski</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>valižanski</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Ime datoteke mora biti niz znakov.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Nastavitve vrste slike ni mogoče izbrisati.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Vrsta slike mora biti niz.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Atribut &apos;allTypes&apos; je samo za branje</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Neuspešen izvoz slike</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Sliki ni mogoče spremeniti merilo za 0%.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Izbrani predmet ni slikovni okvir.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Pisave ni mogoče najti.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Praznega vzorca ni mogoče upodobiti.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Plast ne more imeti praznega imena.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Plasti ni mogoče najti.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Zadnje plasti ni mogoče najti.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Plasti brez imena ni mogoče ustvariti.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Števec izbire je izven meja.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti poravnave besedila.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Velikost pisave je izven meja - mora biti &lt;= velikost &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti velikosti pisave .</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče določiti pisave.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Razmik med vrsticami je izven dosega, mora biti &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti razmika vrstic.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Razmik med stolpci je izven meja, mora biti pozitiven.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti razmika med stolpci.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Število stolpcev je izven meja, mora biti &gt;1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti števila stolpcev.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>V nebesedilnem okviru ni mogoče izbrati besedila</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>V nebesedilnem okvirju ni mogoče izbrisati besedila.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>V nebesedilnem okviru ni mogoče nastaviti polnila besedila.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>V nebesedilnem okviru ni mogoče nastaviti poteze besedila.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>V nebesedilnem okviru ni mogoče nastaviti senčenja besedila.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Le besedilne okvire je mogoče povezovati.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Ciljni okvir mora biti prazen.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Ciljni okvir je že povezan z drugim okvirom.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Ciljni okvir je že povezan z drugim okvirom.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Izvor in cilj sta en in isti predmet.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nebesedilnega okvira ni mogoče razvezati.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Nebesedilnega okvira ni mogoče pretvoriti v krivulje.</translation>
    </message>
    <message>
        <source>Brazilian Portugese</source>
        <translation type="obsolete">Brazilska portugalščina</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Template </source>
        <translation type="obsolete">Predloga</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>A4</source>
        <translation type="obsolete">A4</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation type="obsolete">Pisava %1 (ki jo je našel fontconfig) je pokvarjena, zato jo zanemarjam</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Razvojna različica Scribusa</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="obsolete">&amp;Nadaljuj</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
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
        <translation> pik</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>točk</translation>
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
        <translation>pik</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>točk (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetri (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>palcev (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>pic (p)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Datoteka obstaja</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="obsolete">Datoteka z imenom &apos;%1&apos; že obstaja.
Ali jo želite zamenjati z novo?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Zamenjaj</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Colour not found.</source>
        <comment>python error</comment>
        <translation type="obsolete">Ne najdem barve.</translation>
    </message>
    <message>
        <source>Colour not found in document.</source>
        <comment>python error</comment>
        <translation type="obsolete">Ne najdem barve v dokumentu.</translation>
    </message>
    <message>
        <source>Colour not found in default colors.</source>
        <comment>python error</comment>
        <translation type="obsolete">Ne najdem barve med privzetimi barvami.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Dokumenta ni mogoče odpreti.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Dokumenta ni mogoče shraniti.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argument mora biti ime predmeta strani, ali instanca PyCObject </translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Lastnosti ni mogoče najti</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Otroka ni mogoče najti</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Neuspešno pretvarjanje vrste rezultata &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Vrsta lastnosti &apos;%1&apos; ni podprta</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Neuspešno pretvarjanje &apos;%1&apos; v vrsto lastnosti &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Vrsti se ujemata, a nastavljanje lastnosti ni bilo uspešno.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Neuspešno shranjevanje slike</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Predmet z danim imenom že obstaja.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Spisek točk mora vsebovati vsaj dve točki (štiri vrednosti).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Spisek točk mora vsebovati sodo število vrednosti.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Spisek točk mora vsebovati vsaj tri točke (šest vrednosti).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Spisek točk mora vsebovati vsaj štiri točke (osem vrednosti).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Spisek točk mora vsebovati mnogokratnik šestih vrednosti.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Predmeta ni mogoče najti.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Sloga ni mogoče najti.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Sloga nebesedilnemu okviru ni mogoče nastaviti.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Neuspešno shranjevanje EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Številka strani je izven obsega.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument ni seznam: mora biti seznam decimalnih (float) vrednosti.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument vsebuje ne-številčne vrednosti: mora biti spisek decimalnih (float) vrednosti.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument ne vsebuje številčnih vrednosti: mora biti spisek decimalnih (float) vrednosti.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Višina vrstice je izven obsega, mora biti 0 &lt;= širina &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Senca vrstice je izven obsega, mora biti 0 &lt;= senca &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Senca polnila je izven obsega, mora biti 0 &lt;= senca &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Sloga črte ni mogoče najti.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Predmet ni povezan besedilni okvir, ni mogoče razvezati.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Predmet je zadnji okvir v nizu, ni ga mogoče razvezati. Razvežite prejšnji okvir.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Samo v besedilnih okvirih lahko preverite prelivanje besedila</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>hrvaški</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>portugalski</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>portugalski (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus se je sesul</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus se je sesul zaradi napake #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Glavna stran</translation>
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
        <translation>Izhodne datoteke %1 ni mogoče odpreti</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Izhodni tok ni zapisljiv</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Neuspešno preverjanje nastavitev: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Neuspešno odpiranje vhodne datoteke %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Neuspešno branje nastavitev XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (vrstica %2, znak %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Neuspešno branje nastavitev XML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>ničelno korensko vozlišče</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>neveljavna &lt;pdfVersion&gt;</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>najdenih %1 &lt;%2&gt; vozlišč, potrebno je le eno.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>nepričakovano ničelno vozlišče &lt;%2&gt; (null)</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>vozlišče &lt;%1&gt; ni element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementu &lt;%1&gt; manjka lastnost &apos;value&apos; (vrednost)</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>vrednost elementa &lt;%1&gt; mora biti &apos;true&apos; ali &apos;false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementu &lt;lpiSettingsEntry&gt; manjka lastnost &apos;name&apos; (ime)</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Knjižnica Freetype2 ni na voljo</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Pisava %1 je okvarjena, zato ne bo vdelana</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Pisava %1 je okvarjena (branje toka), zato ne bo vdelana</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Pisava %1 je okvarjena (FreeType2), zato bo izpuščena</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Pisava %1 je okvarjena (brez vrste), zato bo izpuščena</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Pisava %1 ima pokvarjen znak %2 (koda znaka %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Pisava %1 je okvarjena in bo izpuščena</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Pisava %1 ni berljiva, zato ne bo vdelana</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Neuspešno nalaganje pisave %1 - vrsta pisave neznana</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Pisava %1 naložena iz %2 (%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Pisava %1(%2) je dvojnik pisave %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Nalaganje pisave %1 (najdena s fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Neuspešno nalaganje pisave - freetype2 ne najde datoteke s pisavo</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Pisava %1 je okvarjena (FreeType), zato bo izpuščena</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Pisava %1  ima neveljaven znak %2 (koda znaka %3), zato bo izpuščena</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>izluščenje vrste %1 iz pisave %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>Glava memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabela &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>Tabela memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>Zamik memcpy: %1 %2 %3</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Uporabljate razvojno različico Scribus 1.3.x. Trenutni dokument je bil izvorno pripravljen s Scribus 1.2.2 ali starejšim. Če ga sedaj shranite ga v Scribusu 1.2.2 ne boste več mogli odpreti, razen če izberete Datoteka-&gt;Shrani kot. Ali ste prepričani, da želite nadaljevati s to operacijo?</translation>
    </message>
    <message>
        <source>Invalid argument: </source>
        <translation type="obsolete">Neveljaven argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation type="obsolete">Datoteka %1 ne obstaja. Končujem.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="obsolete">Uporaba: scribus [možnosti ... ] [datoteka]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="obsolete">Možnosti:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="obsolete">Izpiše pomoč (to sporočilo) in konča</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language</source>
        <translation type="obsolete">Uporabi xx kot bližnjico za jezik</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation type="obsolete">Izpiše seznam trenutno nameščenih jezikov vmesnika</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="obsolete">Izpisuj informacije v konzolo med nalaganjem pisav</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation type="obsolete">Ne kaži pozdravnega okna med nalaganjem</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation type="obsolete">Izpiše različico in konča</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="obsolete">Nameščeni so naslednji jeziki Scribusovega vmesnika:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation type="obsolete">Za izbiro drugega jezika:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="obsolete">scribus -l xx ali scribus --lang xx, kjer xx nadomestite s kodo jezika.</translation>
    </message>
    <message>
        <source>Scribus Version </source>
        <translation type="obsolete">Različica Scribusa</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="obsolete">Scribus, prostokodno namizno založništvo</translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="obsolete">---------------------------------------</translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="obsolete">Domača stran: http://www.scribus.net</translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="obsolete">Dokumentacija: http://docs.scribus.net</translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="obsolete">Wiki: http://wiki.scribus.net</translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="obsolete">Težave: http://bugs.scribus.net</translation>
    </message>
    <message>
        <source> cm</source>
        <translation>cm</translation>
    </message>
    <message>
        <source> c</source>
        <translation>c</translation>
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
        <source>Millimeters (mm)</source>
        <translation>milimetri (mm)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>centimetri (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>stran</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">Predogled &amp;pisav...</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Predloga dokumenta: </translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Barve ni mogoče najti.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Barve v dokumentu ni mogoče najti.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Barve med privzetimi barvami ni mogoče najti.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Manj kot dveh predmetov ni mogoče združiti</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Manj kot dveh predmetov ni mogoče združiti</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Za združevanje je potrebna izbira ali seznam predmetov </translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Nebesedilnemu okviru ni mogoče nastaviti zaznamka</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Podatkov iz nebesedilnega okvirja ni mogoče pridobiti</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Ime datoteke ne sme biti prazno.</translation>
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

Ta modul je vmesnik Python za Scribus. Zagotavlja funkcije
za nadzor scribusa in obdelovanje predmetov na platnu. Vsaka
funkcija je dokumentirana posamično spodaj.

Nekatere stvari so skupne za večji del vmesnika.

Večina funkcij deluje na okvirih. Okvire določa njihovo ime,
niz - niso resnični predmeti Python. Veliko funkcij ima dodaten
parameter (ki ni ključna beseda), ime okvira.
Pri večini funkcij so pogoste tudi številne izjeme. Te trenutno še
niso dokumentirane v docstring-u za vsako funkcijo.
- Številne funkcije bodo vrnile NoDocOpenError, če jih skušate uporabiti
brez dokumenta, na katerem bi delovale.
- Če ne posredujete ime okvira funkciji, ki ga zahteva,
bo funkcija uporabila trenutno izbrani okvir, če sploh, ali
vrnila NoValidObjectError, če ne najde ničesar za obdelavo.
- Mnoge funkcije bodo vrnile WrongFrameTypeError, če jih skušate uporabiti
na vrsti okvira, ki jim ni znan. Če na primer nastavite barvo besedila
na okviru slike, to ni smiselno, zato bo posledično vrnjena ta izjema.
- Napake, ki izvirajo iz klicev na spodnji API Python, bodo
posredovane nespremenjene. Zanje seznam izjem, ki jih
vrne vsaka funkcija, ki je na voljo tu s docstring-om, ni popoln.

Podrobnosti o izjemah, ki jih lahko vrne vsaka od funkcij, si oglejte
v dokumentaciji funkcije, čeprav ta seznam kot pri večini kode Python
ni povsem izčrpen zaradi izjem klicanih funkcij.</translation>
    </message>
    <message>
        <source>Short Words Manual</source>
        <translation type="obsolete">Priročnik za kratke besede</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Dokumenti z besedilom OpenDocument</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopija #%1 od </translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Črna</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cijanska</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Rumena</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Barvni krog</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Predogled pisav</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Moj vtičnik</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nova iz predloge</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Izvozi kot sliko</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>Uvoznik PS/EPS </translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Shrani kot predlogo</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skripterka</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Kratke besede</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>Izvoz SVG</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>Uvoz SVG</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Uvoznik OpenOffice.org Draw </translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus se je sesul zaradi naslednje napake: %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Ustvarjanje medpomnilnika pisav</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Najdene nove pisave, preverjanje je v teku ...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Najdene spremenjene pisave, preverjanje je v teku ...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Branje medpomnilnika pisav</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Pisanje posodobljenega medpomnilnika pisav</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Iskanje pisav</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Uporabljate razvojno različico Scribus 1.3.x. Trenutni dokument je bil izvorno pripravljen s Scribusom 1.2.2 ali starejšim. Če ga sedaj shranite, ga s Scribusom 1.2.2 ne boste več mogli odpreti, razen če izberete Datoteka-&gt;Shrani kot. Ste prepričani, da želite nadaljevati s to operacijo?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Spremembe v vašem dokumentu niso bile shranjene, zahtevali pa ste, da se jih povrne v staro stanje. Želite nadaljevati?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Datoteka z imenom &apos;%1&apos; že obstaja. &lt;br/&gt;Jo želite prepisati z datoteko, ki jo shranjujete?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder je večji, kakor je dovoljeno.</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Podpora za star zapis .sla</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>nemški (trad.)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Izvažanje datoteke PostScript</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Tiskanje datoteke</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Ali poskušati uvoziti več strani, kot jih je trenutno na voljo v dokumentu, šteto od aktivne strani.&lt;/p&gt;Izberite eno od naslednjih možnosti:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Ustvari&lt;/b&gt; manjkajoče strani&lt;/li&gt;&lt;li&gt;&lt;b&gt;Uvozi&lt;/b&gt; strani do zadnje strani&lt;/li&gt;&lt;li&gt;&lt;b&gt;Prekliči&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Ustvari</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>Uvo&amp;zi</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>tajski</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Generator črtne kode</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Dokumenti Word</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Dokumenti Palm PDB</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_data</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Uvoz PDB</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Datoteke %1 ni mogoče odpreti</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="obsolete">Ta datoteka ni prepoznavna kot PDB dokument. Prosim prijavite to kot hrošča, če ste prepričani, da je datoteka prava.</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>luksemburški</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>japonski</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>arabski</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation>estonski</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Podano ime glavne strani se ne ujema z nobenim obstoječim.</translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>islandski</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 je morda okvarjena : manjkajo oznake ločljivosti</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Te datoteke ni mogoče prepoznati kot dokumenta PDB. Prosimo, sporočite tega hrošča, če ste prepričani vanj.</translation>
    </message>
    <message>
        <source>Breton</source>
        <translation>bretonski</translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation>angleški (ZDA)</translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation>angleški (Avstralija)</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation>%1 je morda okvarjena : manjkajoče ali napačne oznake ločljivosti</translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation>nemški (Švica)</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>P&amp;onovi</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Po&amp;čisti</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Izberi vse</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Povečaj</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Obnovi</translation>
    </message>
    <message>
        <source>System Menu</source>
        <translation>Sistemski meni</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Odsenči</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Premakni</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Velikost</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Zapri</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Pomanjšaj</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ohrani na &amp;vrhu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Se&amp;nči</translation>
    </message>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>O&amp;dsenči</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Poman&amp;jšaj</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Pove&amp;čaj</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Obnovi navzdol</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> točk</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med levim robom strani in levim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med spodnjim robom strani in spodnjim mejnim vodilom</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Nastavitev dokumenta</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Razdalja med desnim robom strani in desnim mejnim vodilom.
Če ste izbrali nasproti postavljene strani, bo ta prostor uporabljen za pravilno zlaganje</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Vodila robov</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Naj bo prva stran leva</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Omogoči enojno ali mnogotero postavitev</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Razdalja med gornjim robom strani in zgornjim mejnim vodilom</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Desno:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Najprej &amp;leva stran</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Znotraj:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">Z&amp;unaj:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Velikost:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Usmerjenost:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Pokončno</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Ležeče</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">širina:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">višina:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Številka &amp;prve strani:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Velikost:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Pravna</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Pismo</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloidna</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Usmerjenost:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Širina:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Višina:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Enota:</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation type="obsolete">Postavitev</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Samodejno shranjevanje</translation>
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
        <source>Guides</source>
        <translation>Vodila</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Prikaz strani</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Prikazuj &amp;nenatisljivo območje v barvi meje</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Pokaži slike</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Kaži verige besedil</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Kaži okvire</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="obsolete">Prazen prostor</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Prikaz</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografija</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Deljenje</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Pisave</translation>
    </message>
    <message>
        <source>Doc-Checker</source>
        <translation type="obsolete">Pregled dokumenta</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Izvoz PDF</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Upravljanje barv</translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
        <translation type="obsolete">Vključi ali izključi povezane okvirje</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Vključi ali izključi prikazovanje okvirov</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Vključi ali izključi prikazovanje slik</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Barva papirja</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Označi prostor robu z barvo robu</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Možnosti</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="obsolete">Uveljavi nastavitev velikosti na vseh straneh</translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="obsolete">Uveljavi nastavitev robov na vseh straneh</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Podatki o dokumentu</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Pokaži ukazne znake v besedilu</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Vodila glede na stran</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Najmanj praznega prostora</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Presledki med stranmi</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vodoravno:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Navpično:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Končno preverjanje</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Lastnosti predmeta dokumenta</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Kazalo vsebine in seznami</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Prikazuj ne-natisljive znake, kot je konec odstavka, v besedilnih okvirih</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Prilagajanje barv</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Vključi ali izključi prikaz povezanih okvirov.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Uveljavi nastavitev velikosti na vseh straneh</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Odseki</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Uveljavi spremembo velikosti strani na vseh obstoječih straneh v dokumentu</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Skripte Python (*.py);; Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Poženi kot razširitveno skripto</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Upravitelj slogov</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation>Stolpec 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation>K&amp;loniraj</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>V &amp;redu</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>U&amp;porabi</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>&amp;Prekliči</translation>
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
        <translation>Lastnosti</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>Črte</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Slog trenutnega odstavka</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Nastavitve sloga</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Barva polnila besedila</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Nasičenost barve polnila besedila</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Nastavitve barve polnila</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Barva poteze besedila</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Nasičenost barve poteze besedila</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Nastavitve barve poteze</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source> pt</source>
        <translation>točk</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Pisava izbranega besedila</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Sprememba merila širine črk</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Nastavitve pisave</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Sprememba merila višine črk</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Spodsekavanje:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ročno spodsekavanje</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Nastavitve znakov</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Ročno sledenje</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Shrani stran kot &amp;SVG ...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Uzvozi datoteke SVG</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Izvozi trenutno stran v datoteko SVG.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Uvozi &amp;SVG ...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Uzvozi datoteke SVG</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Uvozi večino datotek SVG v trenutni dokument in
pretvori vektorske podatke v Scribusove predmete.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>SVG (Scalable Vector Graphics)</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>Datoteka SVG vsebuje nepodprte možnosti</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Kratke besede</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Uveljavi neprelomljiv presledek na:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Izbrani okviri</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Aktivna &amp;stran</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Vsi predmeti</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Obdelani le izbrani okviri.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Obdelana le aktivna stran.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Obdelani vsi predmeti dokumenta.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Uporabniške nastavitve</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Sistemske nastavitve</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Ponastavi</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Shrani uporabniške nastavitve</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Ponovno naloži sistemske nastavitve in izbriši uporabniške nastavitve</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Spremeni nastavitve po meri. Če jih shranite, bodo prepisane čez sistemske nastavitve</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Kratke besede</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Uporabniške nastavitve že obstajajo. Jih resnično želite prepisati?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Datoteke %1 ni mogoče zapisati.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Uporabniške nastavitve shranjene</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Sistemske nastavitve ponovno naložene</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Datoteke %1 ni mogoče odpreti</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Shrani kot &amp;predlogo ...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Shrani dokument kot predlogo</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Shrani dokument kot predlogo. Dober način za poenostavitev začetka dela z dokumenti, ki imajo stalno enak videz</translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Ne najdem ciljne datoteke %1: %2</translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Ne morem odpreti ciljne datoteke %1: %2</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Stalen</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Uvozni</translation>
    </message>
    <message>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Izvozni</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Dejanje</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Odpri/Shrani/Uvozi/Izvozi</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznano</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Zgoraj</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Desno</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Spodaj</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Levo</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Dovoli sidranje ...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodoravno</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Smer plavanja ...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Tiskanje ...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopija #%1 izvirnika </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>p</source>
        <translation type="obsolete">pik</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">pik</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">pal</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">pal</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">točk</translation>
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
        <source>All</source>
        <translation type="obsolete">Vse</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Izreži</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nova</translation>
    </message>
    <message>
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopiraj</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Datoteka</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Pisava</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Zakleni</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Odpri</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Končaj</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Shrani</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Velikost</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Končujem</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Razveljavi</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Poravnaj desno</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Zapri trenuten dokument</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Prečrtano</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Poravnaj na sredino</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">Ur&amp;edi</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">Datoteka</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">Pomoč</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">Predmet</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">Stran</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">Pogled</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Slogi...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Razporedi/Poravnaj...</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Počisti</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Zapri</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Barva</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Češčina</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Nizozemščina</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Grščina</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Združi</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irščina</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Spusti</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Prilepi</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Dvigni</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Pripravljen</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Senči</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Slog</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Orodja</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Pošlji na dno</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italijanščina</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Preskakuj do mreže</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Vključujem dodatke</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Razdruži</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Pametno deljenje besed</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS datoteke (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Orodja</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Zbriši...</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Pomanjšane velike črke</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Večkrat podvoji</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Prikaži paleto strani</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Barve...</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Sličice</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Iščem pisave</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Podatki o dokumentu...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Premakni...</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Skrij okvirje</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Prekliči</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Danščina</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Zbriši</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Nedeljiv presledek</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Skrij slike</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">MadĹžarščina</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Francoščina</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Nemščina</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Zakleni/Odkleni</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Preobrni</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Običajno</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Na vašem sistemu ni Postscriptnih pisav</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Prikaži sloje</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Poljščina</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Nalagam:</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Odkleni vse</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Zakleni vse</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slovaščina</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y:</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Odkleni</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Odpri...</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Prikaži mrežo</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Obroba</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovenščina</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Lastnosti</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Podvoji</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Skrij meje</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Shrani kot...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Dodaj sliko...</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">Dodatno</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Besedilo pripni na pot</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Nekateri predmeti so zaklenjeni.</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Vstavi številko strani</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Skrij osnovno mrežo</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Prinesi na vrh</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Prilagajam barve</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Upravljaj vodila...</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrajinščina</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Odložišče</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Berem odložišče</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Priprava dokumenta</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Java skripti...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Na velikost okna</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Natisni...</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Prikaži osnovno mrežo</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">O QT-ju</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Dodaj besedilo/sliko</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalonščina</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Shrani kot</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Uveljavi predlogo...</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Podpisano</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Odpri dokument</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Nadpisano</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Vstavi...</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Ruščina</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Tiskanje ni uspelo!</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Shrani trenutno odprt dokument kot PDF</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Opravi deljenje besed</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Španščina</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus je razpadel zaradi napake #%1</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Natisni trenutno odprt dokument</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Dodaj besedilo...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Tiskam...</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Podčrtano</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Švedščina</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Shrani trenuten dokument</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Nalagam...</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Besedilne datoteke (*.txt);;Vse datoteke(*)</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Izberi mapo</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Ustvari nov dokument</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Pomoč na spletu</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Izberi vse</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugalščina</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Upravljaj s slikami</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Angleščina</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Obrisano</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Scribus je razpadel</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Predloge...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Shranjujem...</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finščina</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Nastavitve branja</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">NAstavljam bližnjice</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Usodna napaka</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Poravnaj levo</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">Okna</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Izberi novo pisavo</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">O Scribusu</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Vključujem deljenje</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Litvanščina</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Priročnik za Scribus</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Pisave...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">Na&amp;stavitve</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Vse podprte oblike</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Ne morem pisati v datoteko:\n
%1</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Manjkajo sledeči programi:</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Upravljanje barv...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Odpri...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">Odpri &amp;nedavno</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zapri</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Shrani</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Shrani &amp;kot...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">&amp;Povrni do shranjene</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Zberi za &amp;izvoz...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Dodaj besedilo/sliko...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Dodaj &amp;besedilo...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Uvozi</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Shrani &amp;besedilo...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Shrani stran kot &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Shrani kot &amp;PDF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Izvoz</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">&amp;Nastavitve dokumenta...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Tiskaj...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Končaj</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Razveljavi</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Počisti</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Izberi &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">Po&amp;išči in zamenjaj...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">&amp;Barve...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">&amp;Slogi odstavka...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Slogi &amp;črt...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Predloge...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Java skripti...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Večkratno podvojevanje</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Zbriši</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Združi</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Razdruži</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Zakleni</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Pošlji na &amp;dno</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Prinesi na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Dvigni</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Razporedi/&amp;Poravnaj...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Oblika</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">Besedilo &amp;pripni na pot</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">Besedilo &amp;snemi s poti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Sestavi poligone</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Razdeli &amp;poligone</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">Pr&amp;etvori v črte</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Vstavi...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Zbriši...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Premakni...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Uveljavi predlogo...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">&amp;Na velikost okna</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">&amp;Sličice</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Kaži &amp;mrežo</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">&amp;Preskakuj do vodil</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Kaži &amp;osnovno mrežo</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Lastnosti</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Obroba</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Odložišče</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Sloji</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">P&amp;aleta strani</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Zaznamki</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">&amp;Upravljaj s slikami</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">Opravi &amp;deljenje besed</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">&amp;Namigi orodja</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">Orodja za &amp;PDF</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Namigi orodja</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Pisave...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">&amp;Deljenje besed...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">&amp;Bližnjice tipkovnice...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;O Scribusu</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">O &amp;QT-ju</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">Pr&amp;iročnik za Scribusa...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">S&amp;log</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Levo</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Sredinsko</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Desno</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Vsiljeno</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Drugo...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Kaskadno</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Tlakovano</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Barva</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">Pre&amp;obrni</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Vstavi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">&amp;Pisava</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Velikost</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Učinki</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">P&amp;oravnava</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Senca</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabulatorji...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Od&amp;kleni</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Pokaži &amp;slike</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Pokaži &amp;meje</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Pokaži &amp;okvirje</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Prikaži &amp;vodila</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript: Ne morete uporabiti EPS slik</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Uvozi &amp;stran(i)...</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Preskakuj do mreže</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">&amp;Nastavitve...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Uvažam strani...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Uvozi stran(i)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Poskušate uvoziti več strani, kot jih je na voljo v dokumentu od trenutno aktivne dalje.&lt;/p&gt;Izberite eno od možnosti:&lt;br&gt;
&lt;ul&gt;&lt;li&gt;&lt;b&gt;Ustvari&lt;/b&gt; manjkajoče strani&lt;/li&gt;&lt;li&gt;&lt;b&gt;Uvozi&lt;/&gt; strani do zadnje strani&lt;/li&gt;&lt;/li&gt;&lt;b&gt;Prekliči&lt;/b&gt;&lt;/li&gt;/&lt;ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Ustvari</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Uvozi</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Uvoz končan</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Nič za uvoziti</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Iščem ICC profile</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Upravljaj &amp;vodila...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Velikost:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Senca:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">&amp;Podatki o dokumentu...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Prekliči brisanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Prekliči premikanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Prekliči spreminjanje predmeta</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Uredi obliko</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Datoteka %1 ni primerne oblike za Scribus</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikaansko</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Vključen je sistem za pisave</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Portugalščina (BR)</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Uredi</translation>
    </message>
    <message>
        <source>There are no Postscript fonts on your system</source>
        <translation type="obsolete">Na sistemu ni Postscriptnih pisav</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>PDF-1.3</source>
        <translation type="obsolete">PDF-1.3</translation>
    </message>
    <message>
        <source>PDF-1.4</source>
        <translation type="obsolete">PDF-1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">PDF/X-3</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation type="obsolete">Uvozi stare nastavitve?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation type="obsolete">Scribus je zaznal nastavitvene datoteke različice 1.2.
Ali jih želite uporabiti v novi različici?</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="obsolete">Dodaj sliko...</translation>
    </message>
    <message>
        <source>Import Page(s)...</source>
        <translation type="obsolete">Uvozi stran(i)...</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="obsolete">&amp;Obnovi</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="obsolete">Način urejanja &amp;predmetov</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="obsolete">%1 točk</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation type="obsolete">&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation type="obsolete">&amp;Običajno</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="obsolete">&amp;Podčrtano</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation type="obsolete">&amp;Prečrtano</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation type="obsolete">Pomanjšane &amp;velike črke</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation type="obsolete">&amp;Nadpisano</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation type="obsolete">Po&amp;dpisano</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation type="obsolete">Je &amp;zaklenjeno</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="obsolete">&amp;Uredi obliko...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="obsolete">&amp;Kopiraj...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="obsolete">Prilagodi &amp;oknu</translation>
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
        <source>Show &amp;Text Chain</source>
        <translation type="obsolete">Kaži &amp;verige besedil</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation type="obsolete">&amp;Meritve</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation type="obsolete">&amp;Zgodovina dejanj</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation type="obsolete">Pre&amp;veri pred letom</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Vstavi posebej</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation type="obsolete">Vstavi pametno deljenje</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation type="obsolete">Vstavi nedeljiv presledek</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation type="obsolete">Preklopi palete</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation type="obsolete">Preklopi vodila</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumenti (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript datoteke (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG slike (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG slike (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>File %1 
is not in an acceptable format</source>
        <translation type="obsolete">Datoteka %1
ni v nobeni sprejemljivi obliki</translation>
    </message>
    <message>
        <source>Some Fonts used by this Document have been substituted:</source>
        <translation type="obsolete">Nekatere pisave uporabljene v tem dokumentu so bile zamenjane:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="obsolete">je bila zamenjana z:</translation>
    </message>
    <message>
        <source>Some ICC-Profiles used by this Document are not installed:</source>
        <translation type="obsolete">Nekateri ICC profili uporabljeni v tem dokumentu niso nameščeni:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="obsolete">(pretvorjen)</translation>
    </message>
    <message>
        <source>Cannot write the File: 
%1</source>
        <translation type="obsolete">Ne morem pisati v datoteko:
%1</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="obsolete">Zaznane napake.
Poskusite uporabiti &quot;Pregled pred poletom&quot;, da jih odpravite</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Opusti</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Zanemari</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="obsolete">Manjkajo sledeči programi:</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS datoteke (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="obsolete">Ne morem pisati v datoteko:
%1</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Zaznane napake.
Poskusite uporabiti &quot;Pregled pred poletom&quot;, da jih odpravite</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="obsolete">&amp;Zakleni vse</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">&amp;Odkleni vse</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bolgarščina</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Program</translation>
    </message>
    <message>
        <source>is already running!</source>
        <translation type="obsolete">že teče!</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Podatki</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">manjka!</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Vključujem bližnjice tipkovnice</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="obsolete">Vključujem urejevalnik zgodbe</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">Berem ICC profile</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="obsolete">Vključujem deljenje besed</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">Na sistemu ne najdem pisav.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Končujem.</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="obsolete">Nastavitve predogleda</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Stopnja</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Pošlji na &amp;sloj</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Možnosti &amp;PDF</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="obsolete">&amp;Pretvori v</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="obsolete">&amp;Vstavi</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="obsolete">Narekovaj</translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="obsolete">Prostor</translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="obsolete">Nekatere pisave, uporabljene v tem dokumntu, so bile nadomeščene:</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="obsolete">Nekateri ICC profili, uporabljeni v dokumentu, niso nameščeni:</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Shrani kot</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="obsolete">Ne morete izrezati predmetov v uporabi</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="obsolete">Predmet %1 uporablja urejevalnik zgodbe. Izrezovanje je bilo preklicano</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="obsolete">Ghostscript: ne morete uporabiti EPS slik v predogledu tiskanja</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="obsolete">Hrvaščina</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="obsolete">Izbrana barva ni uvrščena v nabor barv dokumenta. Izberite ji ime.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="obsolete">Ne najdem barve</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="obsolete">Izbrano ime že obstaja. Izberite drugo ime za to barvo.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="obsolete">&amp;Raven</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="obsolete">Pošlji na sloj</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="obsolete">Nastavit&amp;ve predogleda</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="obsolete">Datoteka %1 ni v sprejemljivi obliki</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
        <translation type="obsolete">Scribus je našel napake.
Uporabite Končno preverjanje da jih odpravite</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="obsolete">&amp;Ustavi</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="obsolete">&amp;Zanemari</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Ime:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="obsolete">Nova glavna stran</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="obsolete">Scribus je našel napake.
Uporabite Končno preverjanje da jih odpravite.</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Scribus je našel napake.
Uporabite Končno preverjanje da jih odpravite</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="obsolete">-Stran%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Nekateri predmeti so zaklenjeni.</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="obsolete">Program %1 že teče!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="obsolete">Program %1 manjka!</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="obsolete">Pretvori v glavno stran</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Primer</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Barva</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation>Barve dokumenta</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nova plast</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Običajen</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Ozadje</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Resnično želite izbrisati vse besedilo?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Ne morete izbrisati predmetov v uporabi</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Predmet %1 uporablja urejevalnik zgodbe. Izrezovanje je bilo preklicano</translation>
    </message>
    <message>
        <source>An error occurred while opening icc profiles, color management is not enabled.</source>
        <translation type="obsolete">Pri odpiranju profilov icc je prišlo do napake, zato upravljanje barv ni omogočeno.</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Pri odpiranju profilov ICC je prišlo do napake, zato upravljanje barv ni omogočeno.</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializacija vtičnikov</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Inicializacija tipk za bližnjice</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Branje nastavitev</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Inicializacija urejevalnika zgodb</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Branje profilov ICC</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Inicializacija deljenja besed</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Branje skicirke</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Nastavljanje bližnjic</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Uredi</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Iskanje pisav</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>V sistemu ni mogoče najti pisav.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Sledi izhod.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Usodna napaka</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Vključen je sistem pisav</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Odpri &amp;nedavne</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Izvozi</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>S&amp;log</translation>
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
        <translation>&amp;Senca</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Pisava</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Učinki</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Predmet</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Nastavitve predogleda</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Raven</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Pošlji na p&amp;last</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Možnosti &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Oblika</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Pretvori v</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Vstavi</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Narekovaj</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Presledek</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Stran</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Pogled</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>D&amp;odatki</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Okna</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Pomoč</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>P&amp;oravnava</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Pripravljen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Uvažanje strani ...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Uvozi stran(i)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Uvoz končan</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nič ni bilo mogoče uvoziti</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Datoteka %1 ni v sprejemljivi obliki</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Nalaganje ...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Nekateri profili ICC, uporabljeni v dokumentu, niso nameščeni:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>je bila zamenjana z:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(pretvorjen)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Vse podprte oblike</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>V datoteko ni mogoče pisati:
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Shranjevanje poteka ...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus je našel napake. Uporabite Končno preverjanje, da jih odpravite</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Prezri</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Ustavi</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Tiskanje v teku ...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Tiskanje ni uspelo!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Ne morete izrezati predmetov v uporabi</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Predmet %1 uporablja urejevalnik zgodb. Izrezovanje je bilo preklicano</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O QT-ju</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Priročnik za Scribus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Besedilne datoteke (*.txt);;Vse datoteke(*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Navadno</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ime:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Pretvori v glavno stran</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="obsolete">Nova glavna stran</translation>
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
        <translation>&amp;Senca:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Manjkajo naslednji programi:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: slik EPS ne morete uporabiti v predogledu tiskanja</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus je našel napake.
Uporabite Končno preverjanje, da jih odpravite.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>Datoteke EPS (*.eps);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus je našel napake.
Uporabite Končno preverjanje, da jih odpravite</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Stran%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Nekateri predmeti so zaklenjeni.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Zakleni vse</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Odkleni vse</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Podatki</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Program %1 že teče!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Program %1 manjka!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Izbrana barva ni uvrščena v nabor barv dokumenta. Izberite ji ime.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Barve ni mogoče najti</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Izbrano ime že obstaja. Izberite drugo ime za to barvo.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Raven</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Pošlji na plast</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Nastavit&amp;ve predogleda</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Orodja</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Presledki in prelomi</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatura</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Nova glavna stran %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Število kopij: %1
Vodoravni zamik: %2
Navpični zamik: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: Slik EPS ali predogleda tiskanja Postscript ne morete uporabljati</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript manjka: Predogled tiskanja Postscript ni na voljo</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Resnično želite zamenjati obstoječo sliko?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Vsebina</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;tura</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Neveljaven argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Datoteka %1 ne obstaja. Sledi izhod.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Uporaba: scribus [možnosti ... ] [datoteka]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Možnosti:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Izpiše pomoč (to sporočilo) in konča</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Uporabi xx kot bližnjico za jezik, npr. &apos;en&apos; ali &apos;de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Izpiše seznam trenutno nameščenih jezikov vmesnika</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Pokaži informacije v konzoli med nalaganjem pisav</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Ne pokaži pozdravnega okna med nalaganjem</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Izpiše različico in konča</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Uporabi vrstni red gumbov z leve proti desni (npr. Prekliči/Ne/Da namesto Da/Ne/Prekliči)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>imedatoteke</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Uporabi ime datoteke kot pot do nastavitev uporabnika</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Nameščeni so naslednji jeziki Scribusovega vmesnika:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Za izbiro drugega jezika od privzetega:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx ali scribus --lang xx, kjer xx nadomestite s kodo jezika.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus različice</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, prostokodno namizno založništvo</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Domača stran</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentacija</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Napake</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>Pokaži okno konzole</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source>p</source>
        <translation type="obsolete">pik</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">pal</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">točk</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Vse</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Plast</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopiraj sem</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Premakni sem</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Slika</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Datoteka: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Izvorna PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Dejanska PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Povezano besedilo</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Besedilni okvir</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Besedilo po poti</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavki: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Besede: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaki: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Natisni: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Vključeno</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Izključeno</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Podatki</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">&amp;Slika je vidna</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Osveži sliko</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Uredi sliko</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Okvir &amp;prilagodi sliki</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Uredi besedilo...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Je PDF &amp;beležka</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">je PDF &amp;opomba</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Lastnosti opombe</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Lastnosti &amp;polja</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Možnosti &amp;PDF</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Uredi besedilo...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Zakleni</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Od&amp;kleni</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Zakleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Odkleni &amp;velikost predmeta</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Pošlji na &amp;odložišče</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Pošlji na p&amp;last</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Vstavi naključno besedilo</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Združi</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">&amp;Razdruži</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;Raven</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Pošlji na &amp;dno</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Prinesi na &amp;vrh</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Spusti</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Dvigni</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">&amp;Okvir slike</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">&amp;Mnogokotnik</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Obrobe</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Okvir besedila</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">&amp;Bezier krivulja</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Pretvori v</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">&amp;Počisti vsebino</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ali zares želite počistiti vso besedilo?</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopija</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Nastavitve predogleda</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Povezovanje besedilnih okvirov</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="obsolete">Ne morete izrezati predmetov v uporabi</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="obsolete">Predmet %1 uporablja urejevalnik zgodbe. Izrezovanje je bilo preklicano</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Poskušate povezati s zapolnjenim ali samim okvirom.</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="obsolete">Ali zares želite izbirasti vse besedilo?</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Ne morete pretvoriti predmetov v uporabi</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Predmet %1 uporablja urejevalnik zgodb. Pretvarjanje v črte je bilo preklicano</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Strani %1 do %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Barvni prostor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Neznano</translation>
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
        <translation>Sivinsko</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Vsebina</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>has been changed since the last save.</source>
        <translation>je bil od zadnjega shranjevanja še spremenjen.</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">Vseeno &amp;zapusti</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">Vseeno &amp;zapri</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Shrani sedaj</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Opusti</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Vgrajena podpora za skripte Python.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skripterka</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">&amp;Skribusove skripte</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Poženi skripto...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Nedavne skripte</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;O skriptah...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">Skripta</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Odpri</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python Skripte (*.py);; Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation>Napaka skripte</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Če ste pognali uradno skipto, prosim, sporočite napako na &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Tudi to sporočilo je v odložišču. Uporabite Ctrl+V, da ga prilepite v slednika hroščev.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Skrij k&amp;onzolo</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">O skriptah</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Pripetila se je notranja napaka pri preverjanju vnesenega ukaza. Podrobnosti so izpisane na izhod standardne napake (stderr).</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Preuči skripto</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python skripte (*.py)</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Neuspešno nastavljanje vtičnika Python. Podrobnosti so izpisane na izhod standardne napake (stderr).</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Skripte Python (*.py);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentacija za:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Skripta</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation>ne vključuje nobenih docstring!</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
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
        <translation type="obsolete">Napredne možnosti</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Nastavitve skripterke</translation>
    </message>
    <message>
        <source>Extension Scripts</source>
        <translation type="obsolete">Razširitvene skripte</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Vključi razširitvene skripte</translation>
    </message>
    <message>
        <source>Startup Script</source>
        <translation type="obsolete">Začetna skripta</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Razširitve</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konzola</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Začetna skripta:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Napake:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Komentarji:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Ključne besede:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Znaki:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Številke:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Nizi znakov:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Izhodiščno besedilo:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Izberi barvo</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Spremeni ...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Poišči začetno skripto</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Pokaži predoglede strani</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Pokaži imena predlog</translation>
    </message>
    <message>
        <source>Show Master Page Names</source>
        <translation type="obsolete">Kaži imena glavnih strani</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Pisava</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Levo</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Brez</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blok</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Desno</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Slog odstavka</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Barva poteze</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Senca poteze</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Poišči in zamenjaj</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Sredina</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Vsiljeno</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Velikost pisave</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Iskanje končano</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Išči:</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Barva polnila</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Senca polnila</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Učinki pisave</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Zamenjaj z:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Cele besede</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignoriraj velikost</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Išči</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Zamenjaj</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Zamenjaj &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Zapri</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Iskanje končano, najdenih %1 zadetkov</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Običajno</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Tu so vse vaše predloge. Da sutvarite novo stran
povlecite predlogo na delovno področje spodaj.</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Dostopne predloge: </translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Najprej leva stran</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Za izbris povlecite strani ali predloge v koš.</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Nasproti ležeče strani</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Strani dokumenta:</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Uredi strani</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Predogled vseh strani v dokumentu.</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="obsolete">Za brisnje povlecite strani ali glavne strani v koš</translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="obsolete">Predogled vseh strani dokumenta</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="obsolete">Tu so vse vaše glavne strani. Za pripravo nove strani povlecite glavno stran v spodnji pogled</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="obsolete">Dostopne glavne strani:</translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="obsolete">Najprej leva stran</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Available Fields</source>
        <translation>Polja na voljo</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Izbrana polja</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation>Izberite polja</translation>
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
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Shade</source>
        <translation>Senči</translation>
    </message>
    <message>
        <source>Other...</source>
        <translation>Drugo ...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Senca:</translation>
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
        <translation>X-odmik</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-odmik</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Kratke &amp;besede ...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Kratke besede</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words.</source>
        <translation type="obsolete">Poseben dodatek za dodajanje nedeljivih presledkov pred ali za t.i. kratkimi besedami.</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Poseben dodatek za dodajanje nedeljivih presledkov pred ali za t.i. kratke besede. Na voljo je za naslednje jezike: </translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Uredi sloge ...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Brez sloga</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>No</source>
        <translation type="obsolete">Ne</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Da</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.sla.gz *.scd *.scd.gz);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenti (*.sla *.scd);;Vse datoteke (*)</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation>Uredi sloge</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nov slog</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopija od %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Dodaj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Po&amp;dvoji</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Shrani</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Ali zares želite zbrisati ta slog?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Uvozi</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="obsolete">Ali zares želite zbrisati ta slog?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Ali zares želite izgubiti vse spremembe?</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Urejevalnik zgodb</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ali zares želite počistiti vso besedilo?</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Trenuten odstavek:</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Želite shraniti spremembe?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Shrani kot</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Znaki: </translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Besedilne datoteke (*.txt);;Vse datoteke(*)</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Skupaj:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Opozorilo</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Odstavkov: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Besed: </translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Posodobi okvir z besedilom in končaj</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Končaj brez posodabljanja besedila</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Posodobi okvir z besedilom</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Vstavi posebej</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nova</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Osveži besedilo iz okvira</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Shrani v datoteko ...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Naloži iz datoteke ...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Shrani &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Posodobi okvir z besedilom in končaj</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Končaj brez posodabljanja besedila</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Izreži</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiraj</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Prilepi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Po&amp;čisti</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Vstavi posebej...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Posodobi okvir z besedilom</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>Ur&amp;edi</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Izberi &amp;vse</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Uredi sloge ...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="obsolete">Naloži besedilo iz datoteke</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Shrani besedilo v datoteko</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Osveži besedilo iz okvirja</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Najd&amp;i in zamenjaj ...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Predogled &amp;pisav ...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Ozadje ...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>Pisava za &amp;prikaz ...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Na&amp;stavitve</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Poišči in zamenjaj</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Predogled &amp;pisav</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Počisti vse besedilo</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Pametno izbiranje besedila</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Vstavi znak ...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Počisti vse besedilo</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Urejevalnik zgodb - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Ali zares želite izgubiti vse spremembe?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ali zares želite izbrisati vse besedilo?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Vstavi</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Znak</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Narekovaj</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Presledki in prelomi</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatura</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Presledek</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Razpostavitev</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Širina črte</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation>Izbran več kot en predmet</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Besedilo orisa</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Pomanjšane velike črke</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Podpisano</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Nadpisano</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Podčrtano</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation type="obsolete">Prečrtano</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Obroba</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation type="obsolete">Samo podčrtaj besede</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Velike črke</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation type="obsolete">Senca</translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="obsolete">Obroba</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podčrtano besedilo. Za kratek čas pritisnite tipko, da nastavite širino črte in možnosti razmika.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Podrčta samo besede. Za kratek čas pritisnite tipko, da nastavite širino črte in možnosti odstopanja.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Prečrta besedilo. Za kratek čas pritisnite gumb, da nastavite širino črte in možnosti odmika.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation>Orisano. Za kratek čas pritisnite gumb, da spremenite širino poteze orisa.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Osenčeno besedilo. Za kratek čas pritisnite gumb, da vključite razmik odmika.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OO.o Writer Importer Options</source>
        <translation type="obsolete">Možnosti uvažanja iz OO.o Writerja</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Posodobi sloge odstavka</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Ime dokumenta uporabi kot predpono za sloge odstavka</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ne vprašaj več</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OO.o document and
never ask your confirmation again</source>
        <translation type="obsolete">Nastavi ali naj uvoznik vedno pri uvažanju dokumentov 
OO.o uporabi nastavljeno vrednost in ne sprašuje 
več za potrditev</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Nastavli ali naj uvoznik doda ime dokumenta 
pred imena uvoženih slogov odstavka</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Če slog s tem imenom že obstaja lahko nastavite, ali naj se slog v 
Scribusu uredi tako, da ustreza uvoženemu ali naj se ga pusti nedotaknjenega</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Možnosti uvoznika iz OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Naj pri uvozu vedno upoštevam trenutno 
določene možnosti za dokumente OpenOffice.org 
in nikoli več ne zahtevam potrditve</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="obsolete">Pospravi sloge odstavka</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="obsolete">Združi sloge odstavka po lastnostih.
Tako dobite manj slogov, a jih je težje upravljati. 
Uporabite le, če veste da vam po uvozu ne bo treba več urejati besedila.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Prepiši sloge odstavka</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>S tem prepišete obstoječe sloge v trenutno odprtem Scribusovem dokumentu</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Spoji sloge odstavka</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Spajanje slogov glede na lastnosti. Tako dobite manj podobnih slogov in jih obdržite, tudi če so v izvirniku poimenovani drugače.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Ime dokumenta pripne slogom odstavka.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Naj bodo te nastavitve privzete. Pri uvažanju OpenOffice.org 1.x datotek ne sprašuj več.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Na začetek</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Na konec</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ni prikazano</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Kazalo vsebine in seznami</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Kazalo vsebine</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Okvir, v katerega bo vključeno kazalo vsebine</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Postavitev številk strani:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Ime lastnosti predmeta:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Lastnost predmeta, ki bo nastavljena na okvirih kot osnova za ustvarjanje vnosov</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Postavi številke strani vnosov na začetek ali konec vrstice ali pa nikamor</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Pokaži nenatisljive vnose</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Vključi tudi okvire, ki niso vključeni v tiskanje</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Slog odstavka za vnose</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Slog odstavka:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Ciljni okvir:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">Ka&amp;zalo</translation>
    </message>
    <message>
        <source>Table of Contents %1</source>
        <translation>Kazalo vsebine %1</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Kazalo vsebine in seznami</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Kazalo vsebine</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Dodaj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Okvir, v katerega se bo vključilo kazalo vsebine</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Postavitev številk strani:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Ime lastnosti predmeta:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Lastnost predmeta, ki bo nastavljena na okvirih kot osnova za ustvarjanje vnosov</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Postavi številke strani vnosov na začetek ali konec vrstice ali pa nikamor</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Pokaži nenatisljive vnose</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Vključi tudi okvire, ki niso vključeni v tiskanje</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Slog odstavka za vnose</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Slog odstavka:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Ciljni okvir:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">Ka&amp;zalo</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Prezri vse napake</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Samodejno preverjanje pred tiskanjem ali izvažanjem</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Preveri manjkajoče znake</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Preveri predmete ki niso na strani</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Preveri premajhne besedilne okvire</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Preveri uporabo prosojnosti</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Preveri manjkajoče slike</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Preveri ločljivost slik</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Najmanjša dovoljena ločljivost</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF-Files</source>
        <translation type="obsolete">Preveri postavljene PDF datoteke</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Preveri opombe in polja PDF</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Dodaj profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Odstrani profil</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Preveri postavljene datoteke PDF</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Običajne nastavitve</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Postavitev v dokumentu</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>V ozadju</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>V ospredju</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Preskakovanje</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Razdalja preskakovanja:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Doseg:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="obsolete">Vodila</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Prikaži vodila</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation type="obsolete">Meje</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Pokaži meje</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Pokaži mrežo strani</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Glavna mreža</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Barva:</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Razdalje:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Pomožna mreža</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Mreža osnovne črte</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Pokaži mrežo osnovnice</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">&amp;Samodejni razmiki &amp;vrstic:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Mreža osnovnice:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Zamik mreže od osnovnice:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Razdalja med črtami pomožne mreže</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Razdalja med črtami glavne mreže</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Razdalja do vodila, na kateri se bo predmet še pripel nanj</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Doseg oprijemanja predmetov</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Barva črt pomožne mreže</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Barva črt glavne mreže</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Barva vstavljenih vodilnih črt</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Barva vodil robov</translation>
    </message>
    <message>
        <source>Color for the basegrid lines</source>
        <translation type="obsolete">Barva mreže osnovne črte</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Vključi ali izključi mrežo osnovnice</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Vključi ali izključi mrežo</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Vključi ali izključi vodila</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Vključi ali izključi robove</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Večjost razmaka črt v primerjavi z velikostjo pisave v odstotkih</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Nastavitve osnovnice</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Vodila niso vidna skozi predmete na strani</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Vodila so vidna nad vsemi predmeti na strani</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Barva mreže osnovnice</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Razdalja med črtami mreže osnovnice</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Razdalja od vrha strani do prve osnovnice</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Upravljaj s tabulatorji</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Obseg izvoza</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Vse str&amp;ani</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Izberi strani</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Sukanje:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Možnosti datoteke</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Združljivost:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Povezanost:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Levi rob</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Desni rob</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Ustvari predogledne &amp;sličice</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>&amp;Povezane besedilne okvire shrani kot članke PDF </translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Vključi opombe</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Ločljivost:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>&amp;Stisni vektorsko grafiko in besedila</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Nastavitve slik</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Samodejno</translation>
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
        <translation>Brez</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metoda:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Kakovost:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>največja</translation>
    </message>
    <message>
        <source>High</source>
        <translation>visoka</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>srednja</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>nizka</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>najmanjša</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;Slike reduciraj na:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>Splo&amp;šno</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Vključi vse pisave</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">&amp;Omejeno vključi vse pisave</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Vdelovanje</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Pisave na voljo:</translation>
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
        <translation>Vdelane pisave:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Pisave za omejiti:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Pisave</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Omogoči &amp;predstavitvene učinke</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Stran</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Pokaži pred&amp;oglede strani</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Učinki</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Dolžina &amp;prikazovanja:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Trajanje &amp;učinka:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>&amp;Vrsta učinka:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Premikanje črt:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Iz:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Smer:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> s</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Brez učinkov</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Zavese</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Okvir</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Preliv</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Bleščice</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Razdeli</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Pomik</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vodoravno</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Navpično</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Navznotraj</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Navzven</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Z leve proti desni</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Od zgoraj navzdol</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Od spodaj navzgor</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Z desne proti levi</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Od zgoraj levo do spodaj desno</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Dodaj učinek na vse strani</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Dodatno</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Uporabi šifriranje</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Gesla</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Uporabnik:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Lastnik:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Nastavitve</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Dovoli &amp;tiskanje dokumenta</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Dovoli &amp;spreminjanje dokumenta</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Dovoli &amp;kopiranje besedila in slik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Dovoli &amp;dodajanje opomb in polj</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Varnost</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Splošno</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Izvoz je &amp;namenjen za:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>zaslon / splet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>tiskalnik</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Sivinsko</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Uporabi posebne nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Nastavitve upodabljanja</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Frekvenca:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Kot:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>Funkcija &amp;spot:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enostavna točka</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Črta</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Okroglo</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipsa</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Polne barve:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Uporabi profil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Namen upodobitve:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Zaznavno</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativne barvne vrednosti</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Nasičenost</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutne barvne vrednosti</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Slike:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ne uporabi vdelanih profilov ICC</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Barva</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Namen upodobitve PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Podatki:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Profil &amp;izhoda:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Obrezava</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Vključi pisave v PDF. To bo omogočilo, da se pri
prenosu izgled in postavitev dokumenta ne spremenita.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Omogoči učinke za predstavitve, kadar uporabljate Acrobat Reader v celozaslonskem načinu.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Pokaži predoglede strani vsake zgoraj navedene strani.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Čas kazanja strani, preden se prezentacija prične na izbrani strani.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Dolžina učinka. Krajši čas učinek pospeši, daljši upočasni.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Vrsta učinka ob prikazu.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Smer učinka gibajočih črt pri učinkih razcepa in zavese.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Začetni položaj učinkov škatle in razcepa.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Smer učinkov bleščice in pomika.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Dodaj učinek vsem stranem.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Izvozi vse strani v PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Izvozi obseg strani v PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Vpišite z vejicami ločen spisek območij.
Območje je lahko * za vse strani, 1-5 za 
več strani ali samo številka ene strani.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Določi združljivost PDF. Privzeta je naširše združljiva oblika Acrobat 4.
Izberite Acrobat 5, če vaš dokument uproablja zmožnosti PDF-1.4, kot na
primer prosojnosti, ali če potrebujete enkripcijo.
PDF/X-3 se uporablja za pošiljanje v tiskarno, in je omogočeno le, če 
uporabljate tudi upravljanje barv.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Določi povezovanje strani PDF. Če niste prepričani
pustite na privzeti vrednosti - levo.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Pripravi pomanjšane sličice vsake strani v PDFju.
Nekateri pregledovalniki jih lahko uporabijo za pregledovanje.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Ustvari članke PDF, kar je koristno pri pregledovanju povezanih člankov v PDF-ju.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Vključi povezave iz dokumenta.
To je uporabno za pregledovanje dolgih PDFjev.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Izvozna resolucija za besedilo in vektorsko grafiko.
To ne vpliva na resolucijo slik.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Stiskanje besedila in slik.
Če nimate dobrega razloga, raje pustite to vključeno, ker zmanjša velikost PDFja.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Vrsta stiskanja slik.
Samodejno dovoli Scribusu, da sam izbere najboljši način.
ZIP je uporabna za slike z barvnimi ploskvami.
JPEG je boljši pri ustvarjanju majhnih PDFjev z mnogo slikami.
Če ne potrebujete posebnih nastavitev stiskanja, raje pustite samodejno.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Raven stiskanja: najmanj (25%), malo (50%), srednje (75%), močno (85%), najmočneje (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Pretvori slike na izbrano DPI resolucijo.
Če tega ne izberete, bodo slike risane pri izvirni resoluciji.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (pik na palec - Dots Per Inch) pri izvozu slik.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Omogoči varnostne zmožnosti v izvoznem PDFju.
Če izberete Acrobat 4, bo PDF zaščiten s 40 bitno enkripcijo.
Če izberete Acrobat 5, bo PDF zaščiten s 128 bitno enkripcijo.
Pozor: PDF enkripcija ni tako zanesljiva kot GPG ali PGP in ima nekatere omejitve.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Izberite glavno geslo, ki bo o(ne)mogočilo vse varnostne možnosti v izvoženem PDFju</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Izberite geslo, s katerim bodo uporabniki lahko brali vaš PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Dovoli tiskanje PDF-ja. Če ne izberete, bo tiskanje onemogočeno. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Dovoli spreminjanje PDF-ja. Če ne izberete, bo spreminjanje onemogočeno.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Dovoli kopiranje besedila in slik iz PDFja.\n
Če ne izberete, se besedila in slik ne bo dalo presneti.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Dovolite dodajanje opomb in drugih polj v PDF.\n
Če ne izberete, bo dodajanje polj onemogočeno.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Barvni model za izvoz vašega PDFja.
Izberite Zaslon/splet za PDFje ki bodo brani na ekranu ali tiskani na običajnih tiskalnikih.
Izberite Tiskalnik, za tiskanje s CMYK tiskalniki.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">To je napredna nastavitev, ki ni privzeta. Vključite jo le, če jo potrebujete zaradi tiskalnika ali
navodil iz tiskarne. Sicer se vaš PDF morda ne bo pravilno natisnil in ne prav prikazal na drugih
sistemih.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Vdelaj barvni profil za polne barve</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Polno barvanje</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Namen upodobitve polnih barv</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Vdelaj barvni profil za slike</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ne uporabi barvnih profilov iz izvirnih slik</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Barvni profil za slike</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Namen upodobitve slik</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Izvozni profil za tiskanje. Če je mogoče, izberite profil v povezavi z vašim tiskalnikom.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Ključna beseda za PDF/X-3, sicer izvoz ne uspe pravilno.
Predlagamo, da uporabite naslov dokumenta.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Odmik od fizičnega vrha strani</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Odmik od fizičnega dna strani</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Odmik od fizičnega levega robu strani</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Odmik od fizičnega desnega robu strani</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Vodoravno prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Navpično prezrcali stran(i)</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="obsolete">Vključi sloje</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Ločljivost slik EPS:</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="obsolete">Popravi slike na:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="obsolete">Vključi predstavitvene učinke ob uporabi Adobe&amp;#174; Reader&amp;#174; v celozaslonskem načinu.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Določi združljivost PDF.
Privzeta je PDF 1.3, ki ponuja najširšo združljivost.
Izberite PDF 1.4, če vaš dokument uporablja prosojnosti ali potrebuje 128 bitno enkripcijo.
PDF/X-3 je namenjen izvozu, kjer potrebujete dokument z upravljanjem barv za komercialni tisk in ga lahko izberete le, če imate vključeno upravljanje barv. Uporabite ga le, če vam tako naroči tiskarna ali v nekaterih primerih tiskanja s štiribarvnimi digitalnimi barvnimi laserskimi tiskalniki.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="obsolete">Sloji dokumenta se izvozijo v PDF
Možno le ob izbiri PDF 1.5.</translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Vključi stiskanje besedila in slik brez izgub.
Če nimate dobrega razloga pustite to vključena. S tem zmanjšate velikost PDF datoteke.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Metoda kompresije slik. Samodejna dovoli Scribusu, da sam izbere najboljšo metodo. ZIP je stiskanje brez izgub in se obnese pri slikah s čistimi barvami. JPEG ustvari manjše PDF datoteke, če vsebujejo mnogo slik (z minimalno izgubo kakovosti). Pustite na Samodejno, razen če potrebujete posebno nastavitev.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="obsolete">Ponovno upodobljevanje rasterskih slik na izbrano ločljivost.
Če pustite neobkljukano se bodo slike upodobile pri njihovih izvornih ločljivostih.
To lahko poveča porabo pomnilnika in upočasni izvoz.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Omogoči varnostne zmožnosti izvoženega PDFja.
Če izberete PDF 1.3, bo zaščiten s 40 bitno enkripcijo.
Če izberete PDF 1.4, bo zaščiten s 128 bitno enkripcijo.
Opozorilo: PDF enkripcija ni tako zanesljiva kot GPG ali PGP enkripcija in vsebuje nekatere omejitve.</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Pretvori čiste barve v procesne barve</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>&amp;Kakovost stiskanja:</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="obsolete">Ravni kvaliteta stiskanja: najmanj (25%), malo (50%), srednje (75%), močno (85%), najmočneje (95%). Upoštevajte, da raven kvalitete ne pomeni nujno velikosti končne slike - tako velikost kakor izguba kvaliteta je različna pri vsaki sliki glede na raven stiskanja.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Dovoli kopiranje besedila in slik iz PDF-ja. Če ne izberete, besedila in slik ne bo mogoče kopirati.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Dovolite dodajanje opomb in drugih polj v PDF. Če ne izberete, bo dodajanje polj onemogočeno.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Vključi čiste barve, da se pretvorijo v kompozitne. Če ne nameravate tiskati čistih barv pri tiskarni potem je verjetno najbolje, da to pustite vključeno.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Vključi pla&amp;sti</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Metoda &amp;stiskanja:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>&amp;Prevzorči slike na:</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Vključi pisave v PDF. To bo omogočilo, da se pri prenosu izgled in postavitev dokumenta ne spremenita.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Dolžina trajanja učinka. Krajši čas učinek pospeši, daljši upočasni.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Vnesite z vejicami ločen seznam območij. Območje je lahko * za vse strani, 1-5 za več strani ali samo številka posamične strani.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Določi vezavo strani PDF. Če niste prepričani, pustite na privzeti vrednosti - levo.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Pripravi pomanjšane sličice vsake strani v PDF-ju. Nekateri pregledovalniki jih lahko uporabijo za pregledovanje.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Vdela povezave iz dokumenta. To je uporabno za pregledovanje dolgih PDF-jev.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Ločljivost izvoza za besedilo in vektorsko grafiko. To ne vpliva na ločljivost slik.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Vključi stiskanje besedila in slik brez izgub. Če nimate dobrega razloga, pustite to možnost vključeno. S tem zmanjšate velikost datoteke PDF.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Omogočite varnostne zmožnosti izvoženega PDF-ja. Če izberete PDF 1.3, bo zaščiten s 40-bitnim šifriranjem. Če izberete PDF 1.4, bo zaščiten s 128-bitnim šifriranjem. Opozorilo: Šifriranje PDF ni tako zanesljivo kot šifriranje GPG ali PGP ter vsebuje nekatere omejitve.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Izberite glavno geslo, ki bo o(ne)mogočilo vse varnostne možnosti v izvoženem PDF-ju</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>To je napredna nastavitev, ki ni privzeta. Vključite jo le, če jo potrebujete zaradi tiskalnika ali navodil iz tiskarne. Sicer se vaš PDF morda ne bo pravilno natisnil in ne pravilno prikazan na drugih sistemih.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Obvezen niz za PDF/X-3, sicer izvoz ne bo v skladu s standardom PDF/X-3. Predlagamo, da uporabite naslov dokumenta.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Nastavitve prikazovanja</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="obsolete">Prikaz strani</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Posamezna stran</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Zvezne strani</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Dvojna stran levo</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Dvojna stran desno</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Videz</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Uporabi privzeti pogled</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Uporabi celozaslonski način</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Pokaži zavihek Zaznamki</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Pokaži sličice</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Pokaži zavihek Plasti</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Skrij orodno vrstico Pogled</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Skrij menu vrstico Pogled</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Poveča/pomanjša strani, da se pogled prilagodi oknu</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Posebna dejanja</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Brez skript</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Ogledovalnik</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Poreži do robov strani</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Z izgubami - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Brez izgub - Zip</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Način stiskanja slik</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript bo pognan,
ko se odpre dokument PDF:
</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Omogoči predstavitvene učinke ob uporabi bralnika Adobe&amp;#174; Reader&amp;#174; in ostalih prikazovalnikov PDF, ki to podpirajo v celozaslonskem načinu.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Določi združljivost PDF.
Privzeta je PDF 1.3, ki ponuja najširšo združljivost. Izberite PDF 1.4, če vaš dokument uporablja prosojnosti ali potrebuje 128-bitno šifriranje. PDF/X-3 je namenjen izvozu, kjer potrebujete dokument z upravljanjem barv za komercialni tisk in ga lahko izberete le, če imate vključeno upravljanje barv. Uporabite ga le, če vam tako naroči tiskarna ali v nekaterih primerih tiskanja s štiribarvnimi digitalnimi barvnimi laserskimi tiskalniki.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Plasti dokumenta se izvozijo v PDF. Možno le ob izbiri PDF 1.5.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options. This only affects JPEG images</source>
        <translation type="obsolete">Metoda kompresije slik. Samodejna dovoli Scribusu, da sam izbere najboljšo metodo. ZIP je stiskanje brez izgub in se obnese pri slikah s čistimi barvami. JPEG ustvari manjše PDF datoteke, če vsebujejo mnogo slik (z minimalno izgubo kakovosti). Pustite na Samodejno, razen če potrebujete posebno nastavitev. To vpliva le na JPEG slike</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Ponovno vzorčenje rastrskih slik na izbrano ločljivost. Če pustite neizbrano, se bodo slike upodobile pri njihovih izvornih ločljivostih. To lahko poveča porabo pomnilnika in upočasni izvoz.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Barvni model za izvoz vašega PDF-ja. Izberite Zaslon/splet za PDF-je, ki bodo izpisani na ekranu ali tiskani na običajnih tiskalnikih. Izberite Tiskalnik za tiskanje s tiskalniki CMYK.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Ne pokaži predmetov, ki so zunaj obsega robov tiskanja strani</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Čas kazanja strani, preden se predstavitev prične na izbrani strani. Nastavitev 0 bo izključila samodejno prehajanje med stranmi.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Uporabljena metoda stiskanja za slike. Samodejno omogoča Scribusu, da izbere najboljšo metodo. ZIP je stiskanje brez izgub in primerno za slike z nespremenljivimi barvami. JPEG je primernejše za manjše datoteke PDF, ki imajo veliko slik (z možno manjšo izgubo kakovosti slik). Pustite na Samodejno, v kolikor nimate posebnih želja za stiskanje.</translation>
    </message>
    <message>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Ravni kakovosti za metode stiskanja z izgubami: najmanjše (25%), nizko (50%), srednje (75%), visoko (85%), največje (95%). Upoštevajte, da raven kakovosti ne določa neposredno velikost izhodne slike - tako velikost kot izguba kakovosti sta različna od slike do slike pri vsaki dani ravni kakovosti. Celo pri najmanjši izgubi - največje - pride pri jpeg vedno do določene izgube kakovosti.</translation>
    </message>
    <message>
        <source>&amp;Embed All</source>
        <translation>&amp;Vdelaj vse</translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation>Pisave za oris:</translation>
    </message>
    <message>
        <source>Outline &amp;All</source>
        <translation>&amp;Oriši vse</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Postavitev dokumenta</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>Vdelaj pisave v PDF. Vdelava pisav bo obdržala postavitev in videz vašega dokumenta. Nekatere pisave (npr. Open Type) so lahko samo podmnožica, zato jih ni mogoče vdelati v različice PDF, starejše od PDF 1.6. </translation>
    </message>
    <message>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation>Podmnožica vseh pisav v PDF. Podmnoženje pisav pomeni, da so vdelani le znaki, uporabljeni v PDF, ne pa cela pisava. Nekatere pisave, kot so pisave Open Type, je mogoče v PDF vdelati le kot podmnožico, saj jih v različice pred PDF 1.6 ni mogoče vdelati v celoti.</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Pisava:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> točk</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Velikost:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Barva polnila:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Barva poteze:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Stolpci:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Oddaljenost:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>V kožuščku hudobnega fanta stopiclja mizar</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Barva &amp;črte:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Senčenje:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>S&amp;enčenje:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Slog črte:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Širina črte:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Slog &amp;črte:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Puščice:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Začetek:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Konec:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Prosto spreminjanje merila</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vodoravna spr. merila:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Navpična spr. merila:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Spremeni merilo slike na velikost okvira</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Ohrani &amp;razmerja</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Barva &amp;polnila:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Najmanj:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Na&amp;jveč:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Po korakih:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Lastnosti besedilnega okvira</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Lastnosti slikovnega okvira</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Lastnosti risanih oblik</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Privzete vrednosti povečav</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Lastnosti risanih črt</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Lastnosti risanih poligonov</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Pisava v novih besedilnih okvirih</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Velikost pisave v novih besedilnih okvirih</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Barva pisave</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Število stolpcev v besedilnih okvirih</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Razmik med stolpci besedilnega okvira</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Vzorec pisave</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Slikovni okviri omogočajo poljubno spremembo merila slik</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Vodoravna sprememba merila slik</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Navpična sprememba merila slik</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Obdrži vodoravno in navpično spremembo merila isti</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Slike v slikovnih okvirih naj imajo spremenjeno merilo do velikosti okvira</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Slike s samodejno spremembo merila ohranijo svoja razmerja</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Barva polnila slikovnih okvirov</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Nasičenost barve polnila</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Barva črte oblik</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Nasičenost barv črt</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Barva polnila pri oblikah</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Slog črt pri oblikah</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Debelina črt pri oblikah</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Najmanjša dovoljena povečava</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Največja dovoljena povečava</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Sprememba povečave v vsaki operaciji povečave</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Barva črt</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Nasičenost barve</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Slog črt</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Širina črt</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Znak za polnjenje tabulatorja:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Širina tabulatorja:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Uporabi vdelano pot obrezovanja</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Predogled na zaslonu</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Predogled v polni ločljivosti</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Predogled v običajni ločljivosti</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Predogled v nizki ločljivosti</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Po meri:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Po meri: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Barva besedila:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Senčenje:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Obroba besedila:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Pika</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Vezaj</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podčrtaj</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Podpisano</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Sprememba merila:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Nadpisano</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Raz&amp;postavitev:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Spre&amp;memba merila:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Pomanjšane velike črke</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Spremem&amp;ba merila:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Razpostavitev nad osnovnico pisave na črti</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativna velikost nadpisanega v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Razpostavitev pod osnovnico pisave na črti</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativna velikost podpisanega v primerjavi z običajno pisavo</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativna velikost pomanjšanih velikih črk v primerjavi z običajno velikostjo pisave</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Podčrtano</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Razpostavitev:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Širina črte:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Prečrtano</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Samodejno &amp;razmikanje vrstic</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Razmik vrstic:</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Velikost razmika vrstic v primerjavi z velikostjo pisave v odstotkih</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Razpostavitev običajne pisave pod osnovnico, izražena v odstotkih višine tistega dela pisave, ki sega pod črto</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Širina vrstice, izražena v odstotkih velikosti pisave</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Razpostavitev običajne pisave nad osnovnico, izražena v odstotkih višine tistega dela pisave, ki sega nad črto</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source> p</source>
        <translation type="obsolete"> pik</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> točk</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Levo</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Vejica</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Desno</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Pika</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Sredina</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Zamik prve vrstice odstavka</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Zamik celotnega odstavka v levo</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Izbriši vse tabulatorje</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Izbriši vse</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Položaj:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Prva &amp;črta:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">&amp;Zamik:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">Levi z&amp;amik:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Brez</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Pika</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Vezaj</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Podčrtaj</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Po meri</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Znak polnjenja:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Po meri:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Po meri: </translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
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
        <source>Line</source>
        <translation type="obsolete">Črta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Stran</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Besedilo</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Vrsta</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="obsolete">Pisava:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Slika</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Ime &quot;%1&quot; ni edinstveno.
Prosim, izberite drugo.</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Lomljena črta</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Oris</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Poligon</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Besedilo po poti</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Podatki</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Pozor</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Združi</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Prosti predmeti</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Stran</translation>
    </message>
</context>
<context>
    <name>UnderlineValues</name>
    <message>
        <source>Auto</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Razpostavitev</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Širina črte</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Dodaj navpično vodilo</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Dodaj vodoravno vodilo</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Odstrani navpično vodilo</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Odstrani vodoravno vodilo</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Premakni navpično vodilo</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Premakni vodoravno vodilo</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Zakleni vodila</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Odkleni vodila</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Premakni</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Spremeni velikost</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Zasukaj</translation>
    </message>
    <message>
        <source>X1: %1, Y1: %2, %3
X2: %4, Y2: %5, %6</source>
        <translation>X1: %1, Y1: %2, %3(new line)
X2: %4, Y2: %5, %6</translation>
    </message>
    <message>
        <source>W1: %1, H1: %2
W2: %3, H2: %4</source>
        <translation>W1: %1, H1: %2(new line)
W2: %3, H2: %4</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Izbira</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Združi</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Izbira/Združi</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Ustvari</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2(new line)
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Poravnaj/razporedi</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Uporabljeni predmeti</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Prekliči</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Nastavi barvo polnila</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Barva1: %1, Barva2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Nastavi senco barve polnila</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Nastavi barvo črte</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Nastavi senco barve črte</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Prezrcali vodoravno </translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Prezrcali navpično</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Zakleni</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Odkleni</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Zakleni velikost</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Odkleni velikost</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Razdruži</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Izbriši</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Preimenuj</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Iz %1
v %2</translation>
    </message>
    <message>
        <source>Apply template</source>
        <translation type="obsolete">Uveljavi predlogo</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Prilepi</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Izreži</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Nastavi prosojnost barve polnila</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Nastavi prosojnost barve črte</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Nastavi slog črte</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Nastavi slog konca črte</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Nastavi slog zgloba črte</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Nastavi širino črte</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Brez sloga</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Nastavi slog črte po meri</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Ne uporabi sloga črte po meri</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Nastavi začetno puščico</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Nastavi končno puščico</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Ustvari tabelo</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Vrstic: %1, stolpcev: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Nastavi pisavo</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Nastavi velikost pisave</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Nastavi širino pisave</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Nastavi barvo polnila pisave</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Nastavi barvo črte pisave</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Nastavi senco polnila pisave</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Nastavi senco črte pisave</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Nastavi spodsekavanje</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Nastavi razmik vrstic</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Nastavi slog odstavka</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Nastavi jezik</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Poravnaj besedilo</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Nastavi učinek besedila</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Slikovni okvir</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Besedilni okvir</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mnogokotnk</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Krivulja bezier</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Lomljena črta</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Pretvori v</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Uvozi sliko SVG</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Uvozi sliko EPS</translation>
    </message>
    <message>
        <source>Import OpenOffice draw image</source>
        <translation type="obsolete">Uvozi OpenOffice draw sliko</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Prazen prostor</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Besedilo poteka okrog okvira</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Besedilo poteka okrog obrobnega okvira</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Besedilo poteka okrog obrobe</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Brez tekočega besedila</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Brez obrobnega okvira</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Brez obrobe</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Stran %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Nastavi spremembo merila slike</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Velikost okvira</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Prosto spreminjanje merila</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Obdrži razmerja</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Prelomi razmerja</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Uredi obrobo</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Uredi obliko</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Ponastavi obrobo</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Uporabi glavno stran</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Nastavi širino pisave</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Uvozi sliko OpenOffice.org Draw</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Dodaj stran</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Dodaj strani</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Izbriši stran</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Izbriši strani</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Dodaj plast</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Izbriši plast</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Preimenuj plast</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Dvigni plast</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Spusti plast</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Pošlji na plast</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Omogoči tiskanje plasti</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Onemogoči tiskanje plasti</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Preimenuj plast</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Dobi sliko</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Besedilo po poti</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Omogoči tiskanje predmeta</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Onemogoči tiskanje predmeta</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Večkratno podvojevanje</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Spremeni zamik slike</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Spremeni merilo slike</translation>
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
        <translation>Ponastavi nadzorno točko</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>Ponastavi nadzorne točke</translation>
    </message>
    <message>
        <source>Modify image effects</source>
        <translation>Spremeni slikovne učinke</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Action History</source>
        <translation>Zgodovina dejanj</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Pokaži samo izbran predmet</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Razveljavi</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>P&amp;onovi</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation>Začetno stanje</translation>
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
        <translation>Poizkus pridobivanja posodobitvene datoteke za Scribus</translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(Navzven ne bodo poslani nikakršni podatki z vašega računalnika)</translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation>Pri pridobivanju posodobitvene datoteke je čas potekel.</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>Napaka pri pridobivanju datoteke posodobitve: %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>Datoteke na strežniku ni mogoče najti</translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>Datoteko različice ni mogoče odpreti: %1
Napaka: %2 v vrstici: %3, stolpcu: %4</translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>Pri iskanju posodobitev za Scribus je prišlo do napake, prosim, preverite svojo spletno povezavo.</translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>Za vašo različico Scribusa %1 posodobitve niso na voljo</translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>Na voljo je ena ali več posodobitev za vašo različico Scribusa (%1):</translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation>Ta seznam lahko vsebuje tudi razvojne različice.</translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Za več podrobnosti obiščite naslov www.scribus.net.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Najmanjši robovi za velikost strani %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Uporabi robove tiskalnika</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Izberite &amp;tiskalnik:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Robovi</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Desno:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Vrh:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Dno:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Levo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;V redu</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>Prekli&amp;či</translation>
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
        <translation>Vnesite vrednost</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Vnesite vrednost in pritisnite &apos;V redu&apos;.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Vnesite vrednost in pritisnite &apos;V redu&apos;</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Pošlji vrednost skripti</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Kratke besede</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Uveljavi neprelomljiv presledek na:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Izbrani okvirji</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktivna &amp;stran</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Vsi predmeti</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Podatki in
Jeziki</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Privzete vrednosti zamenjaj z uporabnikovimi</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Če obstaja uporabniška nastavitena datoteka
(%1)
lahko te nastavitve dodate k splošnim
nastavitvam tako, da ne obkljukate polja.

Prednastavljene vrednosti lahko zamenjate s 
svojimi, če ga obkljukate.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Preračunavanje le izbranih okvirjev.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Preračunavanje le aktive strani.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Preračunavanje vseh predmetov dokumenta.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Scribusove kratke besede</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Dostopne v naslednjih jezikih</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">O kratkih besedah</translation>
    </message>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="obsolete">Uredi nastavitve &amp;sistema...</translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="obsolete">Uredi nastavitve &amp;uporabnika...</translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="obsolete">Ur&amp;ejevalnik nastavitev...</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">Ur&amp;edi</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="obsolete">Podatk&amp;i in jeziki...</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Pomoč</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="obsolete">Kratke besede</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="obsolete">Urejate datoteko, ki je samo berljiva.
%1</translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="obsolete">Napaka &apos;%1&apos;. Ne morem pognati.</translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="obsolete">Nastavitev kratkih besed</translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation type="obsolete">Ime zagonske datoteke urejevalnika besedil:</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Orodja</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Uredi besedilo z urejevalnikom zgodbe</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Poveča ali pomanjša </translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Vstavi črte</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Vstavi tabelo</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Vstavi sliko</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Izberi predmete</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Uredi vsebino okvirja</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Riše različne oblike </translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Poveži okvirje besedila</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Vstavi Bezier krivulje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Zavrti predmet</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Razveži okvire besedila</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Vstavi prostoročno črto</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Lastnosti ...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Vstavi poligone</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Vstavi okvir besedila</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Izmeri</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Link</source>
        <translation>Povezava</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Besedilo</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Seznamsko polje</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinirano polje</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Gumb</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Besedilno polje</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Potrditveno polje</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>Orodja za PDF</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Vstavi polja PDF</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Vstavi opombe PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Izberi znak:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Vstavi črke v besedilo pod kazalec</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Izbriši trenutno izbiro.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Zapri to pogovorno okno in se vrni k urejanju besedila.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Vstavi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Zapri</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Izberite uvoznika</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Samodejno</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Najdi samo besedilo</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Uvozi besedilo brez oblikovanja</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Uvoznik:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodiranje znakov:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Uvozi samo besedilo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Odpri</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Izberite uvoznika</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Zapomni si povezavo</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Zapomni si končnico datoteke - povezavo uvoznika
in ne vprašuj več po njem ob odpiranju takih datotek.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">V redu</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Zapomni si končnico datoteke in ne sprašuj več ob uvažanju datotek te vrste.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nova iz predloge</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Vse</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Opis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uporaba</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Ustvarjeno z</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Avtor</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Odstrani</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Odpri</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Prenašanje predlog</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Nameščanje predlog</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Izvleci paket v mapo s predlogami: za trenutnega uporabnika ~/.scribus/templates, za vse uporabnike sistema pa 
PREDPONA/share/scribus/templates.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Pripravljanje predloge</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Odstranjevanje predloge</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Prevajanje template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Predloge dokumentov lahko najdete na http://www.scribus.net/, v rubriki &quot;Downloads&quot;.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Prepričajte se, da uporabljate prosto dostopne slike in pisave. Če pisave niso za prosto uporabo, jih ne shranjujte v predlogo.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Kdor ustvarja predlogo, mora poskrbeti, da se gornji razdelek &quot;Nameščanje predlog&quot; nanaša tudi na njegove predloge. Tako bodo uporabniki lahko uporabili paket s predlogo v svojih mapah za predloge.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Odstranjevanje predloge preko pogovornega okna &quot;Nova iz predloge&quot; odstrani le vnos v datoteki template.xml, ne pa tudi dejanske datoteke. Meni za brisanje se prikaže le, le imate pravico pisanja v datoteko template.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Skopiraj obstoječo datoteko template.xml v datoteko z imenom template.jezik_DRŽAVA.xml (uporabite isto jezikovno oznako kot za .qm datoteko vašega jezika), na primer template.sl_SI.xml za slovensko datoteko s predlogami. Kopija mora biti v isti mapi kot izvirna datoteka template.xml.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Shrani kot predlogo</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ime</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Vrsta</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Velikost strani</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Barve</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Opis</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uporaba</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Avtor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-pošta</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Bolj podrobno</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>V redu</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Manj podrobno</translation>
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
        <translation>ležeče</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>pokončno</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>po meri</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Ustvari filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Počisti</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>I&amp;zbriši</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Izberite shranjen filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Izberite ime za shranjevanje filtra</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Izberite ime za shranjevanje</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;V redu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">Prekli&amp;či</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Izključi ali vključi to vrstico filtra</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Odstrani to vrstico filtra</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Dodaj novo vrstico filtra</translation>
    </message>
    <message>
        <source>to</source>
        <translation>v</translation>
    </message>
    <message>
        <source>and</source>
        <translation>in</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>odstrani ujemajoče</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ne odstrani ujemajočih</translation>
    </message>
    <message>
        <source>words</source>
        <translation>besede</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Odstrani</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Zamenjaj</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Uporabi</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Vrednost na levi je regularni izraz</translation>
    </message>
    <message>
        <source>with</source>
        <translation>z</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>slog odstavka</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>vse instance</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>vsi odstavki</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>odstavki, ki se začenjajo z</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>odstavki z manj kot</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>odstavki z več kot</translation>
    </message>
</context>
</TS>
